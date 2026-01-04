#pragma once

#include <JuceHeader.h>
#include "JP8080Parameters.h"

//==============================================================================
/**
 * JP-8080 Controller Plugin Processor
 *
 * MIDI controller plugin for the Roland JP-8080 hardware synthesizer.
 * This plugin sends MIDI CC messages to control the JP-8080's parameters.
 */
class JP8080ControllerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    JP8080ControllerAudioProcessor();
    ~JP8080ControllerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // MIDI Configuration
    void setMidiChannel (int channel);
    int getMidiChannel() const { return midiChannel; }

    //==============================================================================
    // Parameter access
    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }

private:
    //==============================================================================
    // Parameter Management
    juce::AudioProcessorValueTreeState apvts;

    // Create parameter layout for APVTS
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Track last sent parameter values to avoid redundant MIDI messages
    std::map<juce::String, int> lastSentValues;

    //==============================================================================
    // MIDI output configuration
    int midiChannel = 1;  // Default MIDI channel (1-16)

    // Helper method to send MIDI CC messages
    void sendMidiCC (juce::MidiBuffer& midiMessages, int ccNumber, int value, int channel);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JP8080ControllerAudioProcessor)
};
