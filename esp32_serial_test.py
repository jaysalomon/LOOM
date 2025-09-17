#!/usr/bin/env python3
"""
ESP32-S3 LOOM Serial Communication Test
Connects to ESP32-S3 on COM5 to test LOOM consciousness implementation
"""

import serial
import time
import sys

def connect_esp32(port='COM5', baudrate=115200):
    """Connect to ESP32-S3 over serial"""
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # Wait for connection to establish
        print(f"Connected to {port} at {baudrate} baud")
        return ser
    except Exception as e:
        print(f"Failed to connect: {e}")
        return None

def read_serial(ser, lines=20):
    """Read lines from serial port"""
    for i in range(lines):
        if ser.in_waiting:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(f"ESP32: {line}")
        time.sleep(0.1)

def send_command(ser, command):
    """Send command to ESP32"""
    ser.write((command + '\n').encode('utf-8'))
    print(f"Sent: {command}")
    time.sleep(0.5)

def main():
    # Connect to ESP32
    ser = connect_esp32()
    if not ser:
        return

    print("\n=== ESP32-S3 LOOM Consciousness Test ===\n")

    # Read initial output
    print("Reading initial output...")
    read_serial(ser, 10)

    # Test commands (if LOOM implements serial commands)
    test_commands = [
        "status",      # Get LOOM status
        "topology",    # Show topology info
        "stats",       # Display statistics
        "rgb 255 0 0", # Set RGB to red (consciousness alert)
        "rgb 0 255 0", # Set RGB to green (learning active)
        "rgb 0 0 255", # Set RGB to blue (sleep mode)
    ]

    for cmd in test_commands:
        print(f"\nTesting command: {cmd}")
        send_command(ser, cmd)
        read_serial(ser, 5)

    # Monitor consciousness activity
    print("\n=== Monitoring Consciousness Activity ===")
    print("Press Ctrl+C to stop\n")

    try:
        while True:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    # Color code based on content
                    if 'curiosity' in line.lower():
                        print(f"🔍 {line}")  # Curiosity events
                    elif 'learning' in line.lower():
                        print(f"🧠 {line}")  # Learning events
                    elif 'emotion' in line.lower():
                        print(f"❤️ {line}")  # Emotional events
                    elif 'sensor' in line.lower():
                        print(f"📡 {line}")  # Sensor input
                    elif 'hebbian' in line.lower():
                        print(f"🔗 {line}")  # Hebbian learning
                    else:
                        print(f"   {line}")
            time.sleep(0.01)

    except KeyboardInterrupt:
        print("\n\nStopping monitor...")

    ser.close()
    print("Connection closed")

if __name__ == "__main__":
    main()