#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JP8080ControllerAudioProcessorEditor::JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set initial window size
    setSize (800, 600);
}

JP8080ControllerAudioProcessorEditor::~JP8080ControllerAudioProcessorEditor()
{
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Title
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawText ("JP-8080 Controller", getLocalBounds().removeFromTop(60),
                juce::Justification::centred, true);

    // Version info
    g.setFont (12.0f);
    g.setColour (juce::Colours::grey);
    g.drawText ("v0.1.0 - Phase 1: Project Setup", getLocalBounds().removeFromBottom(30),
                juce::Justification::centred, true);
}

void JP8080ControllerAudioProcessorEditor::resized()
{
    // GUI layout will be implemented in Phase 4
}
