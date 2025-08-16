#include "WaveformDisplay.h"
#include "PluginProcessor.h"

WaveformDisplay::WaveformDisplay(JUCEtestPluginAudioProcessor& processor)
    : audioProcessor(processor)
{
    setOpaque(true);
    startTimerHz(30); // ~30 FPS
}

WaveformDisplay::~WaveformDisplay() {}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColour);
    g.setColour(waveformColour);

    if (waveformBuffer.empty())
        return;

    auto w = getWidth();
    auto h = getHeight();
    float midY = h * 0.5f;
    float xStep = (float)w / (float)waveformBuffer.size();

    juce::Path path;
    // Clamp the first sample
    float firstSample = juce::jlimit(-1.0f, 1.0f, waveformBuffer[0]);
    path.startNewSubPath(0, midY - firstSample * midY);
    for (size_t i = 1; i < waveformBuffer.size(); ++i)
    {
        float x = (float)i * xStep;
        // Clamp each sample to [-1.0, 1.0]
        float y = midY - juce::jlimit(-1.0f, 1.0f, waveformBuffer[i]) * midY;
        path.lineTo(x, y);
    }
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void WaveformDisplay::resized() {}

void WaveformDisplay::timerCallback()
{
    updateWaveform();
    repaint();
}

void WaveformDisplay::updateWaveform()
{
    // Pull latest samples from processor
    audioProcessor.getLatestWaveformSamples(waveformBuffer, numSamplesToDraw);
}
