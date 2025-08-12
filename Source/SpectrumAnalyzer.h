/*
  ==============================================================================

    SpectrumAnalyzer.h
    Created: Spectrum Analyzer Component for JUCE Plugin
    
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class SpectrumAnalyzer : public juce::Component, private juce::Timer
{
public:
    SpectrumAnalyzer();
    ~SpectrumAnalyzer() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void pushNextSampleIntoFifo(float sample) noexcept;
    void drawNextFrameOfSpectrum();
    void drawFrame(juce::Graphics& g);
    
    // Method to set the current sample rate
    void setSampleRate(double newSampleRate) { sampleRate = newSampleRate; }
	float getSampleRate() const { return sampleRate; }

private:
    void timerCallback() override;

    static constexpr auto fftOrder = 11;
    static constexpr auto fftSize = 1 << fftOrder;
    static constexpr auto scopeSize = 512;

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    
    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];

    // Smoothing for visual appeal
    float smoothedScopeData[scopeSize];
    static constexpr float smoothingFactor = 0.2f;
    
    double sampleRate = 44100.0; // Default to 44.1k

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};
