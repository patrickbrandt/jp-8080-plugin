#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JP8080ControllerAudioProcessor::JP8080ControllerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

JP8080ControllerAudioProcessor::~JP8080ControllerAudioProcessor()
{
}

//==============================================================================
const juce::String JP8080ControllerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JP8080ControllerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JP8080ControllerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JP8080ControllerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JP8080ControllerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JP8080ControllerAudioProcessor::getNumPrograms()
{
    return 1;
}

int JP8080ControllerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JP8080ControllerAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String JP8080ControllerAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void JP8080ControllerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void JP8080ControllerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void JP8080ControllerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JP8080ControllerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JP8080ControllerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // MIDI output processing
    // The midiMessages buffer is where we'll add MIDI CC messages to send to the JP-8080
    // Parameter changes will trigger MIDI CC messages (to be implemented in Phase 2)

    // For now, this is ready to accept and pass through MIDI messages
    // MIDI CC generation will be added when parameters are implemented
}

//==============================================================================
bool JP8080ControllerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JP8080ControllerAudioProcessor::createEditor()
{
    return new JP8080ControllerAudioProcessorEditor (*this);
}

//==============================================================================
void JP8080ControllerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void JP8080ControllerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// MIDI Configuration Methods

void JP8080ControllerAudioProcessor::setMidiChannel (int channel)
{
    // Ensure channel is in valid range (1-16)
    midiChannel = juce::jlimit (1, 16, channel);
}

void JP8080ControllerAudioProcessor::sendMidiCC (juce::MidiBuffer& midiMessages,
                                                   int ccNumber, int value, int channel)
{
    // Ensure values are in valid MIDI range
    ccNumber = juce::jlimit (0, 127, ccNumber);
    value = juce::jlimit (0, 127, value);
    channel = juce::jlimit (1, 16, channel);

    // Create MIDI CC message
    // MIDI channels are 0-15 internally, but displayed as 1-16
    auto message = juce::MidiMessage::controllerEvent (channel - 1, ccNumber, value);

    // Add to MIDI buffer at sample position 0
    midiMessages.addEvent (message, 0);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JP8080ControllerAudioProcessor();
}
