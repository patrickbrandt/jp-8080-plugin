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

## 2. JP-8080 Hardware Specifications

### System Specifications
- **Maximum Polyphony**: 10 voices (Voice Modulator OFF), 8 voices (Voice Modulator ON)
- **Parts**: 2 (Upper, Lower)
- **Key Modes**: Single, Dual, Split
- **Internal Memory**:
  - Performances: User 64, Preset 192 (3 banks)
  - Patches: User 128, Preset 384 (3 banks)
- **Voice Modulator**: Two 12-band band-pass filters, Noise, Robot Oscillator

### Waveforms
**OSC 1**: SUPER SAW, TRIANGLE MOD, NOISE, FEEDBACK OSC, SQR (PWM), SAW, TRI

**OSC 2**: SQR (PWM), SAW, TRI, NOISE

### Effects Types
**Multi-FX (13 types)**: SUPER CHORUS SLW/MID/FST/CLR, FLANGER SLOW/DEEP/FAST, DEEP PHASING SLW, JET PHASING, TWISTING, FREEZE PHASE 1/2, DISTORTION

**Delay (5 types)**: PANNING L->R, PANNING R->L, PANNING SHORT, MONO SHORT, MONO LONG

### Filter Types
- **HPF** (High-pass filter)
- **BPF** (Band-pass filter)
- **LPF** (Low-pass filter)
- **Cutoff Slope**: -12dB/oct or -24dB/oct

---

## 3. JP-8080 MIDI CC Mappings

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
| Parameter | CC# | Description | Value Range |
|-----------|-----|-------------|-------------|
| Portamento Time | CC#5 | PORTA-TIME | 0-127 |
| Portamento Switch | CC#65 | Standard MIDI | 0-63=OFF, 64-127=ON |
| Hold 1 (Sustain) | CC#64 | Standard MIDI | 0-63=OFF, 64-127=ON |
| Modulation | CC#1 | Standard MIDI | 0-127 |
| Expression | CC#11 | Standard MIDI | 0-127 |
| Pan | CC#10 | Standard MIDI | 0=L64, 64=Center, 127=R63 |
| Volume | CC#7 | Standard MIDI | 0-127 |

### Bank Select & Program Change
| Function | CC#/Message | Description |
|----------|-------------|-------------|
| Bank Select MSB | CC#0 | See bank table below |
| Bank Select LSB | CC#32 | See bank table below |
| Program Change | PC 0-127 | Patch/Performance selection |

#### Bank Select Values (for Patches)
| Bank | CC#0 (MSB) | CC#32 (LSB) | Program No. | Patches |
|------|------------|-------------|-------------|---------|
| User | 50H (80) | 00H (0) | 00H-3FH (0-63) | User A 11-88 |
| User | 50H (80) | 00H (0) | 40H-7FH (64-127) | User B 11-88 |
| Preset 1 | 51H (81) | 00H (0) | 00H-3FH (0-63) | Preset 1 A 11-88 |
| Preset 1 | 51H (81) | 00H (0) | 40H-7FH (64-127) | Preset 1 B 11-88 |
| Preset 2 | 51H (81) | 01H (1) | 00H-3FH (0-63) | Preset 2 A 11-88 |
| Preset 2 | 51H (81) | 01H (1) | 40H-7FH (64-127) | Preset 2 B 11-88 |
| Preset 3 | 51H (81) | 02H (2) | 00H-3FH (0-63) | Preset 3 A 11-88 |
| Preset 3 | 51H (81) | 02H (2) | 40H-7FH (64-127) | Preset 3 B 11-88 |
| Card 01-32 | 52H (82) | 00H-1FH (0-31) | Various | Card patches |

#### Bank Select Values (for Performances)
| Bank | CC#0 (MSB) | CC#32 (LSB) | Program No. |
|------|------------|-------------|-------------|
| User | 50H (80) | 00H (0) | 00H-3FH (0-63) |
| Preset 1 | 51H (81) | 00H (0) | 00H-3FH (0-63) |
| Preset 2 | 51H (81) | 01H (1) | 00H-3FH (0-63) |
| Preset 3 | 51H (81) | 02H (2) | 00H-3FH (0-63) |
| Card 01-32 | 52H (82) | 00H-1FH (0-31) | 00H-3FH (0-63) |

### RPN (Registered Parameter Numbers)
The JP-8080 supports the following RPNs:

| RPN MSB | RPN LSB | Parameter | Data Entry MSB | Data Entry LSB |
|---------|---------|-----------|----------------|----------------|
| 00H | 00H | Pitch Bend Sensitivity | 00H-18H (0-24 semitones) | Ignored (00H) |
| 00H | 01H | Master Fine Tuning | 20 00H - 60 00H | -8192 to +8192 (±50 cents) |
| 00H | 02H | Master Coarse Tuning | 28H-58H (-24 to +24 semitones) | Ignored (00H) |
| 7FH | 7FH | RPN Null | N/A | Unsets RPN |

**Important**: After setting RPN parameters via Data Entry (CC#6/38), always send RPN Null (7FH 7FH) to prevent subsequent Data Entry messages from affecting the parameter.

---

## 4. MIDI Configuration Requirements

### JP-8080 System Settings
For the plugin to work correctly, the JP-8080 must be configured as follows:

| Setting | Value | Location | Notes |
|---------|-------|----------|-------|
| Tx/Rx Edit Mode | MODE2 | [MIDI] Bank [5] | Enables full CC parameter control |
| Tx/Rx Edit SW | ON | [MIDI] Bank [5] | Enables parameter editing via MIDI |
| MIDI Sync | OFF or MIDI IN | [MIDI] Bank [5] | Depends on sync source |
| Device ID | 10H-1FH (17-32) | [MIDI] Bank [5] | Default: 10H (17), for SysEx |
| Performance Ctrl CH | 1-16 or OFF | [MIDI] Bank [5] | For performance-level changes |
| Remote Keyboard CH | 1-16 or ALL | [MIDI] Bank [5] | For receiving MIDI input |
| Part MIDI CH (Upper) | 1-16 or OFF | [PART] Bank [3] | Target channel for Upper part |
| Part MIDI CH (Lower) | 1-16 or OFF | [PART] Bank [3] | Target channel for Lower part |

### Tx/Rx Setting Configuration
The Tx/Rx Setting table (42 parameters) allows customization of which CC# controls each parameter. The default MODE2 settings are optimal for most use cases. Custom CC mappings can be configured if needed.

### Active Sensing
- JP-8080 transmits Active Sensing messages every ~200ms when connected
- Plugin should handle Active Sensing timeouts (>400ms = assume disconnected)
- Not critical for one-way controller plugin but useful for bidirectional implementations

---

## 5. Plugin Architecture

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

## 6. Implementation Steps

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

## 7. GUI Design Concept

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

## 8. Key Technical Considerations

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

### SysEx Implementation (Optional Advanced Feature)
For bidirectional communication and patch management:

**SysEx Format**:
```
F0 41 [dev] 00 06 [cmd] [addr] [data/size] [sum] F7

Where:
- F0 = SysEx Start
- 41 = Roland ID
- dev = Device ID (10H-1FH, default 10H)
- 00 06 = Model ID (JP-8080)
- cmd = Command (11H=RQ1 Request, 12H=DT1 Data Set)
- addr = 4-byte address
- data/size = Data bytes or size
- sum = Checksum
- F7 = End of Exclusive
```

**Checksum Calculation**:
```
sum = 128 - ((addr + data) % 128)
```

**Key SysEx Addresses**:
- `01 00 00 00`: Temporary Performance area
- `01 00 40 00`: Temporary Patch (Upper)
- `01 00 42 00`: Temporary Patch (Lower)
- `02 00 00 00`: User Patch area
- `03 00 00 00`: User Performance area

---

## 9. Resources & References

### Documentation
- **JP-8080 MIDI Implementation**: `midi_implementation.pdf` - Complete MIDI CC mappings, RPN support, SysEx format
- **JP-8080 Parameter List**: `params_and_Tx+Rx_Settings.pdf` - All 248 patch parameters with value ranges
- **JP-8080 Specifications**: `synth_specs.pdf` - Hardware specs, polyphony, effects types
- **JP-8080 Performance & Patch List**: `performance_and_patch_list.pdf` - Factory preset reference
- **JUCE Tutorials**: https://juce.com/tutorials/
- **Apple Audio Unit Guide**: https://developer.apple.com/documentation/audiotoolbox/audio-unit-v3-plug-ins

### Development Tools
- JUCE Framework: https://juce.com/
- Xcode: Mac App Store
- MIDI Monitor: https://www.snoize.com/MIDIMonitor/

### Testing
- auval (Audio Unit Validation): Built into macOS
- Logic Pro: For real-world testing
- MIDI Monitor: For verifying CC output

---

## 10. Estimated Complexity

| Phase | Complexity | Core Deliverable |
|-------|------------|------------------|
| Phase 1 | Low | Working plugin skeleton |
| Phase 2 | Medium | Full parameter system |
| Phase 3 | Medium | MIDI output working |
| Phase 4 | High | Complete GUI |
| Phase 5 | Medium | Advanced features |
| Phase 6 | Low | Polish & documentation |

---

## 11. Complete Parameter Reference

### Patch Parameters (248 bytes total)
The JP-8080 has 248 parameters per patch organized into the following sections:

#### Parameter Value Ranges Summary
| Section | Parameters | Key Ranges |
|---------|------------|------------|
| **Effects** | Tone Control, Multi-FX, Delay | Bass/Treble: -64 to +63; Levels: 0-127 |
| **LFO 1** | Waveform, Rate, Fade | Waveform: TRI/SAW/SQR/S&H; Rate/Fade: 0-127 |
| **LFO 2** | Rate, Depth (Pitch/Filter/Amp) | Rate: 0-127; Depth: -64 to +63 |
| **Portamento** | Switch, Time | ON/OFF; Time: 0-127 |
| **OSC 1** | Waveform, Control 1/2 | 7 waveforms; Controls: 0-127 |
| **OSC 2** | Waveform, Sync, Range, Fine, Control 1/2 | 4 waveforms; Range: -WIDE to +WIDE; Fine: ±50 cents |
| **OSC Common** | Balance, X-Mod, LFO Depth, Shift | Balance: -64(OSC1) to +63(OSC2); Others: 0-127 or ±64 |
| **Pitch Envelope** | Depth, Attack, Decay | Depth: -64 to +63; A/D: 0-127 |
| **Filter** | Type, Slope, Cutoff, Resonance, Key Follow | 3 types; 2 slopes; 0-127; Key Follow: ±64 |
| **Filter Envelope** | Depth, ADSR | Depth: -64 to +63; ADSR: 0-127 |
| **Amplifier** | Level, Pan, LFO Depth | Level: 0-127; Pan: OFF/AUTO/MANUAL; LFO: -64 to +63 |
| **Amp Envelope** | ADSR | All: 0-127 |
| **Control** | Mono, Legato, Unison, Velocity | Switches: ON/OFF |
| **Morphing** | Velocity/Control Assign, Bend Assign | ±127/±100/±50; ON/OFF |

#### Important Parameter Notes
1. **Signed Parameters**: Values like -64 to +63 are transmitted as 0-127 (subtract 64 from display value)
2. **Waveform Selection**:
   - OSC1: 0=SUPER SAW, 1=TRIANGLE MOD, 2=NOISE, 3=FEEDBACK OSC, 4=SQR(PWM), 5=SAW, 6=TRI
   - OSC2: 0=SQR(PWM), 1=SAW, 2=TRI, 3=NOISE
3. **Filter Types**: 0=HPF, 1=BPF, 2=LPF
4. **Multi-FX Types**: 13 types (0-12) including chorus, flanger, phaser, distortion
5. **Delay Types**: 5 types (0-4) including panning and mono variants

### Performance Parameters
**Performance Common**: 37 bytes including name, key mode, split point, arpeggio settings

**Voice Modulator**: 41 bytes for vocoder-specific parameters

**Part Parameters**: 8 bytes per part (Upper/Lower) for MIDI channel, transpose, sync settings

### System Parameters
- **Master Tune**: 427.5-452.9 Hz (default 440 Hz)
- **Device ID**: 17-32 (default 17) for SysEx communication
- **MIDI Channels**: Separate channels for Performance Control, Remote Keyboard, and Parts

---

## 12. Future Enhancements

- **Bidirectional Communication**: Receive CC from JP-8080 to update GUI
- **SysEx Support**: Full patch dump/restore
- **Voice Modulator Controls**: Extended panel for vocoder features
- **Arpeggiator/RPS Control**: Pattern selection and tempo sync
- **Multi-instance Support**: Control both Upper and Lower parts simultaneously
- **Hardware Detection**: Auto-detect JP-8080 on MIDI ports
