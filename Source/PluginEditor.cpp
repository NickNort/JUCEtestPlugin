/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

// Dial1 settings --------------------------------------------------------------
const int dialWidth = 100;
const int dialHeight = 100;
void JUCEtestPluginAudioProcessorEditor::initDial1() {
    dial1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dial1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    dial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, JUCEtestPluginAudioProcessorEditor::myColor1);
    dial1.setColour(juce::Slider::ColourIds::thumbColourId, JUCEtestPluginAudioProcessorEditor::myColor0);
    dial1.setRange(0.0f, 50.0f, 0.1f);
    dial1.setDoubleClickReturnValue(true, 1.0f);
    addAndMakeVisible(dial1);
}

// Button1 settings ------------------------------------------------------------
const int buttonWidth = 80;
const int buttonHeight = 60;
void JUCEtestPluginAudioProcessorEditor::initButton1() {
    button1.setButtonText("Gain");
    button1.setColour(juce::TextButton::ColourIds::buttonColourId, JUCEtestPluginAudioProcessorEditor::myColor5);
    button1.setColour(juce::TextButton::ColourIds::buttonOnColourId, JUCEtestPluginAudioProcessorEditor::myColor4);
    button1.setClickingTogglesState(true);
    addAndMakeVisible(button1);
}

//==============================================================================
JUCEtestPluginAudioProcessorEditor::JUCEtestPluginAudioProcessorEditor(JUCEtestPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // ValueTreeState attachments
    dial1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "GAINLEVEL", dial1);

    button1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "GAINTOGGLE", button1);

    // adding components
    initButton1();	// adding button1
    initDial1();    // adding dial1

    // Add spectrum analyzer
    addAndMakeVisible(spectrumAnalyzer);

    // Register spectrum analyzer with processor for audio data
    audioProcessor.setSpectrumAnalyzer(&spectrumAnalyzer);

    // configuring the window
    setResizable(true, true);                       // enabling resizing via bottom right corner
    getConstrainer()->setFixedAspectRatio(1.5f);     // setting a fixed aspect ratio for the window
    setResizeLimits(600, 400, 1200, 800);          // setting min and max size for spectrum analyzer
    setSize(600, 400);                             // setting initial size of the window
}

JUCEtestPluginAudioProcessorEditor::~JUCEtestPluginAudioProcessorEditor()
{
    // Unregister spectrum analyzer
    audioProcessor.setSpectrumAnalyzer(nullptr);
}

//==============================================================================
void JUCEtestPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // setting background color
    g.fillAll(JUCEtestPluginAudioProcessorEditor::myColor5);

    // setting color of the text
    g.setColour(JUCEtestPluginAudioProcessorEditor::myColor1);
    g.setFont(juce::FontOptions(40.0f));
    g.drawFittedText("Nick\'s Test Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void JUCEtestPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto headerArea = bounds.removeFromTop(60); // Space for title
    auto controlsArea = bounds.removeFromBottom(120); // Space for controls at bottom
    auto spectrumArea = bounds; // Remaining space for spectrum

    // Spectrum analyzer takes the main area
    spectrumAnalyzer.setBounds(spectrumArea.reduced(10));

    // Controls at the bottom
    auto controlsCentered = controlsArea.withSizeKeepingCentre(200, 100);

    // button settings
    auto buttonBounds = controlsCentered.removeFromRight(100);
    buttonBounds = buttonBounds.withSizeKeepingCentre(buttonWidth, buttonHeight);

    // dial settings:
    auto dialBounds = controlsCentered.removeFromLeft(100);
    dialBounds = dialBounds.withSizeKeepingCentre(dialWidth, dialHeight);

    // Set component bounds
    button1.setBounds(buttonBounds);
    dial1.setBounds(dialBounds);
}