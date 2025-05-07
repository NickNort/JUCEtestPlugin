/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JUCEtestPluginAudioProcessorEditor::JUCEtestPluginAudioProcessorEditor (JUCEtestPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	addAndMakeVisible(button1);
	button1.setButtonText("This is a button");

    setSize (400, 300);
}

JUCEtestPluginAudioProcessorEditor::~JUCEtestPluginAudioProcessorEditor()
{
}

//==============================================================================
void JUCEtestPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // setting background color
    g.fillAll (juce::Colour(0xff221c2b));

    // setting color of the text
    g.setColour(juce::Colour(0xffb18cbb));
    g.setFont (juce::FontOptions (40.0f));
    g.drawFittedText ("Nick\'s Test Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void JUCEtestPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // defining some margin space:
    auto leftMargin = getWidth() * 0.05;    // dedicate 5% of the width to left margin
    auto rightMargin = getWidth() * 0.95;   // dedicate 5% of the width to right margin
	auto topMargin = getHeight() * 0.05;    // dedicate 5% of the height to top margin
	auto bottomMargin = getHeight() * 0.95; // dedicate 5% of the height to bottom margin
	// button settings:
    auto buttonWidth = (getWidth() * 0.9) * 0.25;   // dedicate 25% of the non-margin space to button width
	auto buttonHeight = buttonWidth * 0.75;         // set the button height to 75% of the button width

    // adding the button and centering it
    button1.setBounds((getWidth() * 0.5) - (buttonWidth * 0.5), ((getHeight() * 0.5) - buttonHeight * 0.5),
        buttonWidth, buttonHeight);
}
