# JP-8080 Logic Pro Controller Plugin - Implementation Plan

## Project Overview

Create an Audio Unit (AU) plugin for Logic Pro that provides a graphical interface to control the Roland JP-8080 hardware synthesizer via MIDI CC messages.

---

## 1. Technology Stack Selection

### Recommended: JUCE Framework
- **Why JUCE**: Cross-platform C++ framework with excellent Audio Unit support, comprehensive documentation, and active community
- **Plugin Format**: Audio Unit (AU) for Logic Pro compatibility
- **Language**: C++ for audio/MIDI processing, with optional GUI frameworks

### Alternative: Native Apple AUv3
- Uses Apple's Audio Unit v3 API directly
- Can use Swift for UI, Objective-C++/C++ for audio processing
- More complex setup but tighter macOS integration

### Development Tools
- Xcode (required for macOS/AU development)
- JUCE Projucer (if using JUCE)
- Logic Pro for testing
- MIDI Monitor for debugging

---

## 2. JP-8080 MIDI CC Mappings

Based on the JP-8080 MIDI Implementation (Transmit/Receive Setting List), the following Control Change messages control the synthesizer parameters. The JP-8080 supports two modes (MODE1 and MODE2) for parameter control.

### Effects Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| Tone Control Bass | CC#92 | TREMOLO |
| Tone Control Treble | CC#95 | PHASER |
| Multi-FX Level | CC#93 | CHORUS |
| Delay Time | CC#12 | EFFECT-CTL1 |
| Delay Feedback | CC#13 | EFFECT-CTL2 |
| Delay Level | CC#94 | CELESTE |

### LFO Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| LFO 1 Rate | CC#16 | GENERAL1 |
| LFO 1 Fade | CC#20 | MODE2 only |
| LFO 2 Rate | CC#17 | GENERAL2 |
| Pitch LFO 2 Depth | CC#22 | MODE2 only |
| Filter LFO 2 Depth | CC#23 | MODE2 only |
| Amp LFO 2 Depth | CC#24 | MODE2 only |

### Oscillator Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| OSC 1 Control 1 | CC#4 | FOOT-TYPE |
| OSC 1 Control 2 | CC#76 | SOUND-CTL7 |
| OSC 2 Range | CC#21 | MODE2 only |
| OSC 2 Fine/Wide | CC#77 | SOUND-CTL8 |
| OSC 2 Control 1 | CC#78 | SOUND-CTL9 |
| OSC 2 Control 2 | CC#79 | SOUND-CTL10 |
| OSC Balance | CC#8 | BALANCE |
| X-Mod Depth | CC#70 | SOUND-CTL1 |
| OSC LFO 1 Depth | CC#18 | GENERAL3 |

### Pitch Envelope Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| Pitch Envelope Depth | CC#25 | MODE2 only |
| Pitch Envelope Attack | CC#26 | MODE2 only |
| Pitch Envelope Decay | CC#27 | MODE2 only |

### Filter Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| Cutoff Frequency | CC#74 | SOUND-CTL5 |
| Resonance | CC#71 | SOUND-CTL2 |
| Cutoff Key Follow | CC#30 | MODE2 only |
| Filter LFO 1 Depth | CC#19 | GENERAL4 |
| Filter Envelope Depth | CC#81 | GENERAL6 |
| Filter Envelope Attack | CC#82 | GENERAL7 |
| Filter Envelope Decay | CC#83 | GENERAL8 |
| Filter Envelope Sustain | CC#28 | MODE2 only |
| Filter Envelope Release | CC#29 | MODE2 only |

### Amplifier Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| Amplifier Level | CC#7 | VOLUME |
| Amp LFO 1 Depth | CC#80 | GENERAL5 |
| Amp Envelope Attack | CC#73 | SOUND-CTL4 |
| Amp Envelope Decay | CC#75 | SOUND-CTL6 |
| Amp Envelope Sustain | CC#31 | MODE2 only |
| Amp Envelope Release | CC#72 | SOUND-CTL3 |

### Control Section
| Parameter | CC# | Description |
|-----------|-----|-------------|
| Portamento Time | CC#5 | PORTA-TIME |
| Portamento Switch | CC#65 | Standard MIDI |
| Hold 1 (Sustain) | CC#64 | Standard MIDI |
| Modulation | CC#1 | Standard MIDI |
| Expression | CC#11 | Standard MIDI |
| Pan | CC#10 | Standard MIDI |

### Bank Select & Program Change
| Function | CC#/Message | Description |
|----------|-------------|-------------|
| Bank Select MSB | CC#0 | See bank table below |
| Bank Select LSB | CC#32 | See bank table below |
| Program Change | PC 0-127 | Patch/Performance selection |

#### Bank Select Values
| Bank | CC#0 (MSB) | CC#32 (LSB) |
|------|------------|-------------|
| User | 50H (80) | 00H (0) |
| Preset 1 | 51H (81) | 00H (0) |
| Preset 2 | 51H (81) | 01H (1) |
| Preset 3 | 51H (81) | 02H (2) |
| Card 01-32 | 52H (82) | 00H-1FH (0-31) |

---

## 3. Plugin Architecture

### Core Components

```
jp-8080-controller/
├── Source/
│   ├── PluginProcessor.cpp/.h      # MIDI processing & parameter management
│   ├── PluginEditor.cpp/.h         # Main GUI window
│   ├── JP8080Parameters.cpp/.h     # Parameter definitions & CC mappings
│   ├── MIDIHandler.cpp/.h          # MIDI output handling
│   ├── GUI/
│   │   ├── OscillatorPanel.cpp/.h  # OSC 1 & 2 controls
│   │   ├── FilterPanel.cpp/.h      # Filter section
│   │   ├── EnvelopePanel.cpp/.h    # ADSR envelopes (Filter & Amp)
│   │   ├── LFOPanel.cpp/.h         # LFO 1 & 2 controls
│   │   ├── EffectsPanel.cpp/.h     # Multi-FX, Delay, Tone Control
│   │   ├── MixerPanel.cpp/.h       # Level, Pan, Balance
│   │   └── PatchBrowser.cpp/.h     # Bank/Program selection
│   └── Utils/
│       ├── MIDILearn.cpp/.h        # MIDI learn functionality
│       └── PresetManager.cpp/.h    # Save/load plugin presets
├── Resources/
│   └── Images/                     # GUI graphics (optional)
└── JuceLibraryCode/               # JUCE library (auto-generated)
```

### Data Flow

```
User GUI Action → Parameter Change → MIDI CC Message → JP-8080 Hardware
                                   ↓
                          Logic Pro Automation
```

---

## 4. Implementation Steps

### Phase 1: Project Setup
1. Install JUCE framework and Projucer
2. Create new Audio Plugin project (AU format)
3. Configure project for MIDI output
4. Set up Xcode build schemes for AU
5. Test basic plugin loading in Logic Pro

### Phase 2: Parameter System
1. Define all JP-8080 parameters with CC mappings
2. Create parameter value ranges matching JP-8080 specs (0-127)
3. Implement bidirectional parameter binding
4. Add parameter smoothing for real-time control
5. Support Logic Pro automation recording/playback

### Phase 3: MIDI Output
1. Implement MIDI CC output functionality
2. Add MIDI channel selection (JP-8080 uses Upper/Lower part channels)
3. Implement Bank Select + Program Change sequences
4. Add SysEx support for advanced features (optional)
5. Handle MIDI device selection/routing

### Phase 4: User Interface
1. Design panel layout mirroring JP-8080 front panel sections
2. Implement rotary knobs for continuous parameters
3. Add buttons/toggles for switches
4. Create dropdown menus for waveform/type selection
5. Build patch browser with bank/program selection
6. Add visual feedback (LED-style indicators)

### Phase 5: Advanced Features
1. MIDI Learn functionality (click control, move hardware knob)
2. Preset management (save/recall plugin states)
3. Copy/paste parameter sections
4. Randomize functionality
5. Parameter locking during randomization

### Phase 6: Testing & Polish
1. Test with actual JP-8080 hardware
2. Verify all CC mappings work correctly
3. Test Logic Pro automation recording
4. Performance optimization
5. Add user documentation

---

## 5. GUI Design Concept

The interface should visually mirror the JP-8080's panel layout for intuitive use:

```
+------------------------------------------------------------------+
|  JP-8080 Controller                              [MIDI Ch: 1 ▼]  |
+------------------------------------------------------------------+
|                                                                  |
|  +------------------+  +------------------+  +------------------+ |
|  |   OSCILLATOR 1   |  |   OSCILLATOR 2   |  |    OSC COMMON    | |
|  | Waveform [    ▼] |  | Waveform [    ▼] |  | Balance [====]   | |
|  | Control1 [====]  |  | Range    [====]  |  | X-Mod   [====]   | |
|  | Control2 [====]  |  | Fine     [====]  |  | LFO Dep [====]   | |
|  |                  |  | Control1 [====]  |  |                  | |
|  |                  |  | Control2 [====]  |  |                  | |
|  +------------------+  +------------------+  +------------------+ |
|                                                                  |
|  +------------------+  +------------------+  +------------------+ |
|  |     FILTER       |  |   FILTER ENV     |  |    AMP ENV       | |
|  | Type   [    ▼]   |  | Depth  [====]    |  | Level  [====]    | |
|  | Cutoff [====]    |  | Attack [====]    |  | Attack [====]    | |
|  | Reso   [====]    |  | Decay  [====]    |  | Decay  [====]    | |
|  | KeyFol [====]    |  | Sustain[====]    |  | Sustain[====]    | |
|  | LFO Dp [====]    |  | Release[====]    |  | Release[====]    | |
|  +------------------+  +------------------+  +------------------+ |
|                                                                  |
|  +------------------+  +------------------+  +------------------+ |
|  |      LFO 1       |  |      LFO 2       |  |     EFFECTS      | |
|  | Wave   [    ▼]   |  | Rate   [====]    |  | FX Lvl [====]    | |
|  | Rate   [====]    |  | Depth  [====]    |  | Dly Tm [====]    | |
|  | Fade   [====]    |  | Target [    ▼]   |  | Dly FB [====]    | |
|  +------------------+  +------------------+  | Dly Lv [====]    | |
|                                              +------------------+ |
|  +---------------------------------------------------------------+|
|  |  PATCH: [User ▼] [A ▼] [11-88 ▼]    [◄] [►]    [SEND]        ||
|  +---------------------------------------------------------------+|
+------------------------------------------------------------------+
```

---

## 6. Key Technical Considerations

### MIDI Routing in Logic Pro
- Plugin outputs MIDI CC messages
- Route to JP-8080 via Logic Pro's MIDI Environment or External Instrument track
- Consider using "External Instrument" plugin for audio return

### Parameter Automation
- All parameters should be automatable in Logic Pro
- Parameter IDs must be stable across plugin versions
- Use parameter groups for logical organization

### JP-8080 Mode Configuration
- Default to MODE2 for maximum CC control
- JP-8080 setting: `Tx/Rx Edit Mode = MODE2`
- JP-8080 setting: `Tx/Rx Edit SW = ON`

### MIDI Channel Handling
- JP-8080 can use different channels for Upper/Lower parts
- Plugin should support selecting target part
- Performance Control Channel for global changes

---

## 7. Resources & References

### Documentation
- JP-8080 MIDI Implementation: `midi_implementation.pdf`
- JUCE Tutorials: https://juce.com/tutorials/
- Apple Audio Unit Guide: https://developer.apple.com/documentation/audiotoolbox/audio-unit-v3-plug-ins

### Development Tools
- JUCE Framework: https://juce.com/
- Xcode: Mac App Store
- MIDI Monitor: https://www.snoize.com/MIDIMonitor/

### Testing
- auval (Audio Unit Validation): Built into macOS
- Logic Pro: For real-world testing
- MIDI Monitor: For verifying CC output

---

## 8. Estimated Complexity

| Phase | Complexity | Core Deliverable |
|-------|------------|------------------|
| Phase 1 | Low | Working plugin skeleton |
| Phase 2 | Medium | Full parameter system |
| Phase 3 | Medium | MIDI output working |
| Phase 4 | High | Complete GUI |
| Phase 5 | Medium | Advanced features |
| Phase 6 | Low | Polish & documentation |

---

## 9. Future Enhancements

- **Bidirectional Communication**: Receive CC from JP-8080 to update GUI
- **SysEx Support**: Full patch dump/restore
- **Voice Modulator Controls**: Extended panel for vocoder features
- **Arpeggiator/RPS Control**: Pattern selection and tempo sync
- **Multi-instance Support**: Control both Upper and Lower parts simultaneously
- **Hardware Detection**: Auto-detect JP-8080 on MIDI ports
