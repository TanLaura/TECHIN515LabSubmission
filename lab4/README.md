# TECHIN515 Lab 4 - Magic Wand

This project implements a gesture recognition system using an ESP32 microcontroller with an MPU6050. The system consists of two main components: a gesture data capture system and a gesture inference system.

## Hardware Requirements

- ESP32 development board
- MPU6050 sensor
- LED (Check with Prototyping Lab for options of LED)
- Jumper wires for connections
- Breadboard for quick prototyping
- Battery and enclosure for creating your wand

### Hardware Connections

Connect the MPU6050 to your ESP32:

- VCC → 3.3V
- GND → GND
- SCL → GPIO22 (or your I2C clock pin)
- SDA → GPIO21 (or your I2C data pin)
- D3 → button → GND
- R → D2
- G → D1
- B → D0

## Deliverables

Deeliberable structure.

```
TECHIN515-magic-wand/
├── src/
│   ├── sketches/               # Sketches with comments and Edge Impulse exports
│   ├── python-scripts/         # Python scripts for data collection
│   └── dataset/                # Collected dataset
│
├── docs/
│   ├── report.pdf              # Final report in PDF format
│
├── enclosure/             # CAD/STL files or images of the final enclosure
│   ├── final-enclosure-images/
│   └── notes.md                # Description of materials, design decisions, and battery
│
├── README.md              # Setup instructions, how to run, install dependencies, etc.
└── .gitignore             # Ignore unnecessary files (e.g., __pycache__, temp data)
```
