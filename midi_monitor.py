#!/usr/bin/env python3
"""
Simple MIDI Monitor for testing JP-8080 SysEx messages
Requires: pip3 install mido python-rtmidi
"""

import mido
import sys

def format_sysex(data):
    """Format SysEx data in a readable way"""
    hex_str = ' '.join(f'{b:02X}' for b in data)

    # Parse JP-8080 SysEx if it matches the expected format
    if len(data) >= 13 and data[0] == 0xF0 and data[1] == 0x41:
        device_id = data[2]
        model_id = (data[3] << 8) | data[4]
        command = data[5]
        address = data[6:10]
        value = data[10] if len(data) > 10 else None
        checksum = data[-2] if len(data) >= 2 else None

        print(f"\n{'='*60}")
        print(f"Roland SysEx Message:")
        print(f"  Raw: {hex_str}")
        print(f"  Device ID: 0x{device_id:02X} ({device_id})")
        print(f"  Model ID: 0x{model_id:04X} (JP-8080: 0x0006)")
        print(f"  Command: 0x{command:02X} ({'DT1' if command == 0x12 else 'Unknown'})")
        print(f"  Address: {' '.join(f'{b:02X}' for b in address)}")

        # Decode address
        if address[0] == 0x01 and address[1] == 0x00:
            part = "Upper" if address[2] == 0x40 else "Lower" if address[2] == 0x42 else "Unknown"
            param_offset = address[3]

            param_name = "Unknown"
            if param_offset == 0x10:
                param_name = "LFO1 Waveform"
                waveforms = ["TRI", "SAW", "SQR", "S/H"]
                value_name = waveforms[value] if value < len(waveforms) else "Invalid"
            elif param_offset == 0x1E:
                param_name = "OSC1 Waveform"
                waveforms = ["SUPER SAW", "TRIANGLE MOD", "NOISE", "FEEDBACK OSC", "SQR (PWM)", "SAW", "TRI"]
                value_name = waveforms[value] if value < len(waveforms) else "Invalid"
            elif param_offset == 0x21:
                param_name = "OSC2 Waveform"
                waveforms = ["SQR (PWM)", "SAW", "TRI", "NOISE"]
                value_name = waveforms[value] if value < len(waveforms) else "Invalid"
            else:
                value_name = f"0x{value:02X}"

            print(f"  Part: Temporary Performance ({part})")
            print(f"  Parameter: {param_name}")
            print(f"  Value: {value} ({value_name})")

        if checksum is not None:
            # Verify checksum
            calc_sum = sum(address) + value
            expected_checksum = 128 - (calc_sum % 128)
            checksum_ok = checksum == expected_checksum
            print(f"  Checksum: 0x{checksum:02X} ({'OK' if checksum_ok else f'FAIL - expected 0x{expected_checksum:02X}'})")

        print(f"{'='*60}")
    else:
        print(f"\nSysEx: {hex_str}")

    return hex_str

def monitor_midi():
    """Monitor MIDI messages from all input ports"""
    print("Available MIDI Input Ports:")
    input_names = mido.get_input_names()

    if not input_names:
        print("  No MIDI input ports found!")
        print("\nTip: Create a virtual MIDI port in Audio MIDI Setup")
        return

    for i, name in enumerate(input_names):
        print(f"  [{i}] {name}")

    print("\nMonitoring all MIDI inputs for SysEx messages...")
    print("Press Ctrl+C to stop\n")

    try:
        # Open all input ports
        ports = []
        for name in input_names:
            try:
                port = mido.open_input(name)
                ports.append((name, port))
                print(f"Listening on: {name}")
            except Exception as e:
                print(f"Could not open {name}: {e}")

        print("\nWaiting for MIDI messages...\n")

        # Monitor messages
        while True:
            for name, port in ports:
                for msg in port.iter_pending():
                    if msg.type == 'sysex':
                        print(f"[{name}]")
                        format_sysex(msg.data)
                    else:
                        print(f"[{name}] {msg}")

    except KeyboardInterrupt:
        print("\n\nStopping MIDI monitor...")
    finally:
        for _, port in ports:
            port.close()

if __name__ == "__main__":
    try:
        monitor_midi()
    except ImportError:
        print("Error: mido library not found")
        print("Install with: pip3 install mido python-rtmidi")
        sys.exit(1)
