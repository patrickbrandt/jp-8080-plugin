#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "JP8080Parameters.h"
#include "JP8080LookAndFeel.h"

//==============================================================================
/**
 * JP-8080 Controller Plugin Editor
 *
 * Phase 4B: Custom JP-8080-style graphics and rotary knobs
 */
class JP8080ControllerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                             private juce::ComboBox::Listener
{
public:
    JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor&);
    ~JP8080ControllerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JP8080ControllerAudioProcessor& audioProcessor;
    JP8080LookAndFeel jp8080LookAndFeel;

    // Helper to create a labeled rotary knob
    void createRotaryKnob(juce::Slider& slider, juce::Label& label,
                         const juce::String& paramID, const juce::String& labelText);

    // ComboBox listener callback
    void comboBoxChanged(juce::ComboBox* comboBox) override;

    // Helper to update patch name ComboBox based on current bank
    void updatePatchNamesForCurrentBank();

    // MIDI Configuration Controls
    juce::Label midiOutputLabel;
    juce::ComboBox midiOutputCombo;
    void populateMidiOutputList();

    juce::Label midiChannelLabel;
    juce::ComboBox midiChannelCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiChannelAttachment;

    juce::Label patchBankLabel;
    juce::ComboBox patchBankCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> patchBankAttachment;

    juce::Label patchNameLabel;
    juce::ComboBox patchNameCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> patchNameAttachment;

    // Oscillator Section
    juce::Label osc1WaveformLabel;
    juce::ComboBox osc1WaveformCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveformAttachment;

    juce::Label osc1Control1Label, osc1Control2Label;
    juce::Slider osc1Control1Knob, osc1Control2Knob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1Control1Attachment, osc1Control2Attachment;

    juce::Label osc2WaveformLabel;
    juce::ComboBox osc2WaveformCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveformAttachment;

    juce::Label osc2RangeLabel, osc2FineLabel, osc2Control1Label, osc2Control2Label;
    juce::Slider osc2RangeKnob, osc2FineKnob, osc2Control1Knob, osc2Control2Knob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2RangeAttachment, osc2FineAttachment,
                                                                            osc2Control1Attachment, osc2Control2Attachment;

    juce::Label oscBalanceLabel, xModDepthLabel, oscLfo1DepthLabel;
    juce::Slider oscBalanceKnob, xModDepthKnob, oscLfo1DepthKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscBalanceAttachment, xModDepthAttachment,
                                                                            oscLfo1DepthAttachment;

    // Filter Section
    juce::Label filterCutoffLabel, filterResonanceLabel, filterKeyFollowLabel, filterLfo1DepthLabel, filterEnvDepthLabel;
    juce::Slider filterCutoffKnob, filterResonanceKnob, filterKeyFollowKnob, filterLfo1DepthKnob, filterEnvDepthKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment, filterResonanceAttachment,
                                                                            filterKeyFollowAttachment, filterLfo1DepthAttachment,
                                                                            filterEnvDepthAttachment;

    // Filter Envelope
    juce::Label filterAttackLabel, filterDecayLabel, filterSustainLabel, filterReleaseLabel;
    juce::Slider filterAttackKnob, filterDecayKnob, filterSustainKnob, filterReleaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAttachment, filterDecayAttachment,
                                                                            filterSustainAttachment, filterReleaseAttachment;

    // Amplifier Envelope
    juce::Label ampLevelLabel, ampAttackLabel, ampDecayLabel, ampSustainLabel, ampReleaseLabel;
    juce::Slider ampLevelKnob, ampAttackKnob, ampDecayKnob, ampSustainKnob, ampReleaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ampLevelAttachment, ampAttackAttachment,
                                                                            ampDecayAttachment, ampSustainAttachment,
                                                                            ampReleaseAttachment;

    // LFO Section
    juce::Label lfo1WaveformLabel;
    juce::ComboBox lfo1WaveformCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1WaveformAttachment;

    juce::Label lfo1RateLabel, lfo1FadeLabel, lfo2RateLabel, lfo2DepthLabel;
    juce::Slider lfo1RateKnob, lfo1FadeKnob, lfo2RateKnob, lfo2DepthKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1RateAttachment, lfo1FadeAttachment,
                                                                            lfo2RateAttachment, lfo2DepthAttachment;

    // Pitch Envelope Section
    juce::Label pitchEnvDepthLabel, pitchEnvAttackLabel, pitchEnvDecayLabel;
    juce::Slider pitchEnvDepthKnob, pitchEnvAttackKnob, pitchEnvDecayKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchEnvDepthAttachment, pitchEnvAttackAttachment,
                                                                            pitchEnvDecayAttachment;

    // Effects Section
    juce::Label toneCtrlBassLabel, toneCtrlTrebleLabel, multiFxTypeLabel, multiFxLevelLabel, delayTypeLabel, delayTimeLabel, delayFeedbackLabel, delayLevelLabel;
    juce::ComboBox multiFxTypeCombo, delayTypeCombo;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> multiFxTypeAttachment, delayTypeAttachment;
    juce::Slider toneCtrlBassKnob, toneCtrlTrebleKnob, multiFxLevelKnob, delayTimeKnob, delayFeedbackKnob, delayLevelKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneCtrlBassAttachment, toneCtrlTrebleAttachment,
                                                                            multiFxLevelAttachment, delayTimeAttachment,
                                                                            delayFeedbackAttachment, delayLevelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JP8080ControllerAudioProcessorEditor)
};
