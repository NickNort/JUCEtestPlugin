#pragma once
#include <JuceHeader.h>

class JUCEtestPluginAudioProcessor;

class WaveformDisplay : public juce::Component, private juce::Timer
{
public:
    WaveformDisplay(JUCEtestPluginAudioProcessor& processor);
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateWaveform();

    JUCEtestPluginAudioProcessor& audioProcessor;
    std::vector<float> waveformBuffer;
    int numSamplesToDraw = 1024; // Number of samples to display
    juce::Colour backgroundColour = juce::Colours::black;
    juce::Colour waveformColour = juce::Colour(0xffae87b8);
};
