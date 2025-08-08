/*
  ==============================================================================
    SpectrumAnalyzer.cpp
    Created: Spectrum Analyzer Component for JUCE Plugin
  ==============================================================================
*/

#include "SpectrumAnalyzer.h"

//==============================================================================
SpectrumAnalyzer::SpectrumAnalyzer()
    : forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    setOpaque(false);
    startTimerHz(30); // 30 FPS refresh rate

    // Initialize arrays
    std::fill(std::begin(fifo), std::end(fifo), 0.0f);
    std::fill(std::begin(fftData), std::end(fftData), 0.0f);
    std::fill(std::begin(scopeData), std::end(scopeData), 0.0f);
    std::fill(std::begin(smoothedScopeData), std::end(smoothedScopeData), 0.0f);
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a)); // Dark background

    // Draw frequency grid lines
    g.setColour(juce::Colour(0xff333333));

    // Vertical frequency lines (octaves)
    const float freqs[] = { 100.0f, 200.0f, 500.0f, 1000.0f, 2000.0f, 5000.0f, 10000.0f };
    const float sampleRate = 44100.0f; // Assume 44.1kHz
    const float binWidth = sampleRate / fftSize;

    for (auto freq : freqs)
    {
        float bin = freq / binWidth;
        float x = juce::jmap(bin, 0.0f, (float)scopeSize, 0.0f, (float)getWidth());
        if (x >= 0 && x <= getWidth())
        {
            g.drawVerticalLine(juce::roundToInt(x), 0.0f, (float)getHeight());
        }
    }

    // Horizontal amplitude lines
    for (int i = 1; i < 6; ++i)
    {
        float y = juce::jmap((float)i, 0.0f, 6.0f, (float)getHeight(), 0.0f);
        g.drawHorizontalLine(juce::roundToInt(y), 0.0f, (float)getWidth());
    }

    drawFrame(g);
}

void SpectrumAnalyzer::drawFrame(juce::Graphics& g)
{
    // Draw the spectrum
    g.setColour(juce::Colour(0xffae87b8)); // Use your plugin's color scheme

    juce::Path spectrumPath;

    auto width = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();

    spectrumPath.startNewSubPath(0, height);

    for (int i = 1; i < scopeSize; ++i)
    {
        auto x = juce::jmap((float)i, 0.0f, (float)scopeSize, 0.0f, (float)width);
        auto y = juce::jmap(smoothedScopeData[i], 0.0f, 1.0f, (float)height, 0.0f);

        spectrumPath.lineTo(x, y);
    }

    spectrumPath.lineTo(width, height);
    spectrumPath.closeSubPath();

    // Fill the spectrum with gradient
    juce::ColourGradient gradient(juce::Colour(0x44ae87b8), 0, height,
        juce::Colour(0xffae87b8), 0, 0, false);
    g.setGradientFill(gradient);
    g.fillPath(spectrumPath);

    // Draw outline
    g.setColour(juce::Colour(0xffba5ed1));
    g.strokePath(spectrumPath, juce::PathStrokeType(1.0f));
}

void SpectrumAnalyzer::resized()
{
    // Component resized - no specific action needed for now
}

void SpectrumAnalyzer::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        nextFFTBlockReady = false;
        repaint();
    }
}

void SpectrumAnalyzer::pushNextSampleIntoFifo(float sample) noexcept
{
    // If the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            std::fill(std::begin(fftData), std::end(fftData), 0.0f);
            std::copy(std::begin(fifo), std::end(fifo), std::begin(fftData));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}

void SpectrumAnalyzer::drawNextFrameOfSpectrum()
{
    // Apply windowing function
    window.multiplyWithWindowingTable(fftData, fftSize);

    // Perform FFT
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));

        auto level = juce::jmap(juce::jlimit(mindB, maxdB,
            juce::Decibels::gainToDecibels(fftData[fftDataIndex]) -
            juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;

        // Apply smoothing
        smoothedScopeData[i] = smoothedScopeData[i] * (1.0f - smoothingFactor) + scopeData[i] * smoothingFactor;
    }
}