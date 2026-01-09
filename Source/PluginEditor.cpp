#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JP8080ControllerAudioProcessorEditor::JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    using namespace JP8080Parameters;

    // Set larger window size for full control layout (wider for single-row panels)
    setSize (1200, 610);

    // Apply custom LookAndFeel
    setLookAndFeel(&jp8080LookAndFeel);

    // MIDI Configuration Controls

    // MIDI Output Device Selector (for SysEx)
    midiOutputLabel.setText ("MIDI Output:", juce::dontSendNotification);
    midiOutputLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (midiOutputLabel);

    midiOutputCombo.addListener (this);
    addAndMakeVisible (midiOutputCombo);
    populateMidiOutputList();

    partLabel.setText ("Part:", juce::dontSendNotification);
    partLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (partLabel);

    partCombo.addItemList (partNames, 1);
    addAndMakeVisible (partCombo);
    partAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::part, partCombo);

    patchBankLabel.setText ("Patch:", juce::dontSendNotification);
    patchBankLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (patchBankLabel);

    patchBankCombo.addItemList (patchBankNames, 1);
    patchBankCombo.addListener (this);
    addAndMakeVisible (patchBankCombo);
    patchBankAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::patchBank, patchBankCombo);

    patchNameLabel.setText ("Patch Name:", juce::dontSendNotification);
    patchNameLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (patchNameLabel);

    // Initialize with User A patch names
    patchNameCombo.addItemList (userAPatchNames, 1);
    addAndMakeVisible (patchNameCombo);
    patchNameAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::patchProgram, patchNameCombo);

    // Update patch names for currently selected bank
    updatePatchNamesForCurrentBank();

    // Oscillator Section
    osc1WaveformLabel.setText("Waveform:", juce::dontSendNotification);
    osc1WaveformLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(osc1WaveformLabel);
    osc1WaveformCombo.addItemList(osc1WaveformNames, 1);
    addAndMakeVisible(osc1WaveformCombo);
    osc1WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), Oscillator::osc1Waveform, osc1WaveformCombo);

    createRotaryKnob(osc1Control1Knob, osc1Control1Label, Oscillator::osc1Control1, "Ctrl 1");
    createRotaryKnob(osc1Control2Knob, osc1Control2Label, Oscillator::osc1Control2, "Ctrl 2");

    osc2WaveformLabel.setText("Waveform:", juce::dontSendNotification);
    osc2WaveformLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(osc2WaveformLabel);
    osc2WaveformCombo.addItemList(osc2WaveformNames, 1);
    addAndMakeVisible(osc2WaveformCombo);
    osc2WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), Oscillator::osc2Waveform, osc2WaveformCombo);

    createRotaryKnob(osc2RangeKnob, osc2RangeLabel, Oscillator::osc2Range, "Range");
    createRotaryKnob(osc2FineKnob, osc2FineLabel, Oscillator::osc2FineWide, "Fine");
    createRotaryKnob(osc2Control1Knob, osc2Control1Label, Oscillator::osc2Control1, "Ctrl 1");
    createRotaryKnob(osc2Control2Knob, osc2Control2Label, Oscillator::osc2Control2, "Ctrl 2");
    createRotaryKnob(oscBalanceKnob, oscBalanceLabel, Oscillator::oscBalance, "Balance");
    createRotaryKnob(xModDepthKnob, xModDepthLabel, Oscillator::xModDepth, "X-Mod");
    createRotaryKnob(oscLfo1DepthKnob, oscLfo1DepthLabel, Oscillator::oscLfo1Depth, "LFO Depth");

    // Filter Section
    createRotaryKnob(filterCutoffKnob, filterCutoffLabel, Filter::cutoff, "Cutoff");
    createRotaryKnob(filterResonanceKnob, filterResonanceLabel, Filter::resonance, "Resonance");
    createRotaryKnob(filterKeyFollowKnob, filterKeyFollowLabel, Filter::keyFollow, "Key Follow");
    createRotaryKnob(filterLfo1DepthKnob, filterLfo1DepthLabel, Filter::lfo1Depth, "LFO Depth");
    createRotaryKnob(filterEnvDepthKnob, filterEnvDepthLabel, Filter::envDepth, "Depth");

    // Filter Envelope
    createRotaryKnob(filterAttackKnob, filterAttackLabel, Filter::envAttack, "Attack");
    createRotaryKnob(filterDecayKnob, filterDecayLabel, Filter::envDecay, "Decay");
    createRotaryKnob(filterSustainKnob, filterSustainLabel, Filter::envSustain, "Sustain");
    createRotaryKnob(filterReleaseKnob, filterReleaseLabel, Filter::envRelease, "Release");

    // Amplifier Section
    createRotaryKnob(ampLevelKnob, ampLevelLabel, Amplifier::level, "Level");
    createRotaryKnob(ampAttackKnob, ampAttackLabel, Amplifier::envAttack, "Attack");
    createRotaryKnob(ampDecayKnob, ampDecayLabel, Amplifier::envDecay, "Decay");
    createRotaryKnob(ampSustainKnob, ampSustainLabel, Amplifier::envSustain, "Sustain");
    createRotaryKnob(ampReleaseKnob, ampReleaseLabel, Amplifier::envRelease, "Release");

    // LFO Section
    lfo1WaveformLabel.setText("Waveform:", juce::dontSendNotification);
    lfo1WaveformLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(lfo1WaveformLabel);
    lfo1WaveformCombo.addItemList(lfo1WaveformNames, 1);
    addAndMakeVisible(lfo1WaveformCombo);
    lfo1WaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), LFO::lfo1Waveform, lfo1WaveformCombo);

    createRotaryKnob(lfo1RateKnob, lfo1RateLabel, LFO::lfo1Rate, "Rate");
    createRotaryKnob(lfo1FadeKnob, lfo1FadeLabel, LFO::lfo1Fade, "Fade");
    createRotaryKnob(lfo2RateKnob, lfo2RateLabel, LFO::lfo2Rate, "Rate");
    createRotaryKnob(lfo2DepthKnob, lfo2DepthLabel, LFO::lfo2PitchDepth, "Depth");

    // Pitch Envelope Section
    createRotaryKnob(pitchEnvDepthKnob, pitchEnvDepthLabel, PitchEnv::depth, "Depth");
    createRotaryKnob(pitchEnvAttackKnob, pitchEnvAttackLabel, PitchEnv::attack, "Attack");
    createRotaryKnob(pitchEnvDecayKnob, pitchEnvDecayLabel, PitchEnv::decay, "Decay");

    // Effects Section
    createRotaryKnob(toneCtrlBassKnob, toneCtrlBassLabel, Effects::toneCtrlBass, "Bass");
    createRotaryKnob(toneCtrlTrebleKnob, toneCtrlTrebleLabel, Effects::toneCtrlTreble, "Treble");

    multiFxTypeLabel.setText("Multi-FX:", juce::dontSendNotification);
    multiFxTypeLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(multiFxTypeLabel);
    multiFxTypeCombo.addItemList(multiFxTypeNames, 1);
    addAndMakeVisible(multiFxTypeCombo);
    multiFxTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), Effects::multiFxType, multiFxTypeCombo);

    createRotaryKnob(multiFxLevelKnob, multiFxLevelLabel, Effects::multiFxLevel, "FX Level");

    delayTypeLabel.setText("Delay:", juce::dontSendNotification);
    delayTypeLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(delayTypeLabel);
    delayTypeCombo.addItemList(delayTypeNames, 1);
    addAndMakeVisible(delayTypeCombo);
    delayTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), Effects::delayType, delayTypeCombo);

    createRotaryKnob(delayTimeKnob, delayTimeLabel, Effects::delayTime, "Delay Time");
    createRotaryKnob(delayFeedbackKnob, delayFeedbackLabel, Effects::delayFeedback, "Delay Feedback");
    createRotaryKnob(delayLevelKnob, delayLevelLabel, Effects::delayLevel, "Delay Level");
}

JP8080ControllerAudioProcessorEditor::~JP8080ControllerAudioProcessorEditor()
{
    midiOutputCombo.removeListener(this);
    patchBankCombo.removeListener(this);
    setLookAndFeel(nullptr);
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    g.fillAll (juce::Colour (0xff1a1a1a));

    // Header background
    g.setColour (juce::Colour (0xff0a0a0a));
    g.fillRect (0, 0, getWidth(), 70);

    // Title
    g.setColour (juce::Colour (0xffff6600));
    g.setFont (juce::Font (32.0f, juce::Font::bold));
    g.drawText ("JP-8080", 20, 10, 200, 50, juce::Justification::centredLeft, true);

    g.setColour (juce::Colours::lightgrey);
    g.setFont (juce::Font (14.0f));
    g.drawText ("Controller", 32, 43, 100, 20, juce::Justification::centredLeft, true);

    // Draw section panels
    auto drawPanel = [&](int x, int y, int w, int h, const juce::String& title)
    {
        // Panel background
        g.setColour (juce::Colour (0xff252525));
        g.fillRoundedRectangle (x, y, w, h, 6.0f);

        // Panel border
        g.setColour (juce::Colour (0xff404040));
        g.drawRoundedRectangle (x, y, w, h, 6.0f, 1.0f);

        // Section title
        g.setColour (juce::Colour (0xffff6600));
        g.setFont (juce::Font (13.0f, juce::Font::bold));
        g.drawText (title, x + 10, y + 5, w - 20, 20, juce::Justification::centredLeft, true);

        // Title underline
        g.setColour (juce::Colour (0xff404040));
        g.fillRect (x + 10, y + 25, w - 20, 1);
    };

    // Row 1: OSCILLATOR sections
    drawPanel (10, 100, 190, 160, "OSCILLATOR 1");
    drawPanel (210, 100, 340, 160, "OSCILLATOR 2");
    drawPanel (560, 100, 330, 160, "OSC COMMON");

    // Row 2: FILTER and ENVELOPE sections (single row layout)
    drawPanel (10, 280, 330, 140, "FILTER");
    drawPanel (350, 280, 410, 140, "FILTER ENVELOPE");
    drawPanel (770, 280, 420, 140, "AMP ENVELOPE");

    // Row 3: LFO, PITCH ENV, and EFFECTS sections (single row layout)
    drawPanel (10, 440, 180, 160, "LFO 1");
    drawPanel (200, 440, 180, 160, "LFO 2");
    drawPanel (390, 440, 240, 160, "PITCH ENV");
    drawPanel (640, 440, 550, 160, "EFFECTS");
}

void JP8080ControllerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Header area for MIDI config
    auto headerArea = bounds.removeFromTop (70);
    headerArea.removeFromLeft (250); // Skip title area
    headerArea.reduce (10, 10);

    // First row: MIDI Output selector
    auto midiOutputRow = headerArea.removeFromTop (20);
    midiOutputLabel.setBounds (midiOutputRow.removeFromLeft (85));
    midiOutputRow.removeFromLeft (5);
    midiOutputCombo.setBounds (midiOutputRow.removeFromLeft (250));

    headerArea.removeFromTop (5); // Spacing between rows

    // Second row: Part, Bank, Patch (MIDI channel derived from Part: Upper=Ch1, Lower=Ch2)
    auto midiRow = headerArea.removeFromTop (20);
    partLabel.setBounds (midiRow.removeFromLeft (35));
    midiRow.removeFromLeft (5);
    partCombo.setBounds (midiRow.removeFromLeft (70));
    midiRow.removeFromLeft (15);
    patchBankLabel.setBounds (midiRow.removeFromLeft (50));
    midiRow.removeFromLeft (5);
    patchBankCombo.setBounds (midiRow.removeFromLeft (100));
    midiRow.removeFromLeft (15);
    patchNameLabel.setBounds (midiRow.removeFromLeft (80));
    midiRow.removeFromLeft (5);
    patchNameCombo.setBounds (midiRow.removeFromLeft (180));

    // Helper lambda to position a knob
    auto positionKnob = [](juce::Slider& knob, juce::Label& label, int x, int y)
    {
        knob.setBounds (x, y + 30, 70, 70);
        label.setBounds (x, y + 105, 70, 16);
    };

    // Row 1: Oscillator sections
    // OSCILLATOR 1 knobs and waveform selector
    positionKnob (osc1Control1Knob, osc1Control1Label, 20, 100);
    positionKnob (osc1Control2Knob, osc1Control2Label, 100, 100);
    osc1WaveformLabel.setBounds (20, 228, 70, 20);
    osc1WaveformCombo.setBounds (95, 228, 90, 20);

    // OSCILLATOR 2 knobs and waveform selector
    positionKnob (osc2RangeKnob, osc2RangeLabel, 220, 100);
    positionKnob (osc2FineKnob, osc2FineLabel, 300, 100);
    positionKnob (osc2Control1Knob, osc2Control1Label, 380, 100);
    positionKnob (osc2Control2Knob, osc2Control2Label, 460, 100);
    osc2WaveformLabel.setBounds (220, 228, 70, 20);
    osc2WaveformCombo.setBounds (295, 228, 90, 20);

    // OSC COMMON (3 knobs)
    positionKnob (oscBalanceKnob, oscBalanceLabel, 570, 100);
    positionKnob (xModDepthKnob, xModDepthLabel, 650, 100);
    positionKnob (oscLfo1DepthKnob, oscLfo1DepthLabel, 730, 100);

    // Row 2: Filter and Envelope sections (single row)
    // FILTER (4 knobs in one row)
    positionKnob (filterCutoffKnob, filterCutoffLabel, 20, 280);
    positionKnob (filterResonanceKnob, filterResonanceLabel, 100, 280);
    positionKnob (filterKeyFollowKnob, filterKeyFollowLabel, 180, 280);
    positionKnob (filterLfo1DepthKnob, filterLfo1DepthLabel, 260, 280);

    // FILTER ENVELOPE (5 knobs in one row)
    positionKnob (filterEnvDepthKnob, filterEnvDepthLabel, 360, 280);
    positionKnob (filterAttackKnob, filterAttackLabel, 440, 280);
    positionKnob (filterDecayKnob, filterDecayLabel, 520, 280);
    positionKnob (filterSustainKnob, filterSustainLabel, 600, 280);
    positionKnob (filterReleaseKnob, filterReleaseLabel, 680, 280);

    // AMP ENVELOPE (5 knobs in one row)
    positionKnob (ampLevelKnob, ampLevelLabel, 780, 280);
    positionKnob (ampAttackKnob, ampAttackLabel, 860, 280);
    positionKnob (ampDecayKnob, ampDecayLabel, 940, 280);
    positionKnob (ampSustainKnob, ampSustainLabel, 1020, 280);
    positionKnob (ampReleaseKnob, ampReleaseLabel, 1100, 280);

    // Row 3: LFO, Pitch Env, and Effects sections (single row)
    // LFO 1 knobs and waveform selector
    positionKnob (lfo1RateKnob, lfo1RateLabel, 20, 440);
    positionKnob (lfo1FadeKnob, lfo1FadeLabel, 100, 440);
    lfo1WaveformLabel.setBounds (20, 568, 70, 20);
    lfo1WaveformCombo.setBounds (95, 568, 80, 20);

    // LFO 2 (2 knobs in one row)
    positionKnob (lfo2RateKnob, lfo2RateLabel, 210, 440);
    positionKnob (lfo2DepthKnob, lfo2DepthLabel, 290, 440);

    // PITCH ENV (3 knobs in one row)
    positionKnob (pitchEnvDepthKnob, pitchEnvDepthLabel, 400, 440);
    positionKnob (pitchEnvAttackKnob, pitchEnvAttackLabel, 480, 440);
    positionKnob (pitchEnvDecayKnob, pitchEnvDecayLabel, 560, 440);

    // EFFECTS (6 knobs in one row + 2 type selectors)
    positionKnob (toneCtrlBassKnob, toneCtrlBassLabel, 650, 440);
    positionKnob (toneCtrlTrebleKnob, toneCtrlTrebleLabel, 730, 440);
    multiFxTypeLabel.setBounds (650, 568, 70, 20);
    multiFxTypeCombo.setBounds (725, 568, 165, 20);
    positionKnob (multiFxLevelKnob, multiFxLevelLabel, 810, 440);
    delayTypeLabel.setBounds (890, 568, 50, 20);
    delayTypeCombo.setBounds (945, 568, 155, 20);
    positionKnob (delayTimeKnob, delayTimeLabel, 890, 440);
    positionKnob (delayFeedbackKnob, delayFeedbackLabel, 970, 440);
    positionKnob (delayLevelKnob, delayLevelLabel, 1050, 440);
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::createRotaryKnob(juce::Slider& slider, juce::Label& label,
                                                              const juce::String& paramID, const juce::String& labelText)
{
    // Configure slider as rotary knob
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange (0, 127, 1);
    slider.setLookAndFeel (&jp8080LookAndFeel);
    addAndMakeVisible (slider);

    // Create APVTS attachment
    auto* param = audioProcessor.getValueTreeState().getParameter(paramID);
    if (param != nullptr)
    {
        if (paramID == JP8080Parameters::Oscillator::osc1Control1)
            osc1Control1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::osc1Control2)
            osc1Control2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::osc2Range)
            osc2RangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::osc2FineWide)
            osc2FineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::oscBalance)
            oscBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::xModDepth)
            xModDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::osc2Control1)
            osc2Control1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::osc2Control2)
            osc2Control2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Oscillator::oscLfo1Depth)
            oscLfo1DepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::cutoff)
            filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::resonance)
            filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::keyFollow)
            filterKeyFollowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::lfo1Depth)
            filterLfo1DepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::envDepth)
            filterEnvDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::envAttack)
            filterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::envDecay)
            filterDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::envSustain)
            filterSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::envRelease)
            filterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Amplifier::level)
            ampLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Amplifier::envAttack)
            ampAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Amplifier::envDecay)
            ampDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Amplifier::envSustain)
            ampSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Amplifier::envRelease)
            ampReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::LFO::lfo1Rate)
            lfo1RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::LFO::lfo1Fade)
            lfo1FadeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::LFO::lfo2Rate)
            lfo2RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::LFO::lfo2PitchDepth)
            lfo2DepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::PitchEnv::depth)
            pitchEnvDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::PitchEnv::attack)
            pitchEnvAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::PitchEnv::decay)
            pitchEnvDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::toneCtrlBass)
            toneCtrlBassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::toneCtrlTreble)
            toneCtrlTrebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::multiFxLevel)
            multiFxLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::delayTime)
            delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::delayFeedback)
            delayFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Effects::delayLevel)
            delayLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
    }

    // Configure label
    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (11.0f));
    addAndMakeVisible (label);
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::populateMidiOutputList()
{
    midiOutputCombo.clear(juce::dontSendNotification);

    // Add "None" option
    midiOutputCombo.addItem("-- Select MIDI Output --", 1);

    // Get available MIDI outputs
    auto midiOutputs = audioProcessor.getAvailableMidiOutputs();

    int itemId = 2;
    for (const auto& device : midiOutputs)
    {
        midiOutputCombo.addItem(device.name, itemId);
        itemId++;
    }

    // Select currently selected device
    juce::String currentId = audioProcessor.getSelectedMidiOutputId();
    if (currentId.isEmpty())
    {
        midiOutputCombo.setSelectedId(1, juce::dontSendNotification); // "None" selected
    }
    else
    {
        // Find the device in the list
        int index = 2;
        for (const auto& device : midiOutputs)
        {
            if (device.identifier == currentId)
            {
                midiOutputCombo.setSelectedId(index, juce::dontSendNotification);
                break;
            }
            index++;
        }
    }
}

void JP8080ControllerAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &patchBankCombo)
    {
        updatePatchNamesForCurrentBank();
    }
    else if (comboBox == &midiOutputCombo)
    {
        int selectedId = midiOutputCombo.getSelectedId();

        if (selectedId <= 1)
        {
            // "None" selected - clear the output
            audioProcessor.setSelectedMidiOutput("");
        }
        else
        {
            // Find the device by index
            auto midiOutputs = audioProcessor.getAvailableMidiOutputs();
            int index = selectedId - 2; // Offset for "None" item

            if (index >= 0 && index < midiOutputs.size())
            {
                audioProcessor.setSelectedMidiOutput(midiOutputs[index].identifier);
            }
        }
    }
}

void JP8080ControllerAudioProcessorEditor::updatePatchNamesForCurrentBank()
{
    using namespace JP8080Parameters;

    // Get current bank selection
    int bankIndex = patchBankCombo.getSelectedItemIndex();
    if (bankIndex < 0 || bankIndex >= 8)
        return;

    PatchBank bank = static_cast<PatchBank>(bankIndex);

    // Get current program selection before we update the list
    int currentProgram = patchNameCombo.getSelectedItemIndex();

    // Clear and repopulate the patch name ComboBox
    patchNameCombo.clear(juce::dontSendNotification);
    patchNameCombo.addItemList(getPatchNamesForBank(bank), 1);

    // Restore the program selection (or select first if invalid)
    if (currentProgram >= 0 && currentProgram < 64)
        patchNameCombo.setSelectedItemIndex(currentProgram, juce::dontSendNotification);
    else
        patchNameCombo.setSelectedItemIndex(0, juce::dontSendNotification);
}
