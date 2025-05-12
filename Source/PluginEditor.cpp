/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

// Dial1 settings --------------------------------------------------------------
auto dialWidth = 100;
auto dialHeight = 100;
void JUCEtestPluginAudioProcessorEditor::initDial1() {
    addAndMakeVisible(dial1);
    dial1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dial1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    dial1.setRange(0.0, 10.0, 0.1);
    //dial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour(0xffae87b8));
    dial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, JUCEtestPluginAudioProcessorEditor::myColor1);
    //dial1.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xfff8f5fa));
    dial1.setColour(juce::Slider::ColourIds::thumbColourId, JUCEtestPluginAudioProcessorEditor::myColor0);
    dial1.setDoubleClickReturnValue(true, 0.0);
}

//void JUCEtestPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
//	//if (slider == &dial1) {
//	//	audioProcessor.gainLevel = dial1.getValue();
//	//}
//}

// Button1 settings ------------------------------------------------------------
auto buttonWidth = 80;
auto buttonHeight = 60;
void JUCEtestPluginAudioProcessorEditor::initButton1() {
	addAndMakeVisible(button1);
	button1.setButtonText("This is a button");
	//button1.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xff3e204b));
    button1.setColour(juce::TextButton::ColourIds::buttonColourId, JUCEtestPluginAudioProcessorEditor::myColor4);
	//button1.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(0xffa458a7));
    button1.setColour(juce::TextButton::ColourIds::buttonOnColourId, JUCEtestPluginAudioProcessorEditor::myColor5);
	button1.setClickingTogglesState(true);
}

//void JUCEtestPluginAudioProcessorEditor::buttonClicked(juce::Button* button) {
//	//if (button == &button1) {
//	//	audioProcessor.gainToggle = button1.getToggleState();
//	//}
//}

//==============================================================================
JUCEtestPluginAudioProcessorEditor::JUCEtestPluginAudioProcessorEditor (JUCEtestPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // adding components
	initButton1();	// adding button1
    initDial1();    // adding dial1

    // configuring the window
    setResizable(true, true);                       // enabling resizing via bottom right corner
    getConstrainer()->setFixedAspectRatio(1.5);     // setting a fixed aspect ratio for the window
    setResizeLimits(450, 300, 1800, 1200);          // setting min and max size of the window
    setSize (450, 300);                             // setting initial size of the window
}

JUCEtestPluginAudioProcessorEditor::~JUCEtestPluginAudioProcessorEditor()
{
}

//==============================================================================
void JUCEtestPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // setting background color
    //g.fillAll (juce::Colour(0xff211b29));
    g.fillAll(JUCEtestPluginAudioProcessorEditor::myColor5);

    // setting color of the text
    //g.setColour(juce::Colour(0xffb18cbb));
    g.setColour(JUCEtestPluginAudioProcessorEditor::myColor1);
    g.setFont (juce::FontOptions (40.0f));
    g.drawFittedText ("Nick\'s Test Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void JUCEtestPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	// button settings
    auto buttonLeftBound = getWidth() * 0.5 + 20;
	auto buttonTopBound = getHeight() * 0.5 - (buttonHeight * 0.5);

	// dial settings:
    auto dialLeftBound = getWidth() * 0.5 - dialWidth - 20;
	auto dialTopBound = getHeight() * 0.5 - (dialHeight * 0.5);


    // adding the button and dial
    button1.setBounds(buttonLeftBound, buttonTopBound, buttonWidth, buttonHeight);
    dial1.setBounds(dialLeftBound, dialTopBound, dialWidth, dialHeight);
}
