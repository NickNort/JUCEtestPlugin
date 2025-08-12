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

    // Draw frequency grid lines and labels
    g.setColour(juce::Colour(0xff333333));

    // Vertical frequency lines (logarithmic spacing for better visualization)
    const float freqs[] = { 20.0f, 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f, 2000.0f, 5000.0f, 10000.0f, 20000.0f };
    const float nyquist = sampleRate / 2.0f; // Maximum displayable frequency

    for (auto freq : freqs)
    {
        if (freq <= nyquist) // Only draw frequencies within Nyquist limit
        {
            // Enhanced logarithmic mapping that spreads out low frequencies more
            float normalizedFreq = freq / nyquist;
            // Use a more aggressive logarithmic curve for better low-frequency spacing
            float logNormalizedFreq = std::log(normalizedFreq * 19.0f + 1.0f) / std::log(20.0f);
            float x = juce::jmap(logNormalizedFreq, 0.0f, 1.0f, 0.0f, (float)getWidth());
            
            if (x >= 0 && x <= getWidth())
            {
                g.drawVerticalLine(juce::roundToInt(x), 0.0f, (float)getHeight());
                
                // Add frequency labels
                g.setColour(juce::Colour(0xff888888));
                g.setFont(juce::Font(10.0f));
                
                juce::String labelText;
                if (freq >= 1000.0f)
                    labelText = juce::String(freq / 1000.0f, 1) + "k";
                else
                    labelText = juce::String((int)freq);
                
                // Draw label at bottom
                g.drawText(labelText, 
                          juce::roundToInt(x) - 15, getHeight() - 20, 
                          30, 15, 
                          juce::Justification::centred);
            }
        }
    }

    // Horizontal amplitude lines and labels
    g.setColour(juce::Colour(0xff333333));
    const float dbValues[] = { -60.0f, -40.0f, -20.0f, 0.0f };
    
    for (auto dbValue : dbValues)
    {
        // Map dB to screen position (assuming -60dB to 0dB range)
        float normalizedValue = juce::jmap(dbValue, -60.0f, 0.0f, 0.0f, 1.0f);
        float y = juce::jmap(normalizedValue, 0.0f, 1.0f, (float)getHeight(), 0.0f);
        
        if (y >= 0 && y <= getHeight())
        {
            g.drawHorizontalLine(juce::roundToInt(y), 0.0f, (float)getWidth());
            
            // Add amplitude labels
            g.setColour(juce::Colour(0xff888888));
            g.setFont(juce::Font(10.0f));
            
            juce::String labelText = juce::String((int)dbValue) + "dB";
            
            // Draw label on the left side
            g.drawText(labelText, 
                      5, juce::roundToInt(y) - 7, 
                      30, 15, 
                      juce::Justification::centredLeft);
        }
    }

    // Reset color for grid lines
    g.setColour(juce::Colour(0xff333333));

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

    auto logFreqBase = 20.0f;
    auto logFreqDivisor = 19.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        // Enhanced logarithmic frequency mapping that spreads out low frequencies
        auto logPosition = (float)i / (float)scopeSize;
        // Use matching logarithmic curve - inverse of the paint method
        auto frequency = (std::pow(logFreqBase, logPosition) - 1.0f) / logFreqDivisor;
        
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(frequency * (float)fftSize * 0.5f));

        auto level = juce::jmap(juce::jlimit(mindB, maxdB,
            juce::Decibels::gainToDecibels(fftData[fftDataIndex]) -
            juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;

        // Apply smoothing
        smoothedScopeData[i] = smoothedScopeData[i] * (1.0f - smoothingFactor) + scopeData[i] * smoothingFactor;
    }
}