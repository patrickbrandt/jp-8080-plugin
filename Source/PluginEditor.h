#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "JP8080Parameters.h"

//==============================================================================
/**
 * JP-8080 Controller Plugin Editor
 *
 * Functional GUI for MIDI configuration and parameter access.
 * Phase 4A: MIDI controls + generic parameter access
 * Phase 4B (future): Custom JP-8080-style graphics and knobs
 */
class JP8080ControllerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor&);
    ~JP8080ControllerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JP8080ControllerAudioProcessor& audioProcessor;

    // MIDI Configuration Controls
    juce::Label midiChannelLabel;
    juce::ComboBox midiChannelCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiChannelAttachment;

    juce::Label patchBankLabel;
    juce::ComboBox patchBankCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> patchBankAttachment;

    juce::Label patchProgramLabel;
    juce::Slider patchProgramSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> patchProgramAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JP8080ControllerAudioProcessorEditor)
};
