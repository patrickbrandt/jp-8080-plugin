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
        static const juce::String osc1Waveform    = "osc1_waveform";    // Waveform selector (SysEx)
        static const juce::String osc1Control1    = "osc1_control1";    // CC#4
        static const juce::String osc1Control2    = "osc1_control2";    // CC#76
        static const juce::String osc2Waveform    = "osc2_waveform";    // Waveform selector (SysEx)
        static const juce::String osc2Range       = "osc2_range";       // CC#21 (MODE2)
        static const juce::String osc2FineWide    = "osc2_fine_wide";   // CC#77
        static const juce::String osc2Control1    = "osc2_control1";    // CC#78
        static const juce::String osc2Control2    = "osc2_control2";    // CC#79
        static const juce::String oscBalance      = "osc_balance";      // CC#8
        static const juce::String xModDepth       = "xmod_depth";       // CC#70
        static const juce::String oscLfo1Depth    = "osc_lfo1_depth";   // CC#18
    }

    // Oscillator waveform options
    static const juce::StringArray osc1WaveformNames = {
        "SUPER SAW", "TRIANGLE MOD", "NOISE", "FEEDBACK OSC", "SQR (PWM)", "SAW", "TRI"
    };

    static const juce::StringArray osc2WaveformNames = {
        "SQR (PWM)", "SAW", "TRI", "NOISE"
    };

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
        static const juce::String lfo1Waveform    = "lfo1_waveform";     // Waveform selector (SysEx)
        static const juce::String lfo1Rate        = "lfo1_rate";         // CC#16
        static const juce::String lfo1Fade        = "lfo1_fade";         // CC#20 (MODE2)
        static const juce::String lfo2Rate        = "lfo2_rate";         // CC#17
        static const juce::String lfo2PitchDepth  = "lfo2_pitch_depth";  // CC#22 (MODE2)
        static const juce::String lfo2FilterDepth = "lfo2_filter_depth"; // CC#23 (MODE2)
        static const juce::String lfo2AmpDepth    = "lfo2_amp_depth";    // CC#24 (MODE2)
    }

    // LFO waveform options
    static const juce::StringArray lfo1WaveformNames = {
        "TRI", "SAW", "SQR", "S/H"
    };

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
        "User A",
        "User B",
        "Preset 1 A",
        "Preset 1 B",
        "Preset 2 A",
        "Preset 2 B",
        "Preset 3 A",
        "Preset 3 B"
    };

    // ========== PATCH NAME DEFINITIONS ==========
    // User A/B banks initially contain same presets as Preset2 A/B
    static const juce::StringArray userAPatchNames = {
        "A11 MG Bass", "A12 Trance Bass 4", "A13 Trance Bass 5", "A14 Trance Bass 6",
        "A15 Bone Sa Mo", "A16 Bone Yall", "A17 PHM 1", "A18 PHM 2",
        "A21 PHM 3", "A22 Static Bass 1", "A23 Static Bass 2", "A24 M Bass",
        "A25 PHM 4", "A26 PHM 5", "A27 Mini Bass", "A28 Wonderland Bass",
        "A31 Hard Bass", "A32 Fretless Synth", "A33 Lead Bass", "A34 JP Fat Synbrass",
        "A35 Gate me!", "A36 Kling Klang 2", "A37 Rising Key", "A38 Flat Out 1",
        "A41 Flat LFO", "A42 Flat Out 2", "A43 Flat Out 3", "A44 MiniSynth 1",
        "A45 AW/DM Resonance1", "A46 AW/DM Resonance2", "A47 AW/DM Resonance3", "A48 AW/DM Resonance4",
        "A51 WONDERLAND 1", "A52 WONDERLAND 2", "A53 Jupiter8Arpeggio", "A54 Fuel",
        "A55 Shake", "A56 Model", "A57 Vanishing Key", "A58 Fade Away",
        "A61 Hard Key 1", "A62 Hard Key 2", "A63 Hard Key 3", "A64 Cheesy Key 1",
        "A65 Cheesy Key 2", "A66 DM 1", "A67 DM 2", "A68 Hard Key 4",
        "A71 Hard Key 5", "A72 Hard Key 6", "A73 DM 3", "A74 Arpy 3",
        "A75 Arpy 4", "A76 J Echo", "A77 Mini Seq. 1", "A78 Mini Seq. 2",
        "A81 Pulsar 88", "A82 Kling Klang 3", "A83 Straight Jacket", "A84 DM 4",
        "A85 CHEM 1", "A86 CHEM 2", "A87 Dusseldorf 1", "A88 Dusseldorf 2"
    };

    static const juce::StringArray userBPatchNames = {
        "B11 Virtual Voltage", "B12 DAD or alive", "B13 Freeze Frame", "B14 Rave2theRhythm",
        "B15 Cheesy Lead 1", "B16 Cheesy Lead 2", "B17 Cheesy Lead 3", "B18 Cheesy Lead 4",
        "B21 Cheesy Lead 5", "B22 Coreline Nine", "B23 Wonderland Pad 1", "B24 Wonderland Pad 2",
        "B25 Wonderland Pad 3", "B26 seqaT nortoleM", "B27 Venusian Strings", "B28 Wonderland Pad 4",
        "B31 GRAMMAPHONE 1", "B32 GRAMMAPHONE 2", "B33 GRAMMAPHONE 3", "B34 Oil Canvas 1",
        "B35 Oil Canvas 2", "B36 Oil Canvas 3", "B37 Oil Canvas 4", "B38 Oil Canvas 5",
        "B41 Blossoms 1", "B42 Blossoms 2", "B43 Broom", "B44 J Pad",
        "B45 Dream Kate", "B46 Temple 1", "B47 Temple 2", "B48 Thick",
        "B51 Open 54", "B52 Kling Klang 4", "B53 Epic", "B54 Multiples 1",
        "B55 Lodelity", "B56 Lode in Stereo", "B57 Wonderland Brs", "B58 Hard Pad",
        "B61 Gate in Stereo 1", "B62 Gate in Stereo 2", "B63 Gate in Stereo 3", "B64 S/H in Stereo",
        "B65 Bub", "B66 Simple E.Drums", "B67 Boom your Woofer", "B68 Multiples 2",
        "B71 Midnight 1-900", "B72 100% After", "B73 Time and Space", "B74 LFO 1",
        "B75 LFO 2", "B76 HLAH", "B77 Blade", "B78 Cyborg",
        "B81 Fall", "B82 Rise", "B83 Radioactive 1", "B84 Radioactive 2",
        "B85 DroneOn", "B86 Duss", "B87 Hydro Noise", "B88 From Space..."
    };

    static const juce::StringArray preset1APatchNames = {
        "A11 Spit'n Slide Bs", "A12 Velo Decay Bass", "A13 Wall Bob", "A14 Juno Sub Bass",
        "A15 Subsonic Bass", "A16 Big & Dark", "A17 Bass Flow", "A18 Juno Bass Vel",
        "A21 Dubb Bass", "A22 Juice Bass", "A23 Dreams Are Made", "A24 Reso Bass Line",
        "A25 Bass Pedals", "A26 Hard Core Bass", "A27 MC-202 Bass", "A28 Rubber SH-2",
        "A31 Raging Bass", "A32 Blipper Bass", "A33 JP-303", "A34 Rave Time",
        "A35 Fretless Bass", "A36 Digi Strat", "A37 Fire Wire", "A38 Proflike Clavit",
        "A41 Withmod Comp", "A42 Juno Clav", "A43 Gritty Power", "A44 Separate ways",
        "A45 For RPS", "A46 Bread'n Butter", "A47 Silk 5ths", "A48 Ancient Asia",
        "A51 Intervalic", "A52 Squared Away", "A53 Velo Syncoid", "A54 Resonance Chord",
        "A55 Resorelease", "A56 Waspy Synth", "A57 Euro SAW", "A58 Dance Sweep",
        "A61 Trance Food", "A62 One Shot Reso", "A63 The Fat Guy", "A64 Spit Brass",
        "A65 Poly Sync", "A66 Rave 5th", "A67 UK Shorty", "A68 Old Rhodes",
        "A71 Wurly Piano 1", "A72 Wurly Piano 2", "A73 Moody Organ", "A74 Org/Rotary>Ribon",
        "A75 VK09 PercEchoes", "A76 Sine Lead", "A77 Wichita Lead", "A78 Creamy",
        "A81 Smoothy", "A82 Soaring Mini", "A83 Ribn F/B Lead", "A84 Sup-Jup Lead",
        "A85 Modular Lead", "A86 Syncrosolo", "A87 Ripper", "A88 Phantom Lead"
    };

    static const juce::StringArray preset1BPatchNames = {
        "B11 Whammy Mammy", "B12 Wicked Lead", "B13 Drefull Dr.", "B14 Wiggle Mod",
        "B15 Feedback Lead", "B16 Crunch", "B17 Chaos Lead", "B18 Out of Control",
        "B21 String Machine", "B22 Tron Vlns", "B23 Luxury Symph", "B24 Debussy",
        "B25 BPF Velo Strings", "B26 Detuned Str.", "B27 Juno B81 Pad", "B28 Richland",
        "B31 MOD Strings", "B32 Jupiter Pad", "B33 Soft Strings", "B34 Shan-gri-la",
        "B35 Fine Wine", "B36 Glue Pad", "B37 True Pad", "B38 Foreboding",
        "B41 Skreachea", "B42 BPM Pulse 1", "B43 BPM Pulse 2", "B44 Hi-Pass Puls",
        "B45 Sample&Hold Me", "B46 MKS80 Space", "B47 Arctic Sweep", "B48 Replicant CS",
        "B51 Stargate", "B52 Lost in Time", "B53 Circular", "B54 Space Choir",
        "B55 Hypass Sweep", "B56 BPF Tides", "B57 Matrix Sweep", "B58 MKS80 Bells",
        "B61 Tiny bells", "B62 Chimey", "B63 Juno Arp", "B64 Sonar Ping",
        "B65 Air Harp", "B66 Velo FX Percs", "B67 Quizzled", "B68 Intermittent",
        "B71 Brain Static", "B72 Computone", "B73 Pin Matrix", "B74 Space Cheese",
        "B75 Rough Day", "B76 The Etruscan.", "B77 Varese", "B78 Pipe Dream",
        "B81 Meteor", "B82 Snowman", "B83 Space Ghost", "B84 Ozone",
        "B85 Cool-a little...", "B86 Electro Gulls", "B87 Template 1", "B88 Template 2"
    };

    static const juce::StringArray preset2APatchNames = userAPatchNames;
    static const juce::StringArray preset2BPatchNames = userBPatchNames;

    static const juce::StringArray preset3APatchNames = {
        "A11 Culture Bass", "A12 Techno Brie", "A13 Wired Funk", "A14 Deep Thought",
        "A15 Trance Bass", "A16 Baroque Bass", "A17 Pulse 303", "A18 101 Sub Bass",
        "A21 Serious Low Ant1", "A22 Serious Low Ant2", "A23 Bone", "A24 JX Dyna Bass",
        "A25 Xa Bass", "A26 Offbeat Bass", "A27 Drone Bass", "A28 Clean Wow Bass",
        "A31 FM Solid", "A32 FM Tube Bass", "A33 FM Rave Bass", "A34 Velo Organ",
        "A35 Club Organ", "A36 Old Organ", "A37 PercussivToyPno", "A38 Noise Toys",
        "A41 Apostle Piano", "A42 Clavi-Club", "A43 Perc Clavsynth", "A44 Cyber Cellopluck",
        "A45 Pulse Key", "A46 Nova Catch", "A47 Eurodance Perc 1", "A48 Tribal Party",
        "A51 Viking", "A52 Nova Pad", "A53 HPF Saws", "A54 5th Saws Key",
        "A55 Eros Synth", "A56 Mov'Mov Synth !", "A57 Formula Stack 1", "A58 Formula Stack 2",
        "A61 Raveline", "A62 Ravers Delite", "A63 Super Saw Soup", "A64 Chainsawmassacre",
        "A65 Daft Five", "A66 Coming up", "A67 Power of 80's", "A68 Jericho Horns",
        "A71 Milling Lead", "A72 Dark Loonie", "A73 X-Mod May-Day !", "A74 Dirty Mania",
        "A75 Vinyl Story", "A76 Zipper Hymn", "A77 Nova Attack", "A78 Super Attack",
        "A81 Beep 8000", "A82 Optic Perc", "A83 8008-Cow Signal", "A84 X-FM Metallic",
        "A85 Pluck & Pray", "A86 Bermuda Triangle", "A87 Home of the Rave", "A88 Paris spirit"
    };

    static const juce::StringArray preset3BPatchNames = {
        "B11 Eurodance Perc 2", "B12 Lo-Fi Chops", "B13 Tranceients", "B14 Voicetransformer",
        "B15 AW/DM", "B16 Braindead", "B17 I get a Kick", "B18 Upside down",
        "B21 Hoppy Lead", "B22 Magic Ribbon", "B23 Nice Lead", "B24 Solo Sine&Square",
        "B25 Vintage Voltage", "B26 Trusty Lead", "B27 Dream P5", "B28 Eastern Lead 1",
        "B31 Eastern Lead 2", "B32 Tri&Saw Lead", "B33 Crystal Noise", "B34 Happy Euro Lead",
        "B35 Alphabet Lead", "B36 Feedbacky", "B37 Trance Lead", "B38 CheeseOscillator",
        "B41 Prod Lead", "B42 Dirty Electrons", "B43 Kitch Vinylead", "B44 Killerbeez",
        "B45 *¥ Ethnomad !", "B46 P5 Sync", "B47 Ergot Rye Seed", "B48 FB 5th",
        "B51 70's Mono", "B52 Mega HPF Lead", "B53 Siren's Song", "B54 Retro Strings",
        "B55 Ambient Pad", "B56 Mystery Room", "B57 ElectronicHarmon", "B58 Jungle Pad",
        "B61 Filtersweep 1", "B62 Filtersweep 2", "B63 Sizzler", "B64 Hi-Pass Saws",
        "B65 Piping Pad", "B66 Odyssee Astral", "B67 Agitation", "B68 Safari LFO",
        "B71 Tricky LFO", "B72 Extra Hi-Fi", "B73 Rhythmic Synth", "B74 Asteroid Mode",
        "B75 Disaster 1", "B76 Fuzzy Logic", "B77 QZ Sub Naut", "B78 Searing",
        "B81 Disaster 2", "B82 Scrapers", "B83 Trip in Stereo", "B84 CHEM",
        "B85 Xform", "B86 Amuck", "B87 Cat Conversation", "B88 Pulsing Sweep"
    };

    // Helper function to get patch names for a specific bank
    inline const juce::StringArray& getPatchNamesForBank(PatchBank bank)
    {
        switch (bank)
        {
            case PatchBank::UserA:     return userAPatchNames;
            case PatchBank::UserB:     return userBPatchNames;
            case PatchBank::Preset1A:  return preset1APatchNames;
            case PatchBank::Preset1B:  return preset1BPatchNames;
            case PatchBank::Preset2A:  return preset2APatchNames;
            case PatchBank::Preset2B:  return preset2BPatchNames;
            case PatchBank::Preset3A:  return preset3APatchNames;
            case PatchBank::Preset3B:  return preset3BPatchNames;
            default:                   return userAPatchNames;
        }
    }

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
        {Oscillator::osc1Waveform,    "OSC1 Waveform"},
        {Oscillator::osc1Control1,    "OSC1 Control 1"},
        {Oscillator::osc1Control2,    "OSC1 Control 2"},
        {Oscillator::osc2Waveform,    "OSC2 Waveform"},
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
        {LFO::lfo1Waveform,           "LFO1 Waveform"},
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
