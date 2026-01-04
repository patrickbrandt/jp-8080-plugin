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
                       ),
#else
     :
#endif
       apvts (*this, nullptr, "Parameters", createParameterLayout())
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

    // MIDI output processing - Send parameter changes as MIDI CC messages
    // Only send CC when parameter values have changed to avoid flooding MIDI output
    using namespace JP8080Parameters;

    for (const auto& paramID : getAllParameterIDs())
    {
        auto* param = apvts.getParameter(paramID);
        if (param != nullptr)
        {
            // Get normalized value (0.0-1.0) and convert to MIDI range (0-127)
            float normalizedValue = param->getValue();
            int midiValue = static_cast<int>(normalizedValue * 127.0f);

            // Check if value has changed since last sent
            auto it = lastSentValues.find(paramID);
            bool valueChanged = (it == lastSentValues.end()) || (it->second != midiValue);

            if (valueChanged)
            {
                // Get the CC number for this parameter
                int ccNumber = getCCNumber(paramID);

                if (ccNumber >= 0 && ccNumber <= 127)
                {
                    // Send the MIDI CC message
                    sendMidiCC(midiMessages, ccNumber, midiValue, midiChannel);

                    // Update last sent value
                    lastSentValues[paramID] = midiValue;
                }
            }
        }
    }
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
// Parameter Layout Creation
juce::AudioProcessorValueTreeState::ParameterLayout
JP8080ControllerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Helper lambda to create a standard 0-127 parameter
    auto createStandardParam = [](const juce::String& id, const juce::String& name, float defaultValue = 64.0f)
    {
        return std::make_unique<juce::AudioParameterInt>(
            id, name, 0, 127, static_cast<int>(defaultValue));
    };

    // Helper lambda to create a switch parameter (0-63=OFF, 64-127=ON)
    auto createSwitchParam = [](const juce::String& id, const juce::String& name, bool defaultOn = false)
    {
        return std::make_unique<juce::AudioParameterInt>(
            id, name, 0, 127, defaultOn ? 127 : 0);
    };

    using namespace JP8080Parameters;

    // OSCILLATOR SECTION (9 params)
    layout.add(createStandardParam(Oscillator::osc1Control1, getDisplayName(Oscillator::osc1Control1)));
    layout.add(createStandardParam(Oscillator::osc1Control2, getDisplayName(Oscillator::osc1Control2)));
    layout.add(createStandardParam(Oscillator::osc2Range, getDisplayName(Oscillator::osc2Range)));
    layout.add(createStandardParam(Oscillator::osc2FineWide, getDisplayName(Oscillator::osc2FineWide)));
    layout.add(createStandardParam(Oscillator::osc2Control1, getDisplayName(Oscillator::osc2Control1)));
    layout.add(createStandardParam(Oscillator::osc2Control2, getDisplayName(Oscillator::osc2Control2)));
    layout.add(createStandardParam(Oscillator::oscBalance, getDisplayName(Oscillator::oscBalance)));
    layout.add(createStandardParam(Oscillator::xModDepth, getDisplayName(Oscillator::xModDepth), 0.0f));
    layout.add(createStandardParam(Oscillator::oscLfo1Depth, getDisplayName(Oscillator::oscLfo1Depth)));

    // PITCH ENVELOPE SECTION (3 params)
    layout.add(createStandardParam(PitchEnv::depth, getDisplayName(PitchEnv::depth)));
    layout.add(createStandardParam(PitchEnv::attack, getDisplayName(PitchEnv::attack), 0.0f));
    layout.add(createStandardParam(PitchEnv::decay, getDisplayName(PitchEnv::decay), 0.0f));

    // FILTER SECTION (9 params)
    layout.add(createStandardParam(Filter::cutoff, getDisplayName(Filter::cutoff), 127.0f));
    layout.add(createStandardParam(Filter::resonance, getDisplayName(Filter::resonance), 0.0f));
    layout.add(createStandardParam(Filter::keyFollow, getDisplayName(Filter::keyFollow)));
    layout.add(createStandardParam(Filter::lfo1Depth, getDisplayName(Filter::lfo1Depth)));
    layout.add(createStandardParam(Filter::envDepth, getDisplayName(Filter::envDepth)));
    layout.add(createStandardParam(Filter::envAttack, getDisplayName(Filter::envAttack), 0.0f));
    layout.add(createStandardParam(Filter::envDecay, getDisplayName(Filter::envDecay), 64.0f));
    layout.add(createStandardParam(Filter::envSustain, getDisplayName(Filter::envSustain), 127.0f));
    layout.add(createStandardParam(Filter::envRelease, getDisplayName(Filter::envRelease), 64.0f));

    // AMPLIFIER SECTION (6 params)
    layout.add(createStandardParam(Amplifier::level, getDisplayName(Amplifier::level), 100.0f));
    layout.add(createStandardParam(Amplifier::lfo1Depth, getDisplayName(Amplifier::lfo1Depth)));
    layout.add(createStandardParam(Amplifier::envAttack, getDisplayName(Amplifier::envAttack), 0.0f));
    layout.add(createStandardParam(Amplifier::envDecay, getDisplayName(Amplifier::envDecay), 64.0f));
    layout.add(createStandardParam(Amplifier::envSustain, getDisplayName(Amplifier::envSustain), 127.0f));
    layout.add(createStandardParam(Amplifier::envRelease, getDisplayName(Amplifier::envRelease), 64.0f));

    // LFO SECTION (6 params)
    layout.add(createStandardParam(LFO::lfo1Rate, getDisplayName(LFO::lfo1Rate), 64.0f));
    layout.add(createStandardParam(LFO::lfo1Fade, getDisplayName(LFO::lfo1Fade), 0.0f));
    layout.add(createStandardParam(LFO::lfo2Rate, getDisplayName(LFO::lfo2Rate), 64.0f));
    layout.add(createStandardParam(LFO::lfo2PitchDepth, getDisplayName(LFO::lfo2PitchDepth)));
    layout.add(createStandardParam(LFO::lfo2FilterDepth, getDisplayName(LFO::lfo2FilterDepth)));
    layout.add(createStandardParam(LFO::lfo2AmpDepth, getDisplayName(LFO::lfo2AmpDepth)));

    // EFFECTS SECTION (6 params)
    layout.add(createStandardParam(Effects::toneCtrlBass, getDisplayName(Effects::toneCtrlBass)));
    layout.add(createStandardParam(Effects::toneCtrlTreble, getDisplayName(Effects::toneCtrlTreble)));
    layout.add(createStandardParam(Effects::multiFxLevel, getDisplayName(Effects::multiFxLevel), 64.0f));
    layout.add(createStandardParam(Effects::delayTime, getDisplayName(Effects::delayTime), 0.0f));
    layout.add(createStandardParam(Effects::delayFeedback, getDisplayName(Effects::delayFeedback), 0.0f));
    layout.add(createStandardParam(Effects::delayLevel, getDisplayName(Effects::delayLevel), 0.0f));

    // CONTROL SECTION (6 params)
    layout.add(createStandardParam(Control::portamentoTime, getDisplayName(Control::portamentoTime), 0.0f));
    layout.add(createSwitchParam(Control::portamentoSwitch, getDisplayName(Control::portamentoSwitch), false));
    layout.add(createSwitchParam(Control::hold1, getDisplayName(Control::hold1), false));
    layout.add(createStandardParam(Control::modulation, getDisplayName(Control::modulation), 0.0f));
    layout.add(createStandardParam(Control::expression, getDisplayName(Control::expression), 127.0f));
    layout.add(createStandardParam(Control::pan, getDisplayName(Control::pan), 64.0f));

    return layout;
}

//==============================================================================
void JP8080ControllerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save parameter state to memory block
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void JP8080ControllerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore parameter state from memory block
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
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
