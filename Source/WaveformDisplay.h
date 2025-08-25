#pragma once
#include <JuceHeader.h>

class NickPluginAudioProcessor;

class WaveformDisplay : public juce::Component, private juce::Timer
{
public:
    WaveformDisplay(NickPluginAudioProcessor& processor);
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateWaveform();

    NickPluginAudioProcessor& audioProcessor;
    std::vector<float> waveformBuffer;
    int numSamplesToDraw = 1024; // Number of samples to display
    juce::Colour backgroundColour = juce::Colours::black;
    juce::Colour waveformColour = juce::Colour(0xffae87b8);
};
