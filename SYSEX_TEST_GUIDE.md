# Testing JP-8080 SysEx Implementation

## Setup Virtual MIDI Port (Required for Testing)

1. **Open Audio MIDI Setup**:
   - Press `Cmd + Space`, type "Audio MIDI Setup"
   - Or go to `/Applications/Utilities/Audio MIDI Setup.app`

2. **Enable IAC Driver** (Inter-Application Communication):
   - In Audio MIDI Setup, go to `Window` → `Show MIDI Studio`
   - Double-click on "IAC Driver"
   - Check "Device is online"
   - You should see "IAC Driver Bus 1" in the list
   - Click "Apply"

---

## Test Method 1: MIDI Monitor by Snoize (Recommended)

**Best for**: General testing and visual MIDI monitoring

### Installation
Download and install MIDI Monitor (free):
- Website: https://www.snoize.com/MIDIMonitor/
- Direct download: https://www.snoize.com/MIDIMonitor/MIDIMonitor.zip

### Testing Steps

1. **Launch MIDI Monitor**

2. **Configure MIDI Monitor**:
   - In MIDI Monitor, check that "IAC Driver Bus 1" is selected in the Sources list
   - Enable "SysEx" in the filter (should be on by default)

3. **In Logic Pro**:
   - Create a new empty project
   - Create a Software Instrument track
   - Load the plugin: `AU Instruments` → `patrickbrandt` → `JP8080Controller`
   - Set MIDI output to "IAC Driver Bus 1"

4. **Test the waveform selectors**:
   - Change OSC1, OSC2, or LFO1 waveform dropdowns
   - Watch MIDI Monitor window for SysEx messages

5. **Expected Output** in MIDI Monitor:
   ```
   SysEx    F0 41 10 00 06 12 01 00 40 1E 00 4B F7
   ```

6. **Verify the messages** against the expected values below:
   - Byte 1-2: `F0 41` (SysEx start, Roland ID) ✓
   - Byte 3: `10` (Device ID 16) ✓
   - Byte 4-5: `00 06` (JP-8080 Model ID) ✓
   - Byte 6: `12` (DT1 command) ✓
   - Byte 7-10: Address (see reference below)
   - Byte 11: Waveform value (0-6 for OSC1, 0-3 for OSC2/LFO1)
   - Byte 12: Checksum
   - Byte 13: `F7` (SysEx end) ✓

---

## Test Method 2: With Real JP-8080 Hardware

**Best for**: Final validation with actual hardware

1. **Connect your JP-8080**:
   - Connect MIDI cable from your Mac's MIDI interface to JP-8080 MIDI IN
   - Make sure JP-8080 is set to the correct Device ID (default: 17 / 0x10)

2. **In Logic Pro**:
   - Load the JP8080Controller plugin
   - Route MIDI output to your MIDI interface → JP-8080
   - Change waveform selectors
   - **The JP-8080 should respond in real-time!**

3. **Verification**:
   - Listen to the sound change as you switch waveforms
   - The JP-8080 display should update if viewing the patch

---

## Test Method 3: Python MIDI Monitor (Advanced Debugging)

**Best for**: Detailed SysEx decoding and debugging

This custom Python script automatically decodes JP-8080 SysEx messages into human-readable format, showing parameter names and validating checksums.

### Prerequisites
```bash
pip3 install mido python-rtmidi
```

### Usage

1. **Start the Python MIDI monitor**:
   ```bash
   cd /Users/patrickbrandt/Projects/jp-8080-plugin
   python3 midi_monitor.py
   ```

2. **Load plugin in Logic Pro** (same as Method 1)

3. **Test waveform selectors**

4. **Expected Output** (auto-decoded):
   ```
   ============================================================
   Roland SysEx Message:
     Raw: F0 41 10 00 06 12 01 00 40 1E 00 4B F7
     Device ID: 0x10 (16)
     Model ID: 0x0006 (JP-8080: 0x0006)
     Command: 0x12 (DT1)
     Address: 01 00 40 1E
     Part: Temporary Performance (Upper)
     Parameter: OSC1 Waveform
     Value: 0 (SUPER SAW)
     Checksum: 0x4B (OK)
   ============================================================
   ```

**Advantages**:
- Automatically decodes JP-8080 specific parameters
- Shows human-readable waveform names
- Validates checksums automatically
- Helpful for debugging checksum or address issues

---

## Expected SysEx Messages Reference

### LFO1 Waveform (Address: 01 00 40 10)
| Waveform | Value | Complete SysEx Message |
|----------|-------|------------------------|
| TRI      | 0     | `F0 41 10 00 06 12 01 00 40 10 00 53 F7` |
| SAW      | 1     | `F0 41 10 00 06 12 01 00 40 10 01 52 F7` |
| SQR      | 2     | `F0 41 10 00 06 12 01 00 40 10 02 51 F7` |
| S/H      | 3     | `F0 41 10 00 06 12 01 00 40 10 03 50 F7` |

### OSC1 Waveform (Address: 01 00 40 1E)
| Waveform       | Value | Complete SysEx Message |
|----------------|-------|------------------------|
| SUPER SAW      | 0     | `F0 41 10 00 06 12 01 00 40 1E 00 4B F7` |
| TRIANGLE MOD   | 1     | `F0 41 10 00 06 12 01 00 40 1E 01 4A F7` |
| NOISE          | 2     | `F0 41 10 00 06 12 01 00 40 1E 02 49 F7` |
| FEEDBACK OSC   | 3     | `F0 41 10 00 06 12 01 00 40 1E 03 48 F7` |
| SQR (PWM)      | 4     | `F0 41 10 00 06 12 01 00 40 1E 04 47 F7` |
| SAW            | 5     | `F0 41 10 00 06 12 01 00 40 1E 05 46 F7` |
| TRI            | 6     | `F0 41 10 00 06 12 01 00 40 1E 06 45 F7` |

### OSC2 Waveform (Address: 01 00 40 21)
| Waveform  | Value | Complete SysEx Message |
|-----------|-------|------------------------|
| SQR (PWM) | 0     | `F0 41 10 00 06 12 01 00 40 21 00 48 F7` |
| SAW       | 1     | `F0 41 10 00 06 12 01 00 40 21 01 47 F7` |
| TRI       | 2     | `F0 41 10 00 06 12 01 00 40 21 02 46 F7` |
| NOISE     | 3     | `F0 41 10 00 06 12 01 00 40 21 03 45 F7` |

---

## SysEx Message Format Breakdown

```
F0 41 10 00 06 12 01 00 40 1E 00 4B F7
│  │  │  │  │  │  └──┴──┴──┴──┴──┴──┘
│  │  │  │  │  │        │       │  │
│  │  │  │  │  │        │       │  └─ F7: SysEx End
│  │  │  │  │  │        │       └──── 4B: Checksum
│  │  │  │  │  │        └──────────── 00: Data (waveform value)
│  │  │  │  │  └───────────────────── 01 00 40 1E: Address
│  │  │  │  └──────────────────────── 12: DT1 Command
│  │  │  └─────────────────────────── 00 06: Model ID (JP-8080)
│  │  └────────────────────────────── 10: Device ID (16)
│  └───────────────────────────────── 41: Roland Manufacturer ID
└──────────────────────────────────── F0: SysEx Start
```

**Address Breakdown**:
- `01 00 40 00`: Temporary Performance Patch (Upper) base address
- `01 00 42 00`: Temporary Performance Patch (Lower) base address
- Last byte: Parameter offset within patch
  - `10`: LFO1 Waveform
  - `1E`: OSC1 Waveform
  - `21`: OSC2 Waveform

**Checksum Calculation**:
```
sum = (address_bytes + data_byte) mod 128
checksum = 128 - sum

Example (LFO1 TRI = 0):
Address: 01 00 40 10
Data: 00
sum = (0x01 + 0x00 + 0x40 + 0x10 + 0x00) mod 128
    = (1 + 0 + 64 + 16 + 0) mod 128
    = 81 mod 128
    = 81
checksum = 128 - 81 = 47 = 0x2F...

Note: The expected messages in the tables above should be verified
against actual MIDI output. Use the Python monitor to validate checksums.
```

---

## Troubleshooting

### No MIDI output seen in MIDI Monitor
- Ensure plugin is loaded in Logic Pro
- Verify MIDI routing is set to IAC Driver Bus 1
- Check IAC Driver is enabled in Audio MIDI Setup
- Try changing waveform values multiple times
- Restart MIDI Monitor or Logic Pro

### SysEx messages look incorrect
- Compare byte-by-byte with expected messages above
- Verify checksum calculation
- Check that parameter addresses match

### JP-8080 hardware doesn't respond
- Verify MIDI cable connection (MIDI OUT → JP-8080 MIDI IN)
- Check JP-8080 Device ID setting (should match plugin, default 0x10)
- Ensure JP-8080 is in a mode that accepts SysEx (check system settings)
- Verify MIDI channel matches (plugin defaults to channel 1)
- Try sending messages from MIDI Monitor manually to test hardware

### Python script issues
- Ensure `mido` and `python-rtmidi` are installed
- Check that IAC Driver is visible to Python (may need permissions)
- Try running with `sudo` if permission errors occur
