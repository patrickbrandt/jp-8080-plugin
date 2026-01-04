#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JP8080ControllerAudioProcessorEditor::JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    using namespace JP8080Parameters;

    // Set larger window size for full control layout
    setSize (900, 650);

    // Apply custom LookAndFeel
    setLookAndFeel(&jp8080LookAndFeel);

    // MIDI Configuration Controls
    midiChannelLabel.setText ("MIDI Channel:", juce::dontSendNotification);
    midiChannelLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (midiChannelLabel);

    midiChannelCombo.addItemList ({"1", "2", "3", "4", "5", "6", "7", "8",
                                    "9", "10", "11", "12", "13", "14", "15", "16"}, 1);
    addAndMakeVisible (midiChannelCombo);
    midiChannelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), MidiConfig::midiChannel, midiChannelCombo);

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
    createRotaryKnob(osc1Control1Knob, osc1Control1Label, Oscillator::osc1Control1, "OSC1 Ctrl1");
    createRotaryKnob(osc1Control2Knob, osc1Control2Label, Oscillator::osc1Control2, "OSC1 Ctrl2");
    createRotaryKnob(osc2RangeKnob, osc2RangeLabel, Oscillator::osc2Range, "OSC2 Range");
    createRotaryKnob(osc2FineKnob, osc2FineLabel, Oscillator::osc2FineWide, "OSC2 Fine");
    createRotaryKnob(oscBalanceKnob, oscBalanceLabel, Oscillator::oscBalance, "Balance");
    createRotaryKnob(xModDepthKnob, xModDepthLabel, Oscillator::xModDepth, "X-Mod");

    // Filter Section
    createRotaryKnob(filterCutoffKnob, filterCutoffLabel, Filter::cutoff, "Cutoff");
    createRotaryKnob(filterResonanceKnob, filterResonanceLabel, Filter::resonance, "Resonance");
    createRotaryKnob(filterKeyFollowKnob, filterKeyFollowLabel, Filter::keyFollow, "Key Follow");
    createRotaryKnob(filterEnvDepthKnob, filterEnvDepthLabel, Filter::envDepth, "Env Depth");

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
    createRotaryKnob(lfo1RateKnob, lfo1RateLabel, LFO::lfo1Rate, "LFO1 Rate");
    createRotaryKnob(lfo1FadeKnob, lfo1FadeLabel, LFO::lfo1Fade, "LFO1 Fade");
    createRotaryKnob(lfo2RateKnob, lfo2RateLabel, LFO::lfo2Rate, "LFO2 Rate");

    // Effects Section
    createRotaryKnob(multiFxLevelKnob, multiFxLevelLabel, Effects::multiFxLevel, "Multi-FX");
    createRotaryKnob(delayTimeKnob, delayTimeLabel, Effects::delayTime, "Dly Time");
    createRotaryKnob(delayFeedbackKnob, delayFeedbackLabel, Effects::delayFeedback, "Dly FB");
    createRotaryKnob(delayLevelKnob, delayLevelLabel, Effects::delayLevel, "Dly Level");
}

JP8080ControllerAudioProcessorEditor::~JP8080ControllerAudioProcessorEditor()
{
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
    g.drawText ("Controller", 120, 30, 100, 20, juce::Justification::centredLeft, true);

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

    // Row 1: OSCILLATOR and FILTER sections
    drawPanel (10, 100, 280, 150, "OSCILLATOR");
    drawPanel (300, 100, 280, 150, "FILTER");
    drawPanel (590, 100, 300, 150, "FILTER ENVELOPE");

    // Row 2: AMPLIFIER and LFO sections
    drawPanel (10, 260, 380, 150, "AMPLIFIER");
    drawPanel (400, 260, 240, 150, "LFO");
    drawPanel (650, 260, 240, 150, "EFFECTS");

    // Info text
    g.setColour (juce::Colour (0xff666666));
    g.setFont (10.0f);
    g.drawText ("Additional parameters available via Logic Pro automation menu",
                10, getHeight() - 25, getWidth() - 20, 15, juce::Justification::centred, true);
    g.drawText ("v0.1.0 - Phase 4B: Custom GUI", getWidth() - 200, getHeight() - 25, 190, 15,
                juce::Justification::centredRight, true);
}

void JP8080ControllerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Header area for MIDI config
    auto headerArea = bounds.removeFromTop (70);
    headerArea.removeFromLeft (250); // Skip title area
    headerArea.reduce (10, 15);

    auto midiRow = headerArea.removeFromTop (20);
    midiChannelLabel.setBounds (midiRow.removeFromLeft (90));
    midiRow.removeFromLeft (5);
    midiChannelCombo.setBounds (midiRow.removeFromLeft (60));
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
        label.setBounds (x, y + 100, 70, 20);
    };

    // Row 1: Oscillator, Filter, Filter Envelope
    // Oscillator section (6 knobs)
    positionKnob (osc1Control1Knob, osc1Control1Label, 20, 100);
    positionKnob (osc1Control2Knob, osc1Control2Label, 100, 100);
    positionKnob (osc2RangeKnob, osc2RangeLabel, 180, 100);
    positionKnob (osc2FineKnob, osc2FineLabel, 20, 180);
    positionKnob (oscBalanceKnob, oscBalanceLabel, 100, 180);
    positionKnob (xModDepthKnob, xModDepthLabel, 180, 180);

    // Filter section (4 knobs)
    positionKnob (filterCutoffKnob, filterCutoffLabel, 310, 100);
    positionKnob (filterResonanceKnob, filterResonanceLabel, 390, 100);
    positionKnob (filterKeyFollowKnob, filterKeyFollowLabel, 470, 100);
    positionKnob (filterEnvDepthKnob, filterEnvDepthLabel, 310, 180);

    // Filter Envelope (4 knobs)
    positionKnob (filterAttackKnob, filterAttackLabel, 600, 100);
    positionKnob (filterDecayKnob, filterDecayLabel, 680, 100);
    positionKnob (filterSustainKnob, filterSustainLabel, 760, 100);
    positionKnob (filterReleaseKnob, filterReleaseLabel, 600, 180);

    // Row 2: Amplifier, LFO, Effects
    // Amplifier section (5 knobs)
    positionKnob (ampLevelKnob, ampLevelLabel, 20, 260);
    positionKnob (ampAttackKnob, ampAttackLabel, 100, 260);
    positionKnob (ampDecayKnob, ampDecayLabel, 180, 260);
    positionKnob (ampSustainKnob, ampSustainLabel, 260, 260);
    positionKnob (ampReleaseKnob, ampReleaseLabel, 100, 340);

    // LFO section (3 knobs)
    positionKnob (lfo1RateKnob, lfo1RateLabel, 410, 260);
    positionKnob (lfo1FadeKnob, lfo1FadeLabel, 490, 260);
    positionKnob (lfo2RateKnob, lfo2RateLabel, 410, 340);

    // Effects section (4 knobs)
    positionKnob (multiFxLevelKnob, multiFxLevelLabel, 660, 260);
    positionKnob (delayTimeKnob, delayTimeLabel, 740, 260);
    positionKnob (delayFeedbackKnob, delayFeedbackLabel, 660, 340);
    positionKnob (delayLevelKnob, delayLevelLabel, 740, 340);
}

//==============================================================================
void JP8080ControllerAudioProcessorEditor::createRotaryKnob(juce::Slider& slider, juce::Label& label,
                                                              const juce::String& paramID, const juce::String& labelText)
{
    // Configure slider as rotary knob
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 16);
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
        else if (paramID == JP8080Parameters::Filter::cutoff)
            filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::resonance)
            filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                audioProcessor.getValueTreeState(), paramID, slider);
        else if (paramID == JP8080Parameters::Filter::keyFollow)
            filterKeyFollowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
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
void JP8080ControllerAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &patchBankCombo)
    {
        updatePatchNamesForCurrentBank();
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
