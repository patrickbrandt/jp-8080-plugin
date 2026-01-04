#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JP8080ControllerAudioProcessorEditor::JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    using namespace JP8080Parameters;

    // Set window size
    setSize (500, 350);

    // MIDI Channel Control
    midiChannelLabel.setText ("MIDI Channel:", juce::dontSendNotification);
    midiChannelLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (midiChannelLabel);

    midiChannelCombo.addItemList ({"1", "2", "3", "4", "5", "6", "7", "8",
                                    "9", "10", "11", "12", "13", "14", "15", "16"}, 1);
    addAndMakeVisible (midiChannelCombo);
    midiChannelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::midiChannel, midiChannelCombo);

    // Patch Bank Control
    patchBankLabel.setText ("Patch Bank:", juce::dontSendNotification);
    patchBankLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (patchBankLabel);

    patchBankCombo.addItemList (patchBankNames, 1);
    addAndMakeVisible (patchBankCombo);
    patchBankAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::patchBank, patchBankCombo);

    // Patch Program Control
    patchProgramLabel.setText ("Patch Program:", juce::dontSendNotification);
    patchProgramLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (patchProgramLabel);

    patchProgramSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    patchProgramSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 50, 20);
    patchProgramSlider.setRange (1, 64, 1);
    addAndMakeVisible (patchProgramSlider);
    patchProgramAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::patchProgram, patchProgramSlider);
}

JP8080ControllerAudioProcessorEditor::~JP8080ControllerAudioProcessorEditor()
{
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    g.fillAll (juce::Colour (0xff2a2a2a));

    // Header background
    g.setColour (juce::Colour (0xff1a1a1a));
    g.fillRect (0, 0, getWidth(), 60);

    // Title
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (28.0f, juce::Font::bold));
    g.drawText ("JP-8080 Controller", 20, 15, getWidth() - 40, 30,
                juce::Justification::centred, true);

    // Section header
    g.setColour (juce::Colour (0xff404040));
    g.fillRect (10, 80, getWidth() - 20, 30);
    g.setColour (juce::Colours::lightgrey);
    g.setFont (14.0f);
    g.drawText ("MIDI Configuration", 20, 80, getWidth() - 40, 30,
                juce::Justification::centredLeft, true);

    // Info text
    g.setColour (juce::Colours::grey);
    g.setFont (12.0f);
    g.drawText ("JP-8080 parameter controls (45 CC parameters) are accessible via",
                20, 260, getWidth() - 40, 20, juce::Justification::centred, true);
    g.drawText ("Logic Pro's automation menu for this plugin.",
                20, 280, getWidth() - 40, 20, juce::Justification::centred, true);

    // Version info
    g.setColour (juce::Colour (0xff666666));
    g.setFont (10.0f);
    g.drawText ("v0.1.0 - Phase 4: Functional UI", 10, getHeight() - 20, getWidth() - 20, 20,
                juce::Justification::centredRight, true);
}

void JP8080ControllerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (60); // Header
    bounds.reduce (20, 10);

    bounds.removeFromTop (30); // Section header

    auto row1 = bounds.removeFromTop (40);
    midiChannelLabel.setBounds (row1.removeFromLeft (120));
    row1.removeFromLeft (10);
    midiChannelCombo.setBounds (row1.removeFromLeft (200));

    auto row2 = bounds.removeFromTop (40);
    patchBankLabel.setBounds (row2.removeFromLeft (120));
    row2.removeFromLeft (10);
    patchBankCombo.setBounds (row2.removeFromLeft (200));

    auto row3 = bounds.removeFromTop (40);
    patchProgramLabel.setBounds (row3.removeFromLeft (120));
    row3.removeFromLeft (10);
    patchProgramSlider.setBounds (row3.removeFromLeft (200));
}
