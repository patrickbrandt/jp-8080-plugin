# JP-8080 Logic Pro Controller Plugin

An Audio Unit (AU) plugin for Logic Pro that provides a graphical interface to control the Roland JP-8080 hardware synthesizer via MIDI CC messages.

## Technology Stack

- **Recommended**: JUCE Framework (C++ with excellent Audio Unit support)
- **Alternative**: Native Apple AUv3 (Swift/Objective-C++)

## MIDI CC Mappings

Complete mappings extracted from the JP-8080 manual organized by section:

- **Effects**: Tone Control, Multi-FX Level, Delay (Time/Feedback/Level)
- **LFO**: LFO 1 & 2 Rate, Fade, Depth controls
- **Oscillator**: OSC 1 & 2 controls, Balance, X-Mod
- **Filter**: Cutoff, Resonance, Envelope (ADSR)
- **Amplifier**: Level, Envelope (ADSR)
- **Control**: Portamento, Sustain, Modulation, Expression, Pan
- **Bank Select**: User, Preset 1-3, Card banks with MSB/LSB values

## Implementation Phases

1. **Project Setup** - JUCE/Xcode configuration
2. **Parameter System** - CC mappings, automation support
3. **MIDI Output** - CC messages, Bank/Program Change
4. **User Interface** - Panels mirroring JP-8080 layout
5. **Advanced Features** - MIDI Learn, presets
6. **Testing & Polish**

## Key Technical Notes

- Plugin should default to MODE2 for maximum CC control
- JP-8080 requires `Tx/Rx Edit Mode = MODE2` and `Tx/Rx Edit SW = ON`
- Supports Upper/Lower part channel selection

## Documentation

See [plan.md](plan.md) for the full implementation plan including:
- Complete MIDI CC mapping tables
- Detailed file structure
- GUI design mockup
- Technical considerations

## Resources

- [JUCE Framework](https://juce.com/)
- [Apple Audio Unit Documentation](https://developer.apple.com/documentation/audiotoolbox/audio-unit-v3-plug-ins)
