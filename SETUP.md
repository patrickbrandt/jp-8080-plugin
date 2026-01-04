# JP-8080 Controller Plugin - Development Setup

## Phase 1: Project Setup

### Step 1: Install JUCE Framework and Projucer ✓

**Date**: January 4, 2026

**JUCE Version**: 7.0.12

**Installation Location**: `~/Projects/JUCE/`

**Components Installed**:
- JUCE Framework (modules, examples, documentation)
- Projucer.app - JUCE project manager and code generator
- DemoRunner.app - For testing JUCE demos

**Projucer Location**: `~/Projects/JUCE/Projucer.app`

### Step 2: Create New Audio Plugin Project (AU format) ✓

**Date**: January 4, 2026

**Project Name**: JP8080Controller

**Project Type**: Audio Plugin (AU only)

**Plugin Configuration**:
- Plugin Type: MIDI Effect Plugin
- Accepts MIDI: Yes
- Produces MIDI: Yes
- Is Synth: No
- Plugin Code: Jp80
- Manufacturer Code: PaBr
- Bundle ID: com.patrickbrandt.jp8080controller

**Source Files Created**:
- `Source/PluginProcessor.h` - Main processor header
- `Source/PluginProcessor.cpp` - Main processor implementation
- `Source/PluginEditor.h` - GUI editor header
- `Source/PluginEditor.cpp` - GUI editor implementation

**Build System**:
- Xcode project generated at: `Builds/MacOSX/JP8080Controller.xcodeproj`
- Debug and Release configurations configured
- AU plist created: `Builds/MacOSX/Info-AU.plist`

**JUCE Modules Included**:
- juce_audio_basics
- juce_audio_devices
- juce_audio_formats
- juce_audio_plugin_client
- juce_audio_processors
- juce_audio_utils
- juce_core
- juce_data_structures
- juce_events
- juce_graphics
- juce_gui_basics
- juce_gui_extra

### Step 3: Configure Project for MIDI Output ✓

**Date**: January 4, 2026

**MIDI Output Infrastructure Added**:

1. **MIDI Channel Management**:
   - Added `midiChannel` member variable (default: 1)
   - Implemented `setMidiChannel()` method with validation (1-16)
   - Implemented `getMidiChannel()` getter method

2. **MIDI CC Helper Method**:
   - Created `sendMidiCC()` method for sending Control Change messages
   - Parameters: CC number (0-127), value (0-127), channel (1-16)
   - Automatic value range validation using `juce::jlimit()`
   - Proper MIDI channel conversion (display 1-16 → internal 0-15)

3. **Process Block Configuration**:
   - MIDI buffer handling set up in `processBlock()`
   - Ready to generate and output MIDI CC messages
   - Comments documenting where parameter-driven MIDI will be added

**Key Implementation Details**:
- MIDI messages created using `juce::MidiMessage::controllerEvent()`
- Messages added to output buffer at sample position 0
- All MIDI values validated before transmission
- Infrastructure ready for Phase 2 parameter system

**Code Organization**:
- MIDI methods grouped in separate section of implementation
- Clear separation between audio and MIDI processing
- Documented for future parameter integration

### Step 4: Set up Xcode Build Schemes for AU ✓

**Date**: January 4, 2026

**Build Configuration**:

1. **Available Schemes**:
   - JP8080Controller - All
   - JP8080Controller - AU
   - JP8080Controller - Standalone Plugin
   - JP8080Controller - VST3

2. **Build Process**:
   - Used `xcodebuild` command-line tool
   - Built AU scheme in Debug configuration
   - Command: `xcodebuild -project Builds/MacOSX/JP8080Controller.xcodeproj -scheme "JP8080Controller - AU" -configuration Debug clean build`

3. **Build Results**:
   - Build succeeded with no errors
   - AU component installed to: `~/Library/Audio/Plug-Ins/Components/JP8080Controller.component`
   - Plugin automatically code-signed with "Sign to Run Locally"

4. **Plugin Information**:
   - Plugin Name: JP-8080 Controller
   - Manufacturer: PatrickBrandt
   - Type: Audio Unit MIDI Effect (aumi)
   - Version: 0.1.0

**Verification**:
- AU component file exists and is valid
- Plugin ready for Logic Pro testing

### Step 5: Test Basic Plugin Loading in Logic Pro ✓

**Date**: January 4, 2026

**AU Validation Results**:

1. **Plugin Registration**:
   - Confirmed registered with macOS Audio Unit system
   - Type: aumi (Audio Unit MIDI Effect)
   - Plugin Code: Jp80
   - Manufacturer Code: PaBr
   - Display Name: "PatrickBrandt: JP-8080 Controller"

2. **auval Validation Tests**:
   - ✅ OPEN TIMES: PASS (Cold: 10.4ms, Warm: 0.05ms)
   - ✅ REQUIRED PROPERTIES: PASS
   - ✅ RECOMMENDED PROPERTIES: PASS
   - ✅ OPTIONAL PROPERTIES: PASS (Latency, Tail Time, Bypass Effect)
   - ✅ SPECIAL PROPERTIES: PASS (Cocoa UI, Factory Presets, Class Info, Host Callbacks)
   - ✅ PUBLISHED PARAMETER INFO: PASS
   - ⚠️ Channel Configuration: Note about bus format (expected for MIDI-only plugins)

3. **Cocoa UI**:
   - JUCE-generated Cocoa view confirmed working
   - Custom UI view class: JUCE_AUCocoaViewClass_5ad628e01531d8e6

**Testing in Logic Pro**:

To test the plugin in Logic Pro:

1. **Reset Audio Units Cache** (if needed):
   ```bash
   killall -9 AudioComponentRegistrar
   ```

2. **Open Logic Pro**:
   - Create a new project or open an existing one
   - Create a MIDI/Instrument track

3. **Load the Plugin**:
   - Click on the MIDI FX slot on the track
   - Navigate to: Audio Units > PatrickBrandt > JP-8080 Controller
   - The plugin should load and display the UI window with "JP-8080 Controller" title

4. **Verify Functionality**:
   - Plugin window opens without errors
   - Plugin appears in the MIDI FX chain
   - UI displays correctly

**Current Status**:
- Plugin compiles and builds successfully
- AU component properly installed and registered
- Passes core Audio Unit validation tests
- Ready for Phase 2 implementation (Parameter System)

**Known Notes**:
- Plugin is a MIDI Effect type (processes MIDI, not audio)
- No parameters implemented yet (Phase 2)
- Basic UI placeholder active (full UI in Phase 4)

---

## Phase 2: Parameter System

### Step 1: Define All JP-8080 Parameters with CC Mappings ✓

**Date**: January 4, 2026

**Parameter Definition File Created**: `Source/JP8080Parameters.h`

**CC-Controllable Parameters Defined**: 45 total

1. **Oscillator Section** (9 parameters):
   - OSC1 Control 1 (CC#4), Control 2 (CC#76)
   - OSC2 Range (CC#21), Fine/Wide (CC#77), Control 1 (CC#78), Control 2 (CC#79)
   - OSC Balance (CC#8), X-Mod Depth (CC#70), OSC LFO1 Depth (CC#18)

2. **Pitch Envelope Section** (3 parameters):
   - Depth (CC#25), Attack (CC#26), Decay (CC#27)

3. **Filter Section** (9 parameters):
   - Cutoff (CC#74), Resonance (CC#71), Key Follow (CC#30), LFO1 Depth (CC#19)
   - Env Depth (CC#81), Attack (CC#82), Decay (CC#83), Sustain (CC#28), Release (CC#29)

4. **Amplifier Section** (6 parameters):
   - Level (CC#7), LFO1 Depth (CC#80)
   - Env Attack (CC#73), Decay (CC#75), Sustain (CC#31), Release (CC#72)

5. **LFO Section** (6 parameters):
   - LFO1 Rate (CC#16), Fade (CC#20)
   - LFO2 Rate (CC#17), Pitch Depth (CC#22), Filter Depth (CC#23), Amp Depth (CC#24)

6. **Effects Section** (6 parameters):
   - Tone Control Bass (CC#92), Treble (CC#95)
   - Multi-FX Level (CC#93), Delay Time (CC#12), Feedback (CC#13), Level (CC#94)

7. **Control Section** (6 parameters):
   - Portamento Time (CC#5), Switch (CC#65)
   - Hold 1/Sustain (CC#64), Modulation (CC#1), Expression (CC#11), Pan (CC#10)

**Implementation Details**:
- Parameters organized by JP-8080 panel sections using namespaces
- CC number mapping table for MIDI message generation
- Display name table for UI labels
- Helper functions: `getCCNumber()`, `getDisplayName()`, `getAllParameterIDs()`
- Based on MODE2 Tx/Rx settings for maximum CC control

**Excluded Parameters**:
- Filter Type and Slope (SysEx-only, no CC control)
- Waveform selections (SysEx-only, no CC control)
- Other patch-level parameters requiring SysEx

**File Structure**:
```cpp
namespace JP8080Parameters {
    namespace Oscillator { ... }
    namespace PitchEnv { ... }
    namespace Filter { ... }
    namespace Amplifier { ... }
    namespace LFO { ... }
    namespace Effects { ... }
    namespace Control { ... }
}
```

**Next Steps**:
- Create parameter value ranges matching JP-8080 specs (0-127)
