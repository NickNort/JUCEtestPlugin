/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include <atomic>
#include <vector>
#include "WaveformDisplay.h"

// Forward declaration
class SpectrumAnalyzer;

//==============================================================================
/**
*/
class JUCEtestPluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    JUCEtestPluginAudioProcessor();
    ~JUCEtestPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // for parameters like gain level and gain toggle
    juce::AudioProcessorValueTreeState apvts;

    // Method to register spectrum analyzer
    void setSpectrumAnalyzer(SpectrumAnalyzer* analyzer) { spectrumAnalyzer = analyzer; }
	// Method to analyze waveform shape
	void analyzeWaveformShape(const juce::AudioBuffer<float>& buffer);
    // Waveform circular buffer API
    void pushWaveformSamples(const float* samples, int numSamples);
    void getLatestWaveformSamples(std::vector<float>& dest, int numSamples);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    // Pointer to spectrum analyzer for sending audio data
    SpectrumAnalyzer* spectrumAnalyzer = nullptr;

    // Waveform circular buffer members
    std::vector<float> waveformBuffer;
    std::unique_ptr<juce::AbstractFifo> waveformFifo;
    std::mutex waveformMutex;
    int waveformBufferSize = 8192; // ~200ms at 44.1kHz

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JUCEtestPluginAudioProcessor)
};
