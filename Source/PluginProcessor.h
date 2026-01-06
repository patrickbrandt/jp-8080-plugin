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
class JP8080ControllerAudioProcessor  : public juce::AudioProcessor,
                                         private juce::AudioProcessorValueTreeState::Listener
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
    // Parameter access
    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }

private:
    //==============================================================================
    // Parameter Management
    juce::AudioProcessorValueTreeState apvts;

    // Parameter change callback
    void parameterChanged (const juce::String& parameterID, float newValue);

    // Create parameter layout for APVTS
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Track last sent parameter values to avoid redundant MIDI messages
    std::map<juce::String, int> lastSentValues;
    int lastSentBank = -1;
    int lastSentProgram = -1;

    // Queue for pending waveform changes
    struct WaveformChange {
        juce::String paramID;
        int value;
    };
    juce::AbstractFifo waveformChangeFifo { 32 };
    std::array<WaveformChange, 32> waveformChangeBuffer;

    // Direct MIDI output for SysEx (bypasses DAW MIDI routing)
    std::unique_ptr<juce::MidiOutput> directMidiOutput;
    juce::String selectedMidiOutputId;
    void sendSysExDirect(const std::vector<uint8_t>& sysexData);

public:
    // MIDI output device selection
    juce::Array<juce::MidiDeviceInfo> getAvailableMidiOutputs() const;
    juce::String getSelectedMidiOutputId() const { return selectedMidiOutputId; }
    juce::String getSelectedMidiOutputName() const;
    void setSelectedMidiOutput(const juce::String& deviceId);
    void refreshMidiOutput();

private:

    //==============================================================================
    // Helper methods for MIDI output
    void sendMidiCC (juce::MidiBuffer& midiMessages, int ccNumber, int value, int channel);
    void sendBankSelectAndProgramChange (juce::MidiBuffer& midiMessages, int bank, int program, int channel);

    // SysEx helper methods
    uint8_t calculateRolandChecksum (const std::vector<uint8_t>& addressAndData);
    void sendSysExMessage (juce::MidiBuffer& midiMessages, const std::vector<uint8_t>& sysexData);
    void sendWaveformSysEx (juce::MidiBuffer& midiMessages, const juce::String& paramID, int waveformValue);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JP8080ControllerAudioProcessor)
};
