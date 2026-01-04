#pragma once

#include <JuceHeader.h>

namespace JP8080Parameters
{
    // Parameter ID constants for CC-controllable parameters only
    // Organized by JP-8080 panel sections
    // Based on MODE2 Tx/Rx settings for maximum CC control

    // ========== OSCILLATOR SECTION ==========
    namespace Oscillator
    {
        static const juce::String osc1Control1    = "osc1_control1";    // CC#4
        static const juce::String osc1Control2    = "osc1_control2";    // CC#76
        static const juce::String osc2Range       = "osc2_range";       // CC#21 (MODE2)
        static const juce::String osc2FineWide    = "osc2_fine_wide";   // CC#77
        static const juce::String osc2Control1    = "osc2_control1";    // CC#78
        static const juce::String osc2Control2    = "osc2_control2";    // CC#79
        static const juce::String oscBalance      = "osc_balance";      // CC#8
        static const juce::String xModDepth       = "xmod_depth";       // CC#70
        static const juce::String oscLfo1Depth    = "osc_lfo1_depth";   // CC#18
    }

    // ========== PITCH ENVELOPE SECTION ==========
    namespace PitchEnv
    {
        static const juce::String depth           = "pitch_env_depth";   // CC#25 (MODE2)
        static const juce::String attack          = "pitch_env_attack";  // CC#26 (MODE2)
        static const juce::String decay           = "pitch_env_decay";   // CC#27 (MODE2)
    }

    // ========== FILTER SECTION ==========
    namespace Filter
    {
        static const juce::String cutoff          = "filter_cutoff";     // CC#74
        static const juce::String resonance       = "filter_resonance";  // CC#71
        static const juce::String keyFollow       = "filter_key_follow"; // CC#30 (MODE2)
        static const juce::String lfo1Depth       = "filter_lfo1_depth"; // CC#19
        static const juce::String envDepth        = "filter_env_depth";  // CC#81
        static const juce::String envAttack       = "filter_env_attack"; // CC#82
        static const juce::String envDecay        = "filter_env_decay";  // CC#83
        static const juce::String envSustain      = "filter_env_sustain";// CC#28 (MODE2)
        static const juce::String envRelease      = "filter_env_release";// CC#29 (MODE2)
    }

    // ========== AMPLIFIER SECTION ==========
    namespace Amplifier
    {
        static const juce::String level           = "amp_level";         // CC#7
        static const juce::String lfo1Depth       = "amp_lfo1_depth";    // CC#80
        static const juce::String envAttack       = "amp_env_attack";    // CC#73
        static const juce::String envDecay        = "amp_env_decay";     // CC#75
        static const juce::String envSustain      = "amp_env_sustain";   // CC#31 (MODE2)
        static const juce::String envRelease      = "amp_env_release";   // CC#72
    }

    // ========== LFO SECTION ==========
    namespace LFO
    {
        static const juce::String lfo1Rate        = "lfo1_rate";         // CC#16
        static const juce::String lfo1Fade        = "lfo1_fade";         // CC#20 (MODE2)
        static const juce::String lfo2Rate        = "lfo2_rate";         // CC#17
        static const juce::String lfo2PitchDepth  = "lfo2_pitch_depth";  // CC#22 (MODE2)
        static const juce::String lfo2FilterDepth = "lfo2_filter_depth"; // CC#23 (MODE2)
        static const juce::String lfo2AmpDepth    = "lfo2_amp_depth";    // CC#24 (MODE2)
    }

    // ========== EFFECTS SECTION ==========
    namespace Effects
    {
        static const juce::String toneCtrlBass    = "tone_ctrl_bass";    // CC#92
        static const juce::String toneCtrlTreble  = "tone_ctrl_treble";  // CC#95
        static const juce::String multiFxLevel    = "multi_fx_level";    // CC#93
        static const juce::String delayTime       = "delay_time";        // CC#12
        static const juce::String delayFeedback   = "delay_feedback";    // CC#13
        static const juce::String delayLevel      = "delay_level";       // CC#94
    }

    // ========== CONTROL SECTION ==========
    namespace Control
    {
        static const juce::String portamentoTime  = "portamento_time";   // CC#5
        static const juce::String portamentoSwitch= "portamento_switch"; // CC#65
        static const juce::String hold1           = "hold1";             // CC#64 (Sustain)
        static const juce::String modulation      = "modulation";        // CC#1
        static const juce::String expression      = "expression";        // CC#11
        static const juce::String pan             = "pan";               // CC#10
    }

    // ========== MIDI CONFIGURATION ==========
    namespace MidiConfig
    {
        static const juce::String midiChannel     = "midi_channel";      // MIDI channel (1-16)
        static const juce::String patchBank       = "patch_bank";        // Patch bank selection
        static const juce::String patchProgram    = "patch_program";     // Program number (1-128)
    }

    // ========== PATCH BANK DEFINITIONS ==========
    enum class PatchBank
    {
        UserA = 0,      // CC#0=80, CC#32=0, PC=0-63
        UserB,          // CC#0=80, CC#32=0, PC=64-127
        Preset1A,       // CC#0=81, CC#32=0, PC=0-63
        Preset1B,       // CC#0=81, CC#32=0, PC=64-127
        Preset2A,       // CC#0=81, CC#32=1, PC=0-63
        Preset2B,       // CC#0=81, CC#32=1, PC=64-127
        Preset3A,       // CC#0=81, CC#32=2, PC=0-63
        Preset3B        // CC#0=81, CC#32=2, PC=64-127
    };

    // Bank names for display
    static const juce::StringArray patchBankNames = {
        "User A (11-88)",
        "User B (11-88)",
        "Preset 1 A (11-88)",
        "Preset 1 B (11-88)",
        "Preset 2 A (11-88)",
        "Preset 2 B (11-88)",
        "Preset 3 A (11-88)",
        "Preset 3 B (11-88)"
    };

    // Helper to get Bank Select MSB/LSB and Program offset for a bank
    struct BankSelectInfo
    {
        int bankMSB;        // CC#0
        int bankLSB;        // CC#32
        int programOffset;  // Offset to add to program number
    };

    inline BankSelectInfo getBankSelectInfo(PatchBank bank)
    {
        switch (bank)
        {
            case PatchBank::UserA:     return {80, 0, 0};
            case PatchBank::UserB:     return {80, 0, 64};
            case PatchBank::Preset1A:  return {81, 0, 0};
            case PatchBank::Preset1B:  return {81, 0, 64};
            case PatchBank::Preset2A:  return {81, 1, 0};
            case PatchBank::Preset2B:  return {81, 1, 64};
            case PatchBank::Preset3A:  return {81, 2, 0};
            case PatchBank::Preset3B:  return {81, 2, 64};
            default:                   return {80, 0, 0};
        }
    }

    // ========== CC NUMBER MAPPINGS ==========
    // Map parameter IDs to MIDI CC numbers
    static const std::map<juce::String, int> ccNumbers = {
        // Oscillator
        {Oscillator::osc1Control1,    4},
        {Oscillator::osc1Control2,    76},
        {Oscillator::osc2Range,       21},
        {Oscillator::osc2FineWide,    77},
        {Oscillator::osc2Control1,    78},
        {Oscillator::osc2Control2,    79},
        {Oscillator::oscBalance,      8},
        {Oscillator::xModDepth,       70},
        {Oscillator::oscLfo1Depth,    18},

        // Pitch Envelope
        {PitchEnv::depth,             25},
        {PitchEnv::attack,            26},
        {PitchEnv::decay,             27},

        // Filter
        {Filter::cutoff,              74},
        {Filter::resonance,           71},
        {Filter::keyFollow,           30},
        {Filter::lfo1Depth,           19},
        {Filter::envDepth,            81},
        {Filter::envAttack,           82},
        {Filter::envDecay,            83},
        {Filter::envSustain,          28},
        {Filter::envRelease,          29},

        // Amplifier
        {Amplifier::level,            7},
        {Amplifier::lfo1Depth,        80},
        {Amplifier::envAttack,        73},
        {Amplifier::envDecay,         75},
        {Amplifier::envSustain,       31},
        {Amplifier::envRelease,       72},

        // LFO
        {LFO::lfo1Rate,               16},
        {LFO::lfo1Fade,               20},
        {LFO::lfo2Rate,               17},
        {LFO::lfo2PitchDepth,         22},
        {LFO::lfo2FilterDepth,        23},
        {LFO::lfo2AmpDepth,           24},

        // Effects
        {Effects::toneCtrlBass,       92},
        {Effects::toneCtrlTreble,     95},
        {Effects::multiFxLevel,       93},
        {Effects::delayTime,          12},
        {Effects::delayFeedback,      13},
        {Effects::delayLevel,         94},

        // Control
        {Control::portamentoTime,     5},
        {Control::portamentoSwitch,   65},
        {Control::hold1,              64},
        {Control::modulation,         1},
        {Control::expression,         11},
        {Control::pan,                10}
    };

    // ========== PARAMETER DISPLAY NAMES ==========
    static const std::map<juce::String, juce::String> displayNames = {
        // Oscillator
        {Oscillator::osc1Control1,    "OSC1 Control 1"},
        {Oscillator::osc1Control2,    "OSC1 Control 2"},
        {Oscillator::osc2Range,       "OSC2 Range"},
        {Oscillator::osc2FineWide,    "OSC2 Fine/Wide"},
        {Oscillator::osc2Control1,    "OSC2 Control 1"},
        {Oscillator::osc2Control2,    "OSC2 Control 2"},
        {Oscillator::oscBalance,      "OSC Balance"},
        {Oscillator::xModDepth,       "X-Mod Depth"},
        {Oscillator::oscLfo1Depth,    "OSC LFO1 Depth"},

        // Pitch Envelope
        {PitchEnv::depth,             "Pitch Env Depth"},
        {PitchEnv::attack,            "Pitch Env Attack"},
        {PitchEnv::decay,             "Pitch Env Decay"},

        // Filter
        {Filter::cutoff,              "Filter Cutoff"},
        {Filter::resonance,           "Filter Resonance"},
        {Filter::keyFollow,           "Filter Key Follow"},
        {Filter::lfo1Depth,           "Filter LFO1 Depth"},
        {Filter::envDepth,            "Filter Env Depth"},
        {Filter::envAttack,           "Filter Env Attack"},
        {Filter::envDecay,            "Filter Env Decay"},
        {Filter::envSustain,          "Filter Env Sustain"},
        {Filter::envRelease,          "Filter Env Release"},

        // Amplifier
        {Amplifier::level,            "Amp Level"},
        {Amplifier::lfo1Depth,        "Amp LFO1 Depth"},
        {Amplifier::envAttack,        "Amp Env Attack"},
        {Amplifier::envDecay,         "Amp Env Decay"},
        {Amplifier::envSustain,       "Amp Env Sustain"},
        {Amplifier::envRelease,       "Amp Env Release"},

        // LFO
        {LFO::lfo1Rate,               "LFO1 Rate"},
        {LFO::lfo1Fade,               "LFO1 Fade"},
        {LFO::lfo2Rate,               "LFO2 Rate"},
        {LFO::lfo2PitchDepth,         "LFO2 Pitch Depth"},
        {LFO::lfo2FilterDepth,        "LFO2 Filter Depth"},
        {LFO::lfo2AmpDepth,           "LFO2 Amp Depth"},

        // Effects
        {Effects::toneCtrlBass,       "Tone Control Bass"},
        {Effects::toneCtrlTreble,     "Tone Control Treble"},
        {Effects::multiFxLevel,       "Multi-FX Level"},
        {Effects::delayTime,          "Delay Time"},
        {Effects::delayFeedback,      "Delay Feedback"},
        {Effects::delayLevel,         "Delay Level"},

        // Control
        {Control::portamentoTime,     "Portamento Time"},
        {Control::portamentoSwitch,   "Portamento Switch"},
        {Control::hold1,              "Hold 1 (Sustain)"},
        {Control::modulation,         "Modulation"},
        {Control::expression,         "Expression"},
        {Control::pan,                "Pan"},

        // MIDI Configuration
        {MidiConfig::midiChannel,     "MIDI Channel"},
        {MidiConfig::patchBank,       "Patch Bank"},
        {MidiConfig::patchProgram,    "Patch Program"}
    };

    // Helper function to get CC number for a parameter ID
    inline int getCCNumber(const juce::String& paramID)
    {
        auto it = ccNumbers.find(paramID);
        return (it != ccNumbers.end()) ? it->second : -1;
    }

    // Helper function to get display name for a parameter ID
    inline juce::String getDisplayName(const juce::String& paramID)
    {
        auto it = displayNames.find(paramID);
        return (it != displayNames.end()) ? it->second : paramID;
    }

    // Helper function to get all parameter IDs in organized order
    inline std::vector<juce::String> getAllParameterIDs()
    {
        return {
            // Oscillator Section (9 params)
            Oscillator::osc1Control1, Oscillator::osc1Control2,
            Oscillator::osc2Range, Oscillator::osc2FineWide,
            Oscillator::osc2Control1, Oscillator::osc2Control2,
            Oscillator::oscBalance, Oscillator::xModDepth, Oscillator::oscLfo1Depth,

            // Pitch Envelope Section (3 params)
            PitchEnv::depth, PitchEnv::attack, PitchEnv::decay,

            // Filter Section (9 params)
            Filter::cutoff, Filter::resonance, Filter::keyFollow, Filter::lfo1Depth,
            Filter::envDepth, Filter::envAttack, Filter::envDecay,
            Filter::envSustain, Filter::envRelease,

            // Amplifier Section (6 params)
            Amplifier::level, Amplifier::lfo1Depth,
            Amplifier::envAttack, Amplifier::envDecay,
            Amplifier::envSustain, Amplifier::envRelease,

            // LFO Section (6 params)
            LFO::lfo1Rate, LFO::lfo1Fade, LFO::lfo2Rate,
            LFO::lfo2PitchDepth, LFO::lfo2FilterDepth, LFO::lfo2AmpDepth,

            // Effects Section (6 params)
            Effects::toneCtrlBass, Effects::toneCtrlTreble, Effects::multiFxLevel,
            Effects::delayTime, Effects::delayFeedback, Effects::delayLevel,

            // Control Section (6 params)
            Control::portamentoTime, Control::portamentoSwitch, Control::hold1,
            Control::modulation, Control::expression, Control::pan
        };
    }

    // Helper function to check if parameter is a MIDI configuration (not a CC parameter)
    inline bool isMidiConfigParameter(const juce::String& paramID)
    {
        return paramID == MidiConfig::midiChannel ||
               paramID == MidiConfig::patchBank ||
               paramID == MidiConfig::patchProgram;
    }

    // Total: 45 CC-controllable parameters + 3 MIDI config parameters = 48 total
}
