# Keyboard controler for SPLINE

This is a simple way to send commands to SPLINE serial port using a cheap matrix keypad.

## Wiring

You can use ESP32(-S3) or RP2040 board with these connections:

SPLINE | KEYPAD | ESP32-S3 | RP2040
--- | --- | --- | ---
J10 GND | GND | GND | GND
J10 TX | NC | U0RXD | GP1
J10 RX | NC | U0TXD | GP0
J10 VCC | NC | VBUS | VBUS
NC | R0 | 2 | 2
NC | R1 | 3 | 3
NC | R2 | 4 | 4
NC | R3 | 5 | 5
NC | C0 | 6 | 6
NC | C1 | 7 | 7
NC | C2 | 8 | 8
NC | C3 | 9 | 9

## Photo

TODO

## Commands

TODO
