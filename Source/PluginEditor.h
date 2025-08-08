/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SpectrumAnalyzer.h"

//==============================================================================
/**
*/
class JUCEtestPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    JUCEtestPluginAudioProcessorEditor(JUCEtestPluginAudioProcessor&);
    ~JUCEtestPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JUCEtestPluginAudioProcessor& audioProcessor;

    // Color Palette:
    const juce::Colour myColor0 = juce::Colour(0xfffdf5ff); // for white-ish text & dial thumb
    const juce::Colour myColor1 = juce::Colour(0xffae87b8); // dial background & header
    const juce::Colour myColor2 = juce::Colour(0xff9967a6); // 
    const juce::Colour myColor3 = juce::Colour(0xffba5ed1); // button ON
    const juce::Colour myColor4 = juce::Colour(0xff451950); // button OFF
    const juce::Colour myColor5 = juce::Colour(0xff211b29); // window background
    const juce::Colour myColor6 = juce::Colour(0xff261b29); // 

    juce::Slider dial1;
    void initDial1();

    juce::TextButton button1;
    void initButton1();

    // Spectrum Analyzer
    SpectrumAnalyzer spectrumAnalyzer;

    // note about C++: apparently when classes are created, they are created in order from
    // first line to last but when they are destroyed, they are destroyed in reverse order,
    // so this needs to be declared after the dial so that the dial is not destroyed first.
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dial1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> button1Attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JUCEtestPluginAudioProcessorEditor)
};
