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

    // MIDI output processing
    using namespace JP8080Parameters;

    // Get current MIDI channel from parameter (1-16)
    auto* channelParam = apvts.getParameter(MidiConfig::midiChannel);
    int currentMidiChannel = channelParam != nullptr ?
        static_cast<int>(channelParam->getValue() * 15.0f) + 1 : 1; // Convert 0.0-1.0 to 1-16

    // Check for Bank Select + Program Change
    auto* bankParam = apvts.getParameter(MidiConfig::patchBank);
    auto* programParam = apvts.getParameter(MidiConfig::patchProgram);

    if (bankParam != nullptr && programParam != nullptr)
    {
        int currentBank = static_cast<int>(bankParam->getValue() * (patchBankNames.size() - 1));
        int currentProgram = static_cast<int>(programParam->getValue() * 63.0f) + 1; // Convert to 1-64

        // Check if bank or program has changed
        if (currentBank != lastSentBank || currentProgram != lastSentProgram)
        {
            sendBankSelectAndProgramChange(midiMessages, currentBank, currentProgram, currentMidiChannel);
            lastSentBank = currentBank;
            lastSentProgram = currentProgram;
        }
    }

    // Check waveform parameters and send SysEx if changed
    // These parameters use SysEx instead of MIDI CC
    const juce::StringArray waveformParams = {
        Oscillator::osc1Waveform,
        Oscillator::osc2Waveform,
        LFO::lfo1Waveform
    };

    for (const auto& paramID : waveformParams)
    {
        auto* param = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(paramID));
        if (param != nullptr)
        {
            int currentValue = param->getIndex();

            // Check if value has changed since last sent
            auto it = lastSentValues.find(paramID);
            bool valueChanged = (it == lastSentValues.end()) || (it->second != currentValue);

            if (valueChanged)
            {
                // Send SysEx message for waveform change
                sendWaveformSysEx(midiMessages, paramID, currentValue);

                // Update last sent value
                lastSentValues[paramID] = currentValue;
            }
        }
    }

    // Send parameter changes as MIDI CC messages
    // Only send CC when parameter values have changed to avoid flooding MIDI output
    for (const auto& paramID : getAllParameterIDs())
    {
        auto* param = apvts.getParameter(paramID);
        if (param != nullptr)
        {
            // Skip waveform parameters (handled above with SysEx)
            if (waveformParams.contains(paramID))
                continue;

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
                    // Send the MIDI CC message using the selected MIDI channel
                    sendMidiCC(midiMessages, ccNumber, midiValue, currentMidiChannel);

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

    // OSCILLATOR SECTION (11 params - added 2 waveform selectors)
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        Oscillator::osc1Waveform,
        getDisplayName(Oscillator::osc1Waveform),
        osc1WaveformNames,
        0)); // Default: SUPER SAW
    layout.add(createStandardParam(Oscillator::osc1Control1, getDisplayName(Oscillator::osc1Control1)));
    layout.add(createStandardParam(Oscillator::osc1Control2, getDisplayName(Oscillator::osc1Control2)));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        Oscillator::osc2Waveform,
        getDisplayName(Oscillator::osc2Waveform),
        osc2WaveformNames,
        0)); // Default: SQR (PWM)
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

    // LFO SECTION (7 params - added 1 waveform selector)
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        LFO::lfo1Waveform,
        getDisplayName(LFO::lfo1Waveform),
        lfo1WaveformNames,
        0)); // Default: TRI
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

    // MIDI CONFIGURATION
    // MIDI Channel: 1-16
    layout.add(std::make_unique<juce::AudioParameterInt>(
        MidiConfig::midiChannel,
        getDisplayName(MidiConfig::midiChannel),
        1, 16, 1)); // Range: 1-16, default: 1

    // Patch Bank Selection
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        MidiConfig::patchBank,
        getDisplayName(MidiConfig::patchBank),
        patchBankNames,
        0)); // Default: User A

    // Patch Program: 1-64 (JP-8080 displays as 11-88)
    layout.add(std::make_unique<juce::AudioParameterInt>(
        MidiConfig::patchProgram,
        getDisplayName(MidiConfig::patchProgram),
        1, 64, 1)); // Range: 1-64, default: 1

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
// MIDI Output Methods

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

void JP8080ControllerAudioProcessor::sendBankSelectAndProgramChange (juce::MidiBuffer& midiMessages,
                                                                       int bankIndex, int program, int channel)
{
    using namespace JP8080Parameters;

    // Get bank select info for the selected bank
    auto bank = static_cast<PatchBank>(juce::jlimit(0, 7, bankIndex));
    auto bankInfo = getBankSelectInfo(bank);

    // Ensure values are in valid MIDI range
    channel = juce::jlimit (1, 16, channel);
    program = juce::jlimit (1, 64, program); // User programs 1-64

    // Calculate actual MIDI program number (0-63) with bank offset
    int midiProgram = (program - 1) + bankInfo.programOffset;
    midiProgram = juce::jlimit (0, 127, midiProgram);

    // Send Bank Select MSB (CC#0)
    sendMidiCC(midiMessages, 0, bankInfo.bankMSB, channel);

    // Send Bank Select LSB (CC#32)
    sendMidiCC(midiMessages, 32, bankInfo.bankLSB, channel);

    // Send Program Change
    auto pcMessage = juce::MidiMessage::programChange(channel - 1, midiProgram);
    midiMessages.addEvent(pcMessage, 0);
}

//==============================================================================
// SysEx Methods

uint8_t JP8080ControllerAudioProcessor::calculateRolandChecksum (const std::vector<uint8_t>& addressAndData)
{
    // Calculate Roland checksum: sum all bytes, then checksum = 128 - (sum mod 128)
    int sum = 0;
    for (uint8_t byte : addressAndData)
        sum += byte;

    return static_cast<uint8_t>(128 - (sum % 128));
}

void JP8080ControllerAudioProcessor::sendSysExMessage (juce::MidiBuffer& midiMessages,
                                                         const std::vector<uint8_t>& sysexData)
{
    // Create MIDI SysEx message and add to buffer
    auto message = juce::MidiMessage::createSysExMessage(sysexData.data(), static_cast<int>(sysexData.size()));
    midiMessages.addEvent(message, 0);
}

void JP8080ControllerAudioProcessor::sendWaveformSysEx (juce::MidiBuffer& midiMessages,
                                                          const juce::String& paramID,
                                                          int waveformValue)
{
    using namespace JP8080Parameters;

    // JP-8080 SysEx format:
    // F0 41 dev 00 06 12 aa bb cc dd ee sum F7
    //
    // F0 = SysEx start
    // 41 = Roland manufacturer ID
    // dev = Device ID (default 10h = 16)
    // 00 06 = Model ID (JP-8080)
    // 12 = DT1 command (Data Set 1)
    // aa bb cc dd = 4-byte address
    // ee = data value
    // sum = checksum
    // F7 = SysEx end

    const uint8_t ROLAND_ID = 0x41;
    const uint8_t DEVICE_ID = 0x10; // Default device ID
    const uint8_t MODEL_ID_MSB = 0x00;
    const uint8_t MODEL_ID_LSB = 0x06;
    const uint8_t DT1_COMMAND = 0x12;

    // Temporary Performance Patch (Upper) base address: 01 00 40 00
    // We'll use Upper part for now
    const uint8_t ADDR_BYTE1 = 0x01;
    const uint8_t ADDR_BYTE2 = 0x00;
    const uint8_t ADDR_BYTE3 = 0x40; // Upper part
    uint8_t addrByte4 = 0x00;

    // Determine the parameter offset based on parameter ID
    if (paramID == LFO::lfo1Waveform)
        addrByte4 = 0x10; // LFO1 Waveform offset
    else if (paramID == Oscillator::osc1Waveform)
        addrByte4 = 0x1E; // OSC1 Waveform offset
    else if (paramID == Oscillator::osc2Waveform)
        addrByte4 = 0x21; // OSC2 Waveform offset
    else
        return; // Unknown parameter, don't send

    // Build address and data for checksum calculation
    std::vector<uint8_t> addressAndData = {
        ADDR_BYTE1, ADDR_BYTE2, ADDR_BYTE3, addrByte4,
        static_cast<uint8_t>(waveformValue)
    };

    // Calculate checksum
    uint8_t checksum = calculateRolandChecksum(addressAndData);

    // Build complete SysEx message
    std::vector<uint8_t> sysexMessage = {
        0xF0,           // SysEx start
        ROLAND_ID,      // Roland manufacturer ID
        DEVICE_ID,      // Device ID
        MODEL_ID_MSB,   // Model ID MSB
        MODEL_ID_LSB,   // Model ID LSB (JP-8080)
        DT1_COMMAND,    // DT1 command
        ADDR_BYTE1,     // Address byte 1
        ADDR_BYTE2,     // Address byte 2
        ADDR_BYTE3,     // Address byte 3
        addrByte4,      // Address byte 4
        static_cast<uint8_t>(waveformValue), // Data
        checksum,       // Checksum
        0xF7            // SysEx end
    };

    sendSysExMessage(midiMessages, sysexMessage);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JP8080ControllerAudioProcessor();
}
