# LABSoft

LABSoft is a multi-instrument electronic testing software for the Raspberry Pi. It turns a Raspberry Pi into a bench of virtual test instruments, controlled through a touchscreen GUI.

## Instruments

- **Oscilloscope** - Waveform capture and display with trigger support
- **Function Generator** - Signal generation (sine, square, triangle)
- **Voltmeter** - DC/AC voltage measurement
- **Ohmmeter** - Resistance measurement
- **Logic Analyzer** - Digital signal capture and analysis
- **Digital Circuit Checker** - Digital circuit verification
- **Analog Circuit Checker** - Analog circuit verification with signal comparison (time and frequency domain)

## Architecture

LABSoft uses a Model-View-Presenter (MVP) architecture:

- **Model (`src/LAB/`)** - Hardware interface layer. Communicates with external ICs and the Raspberry Pi's peripherals through SPI.
- **View (`src/LABSoft_GUI/`)** - GUI layer built with [FLTK](https://www.fltk.org/).
- **Presenter (`src/LABSoft_Presenter/`)** - Connects the model and view. Handles user input and updates the display.

## Hardware

LABSoft runs on Raspberry Pi boards with the ARM Cortex-A53 architecture:

- Raspberry Pi 3B+
- Raspberry Pi Zero
- Raspberry Pi Zero 2W

External ICs used:

- **AD9833** - Programmable waveform generator (function generator)
- **MCP4XXX** - Digital potentiometer (amplitude/offset control)
- **MCP23S17** - SPI GPIO expander

## Dependencies

- CMake 3.18+
- C++20 compiler
- FLTK
- X11 libraries (Xft, Xinerama, Xcursor, Xrender, Xfixes, Xext)
- Fontconfig
- pthreads

### Libraries (included as submodules)

- [AikaPi](https://github.com/glenbenatiro/AikaPi) - Raspberry Pi hardware abstraction layer
- [AD9833](https://github.com/glenbenatiro/AD9833) - AD9833 waveform generator driver
- [MCP4XXX](https://github.com/glenbenatiro/MCP4XXX) - MCP4XXX digital potentiometer driver
- [MCP23S17](https://github.com/glenbenatiro/MCP23S17) - MCP23S17 GPIO expander driver
- [KISSFFT](https://github.com/mborgerding/kissfft) - FFT library for frequency domain analysis
- [pugixml](https://github.com/zeux/pugixml) - XML parser (used for circuit checker file formats)

## Building

LABSoft is compiled natively on a Raspberry Pi (3B+, Zero, or Zero 2W), since they share the same CPU architecture.

### Install dependencies

```bash
sudo apt update
sudo apt install cmake build-essential libfltk1.3-dev libx11-dev libxft-dev libxinerama-dev libxcursor-dev libxrender-dev libxfixes-dev libxext-dev libfontconfig1-dev
```

### Clone and build

```bash
git clone --recurse-submodules https://github.com/glenbenatiro/LABSoft.git
cd LABSoft
mkdir build && cd build
cmake ../src
make
```

### Run

```bash
./LABSoft
```

The binary can also be compiled on one Pi and transferred to another Pi of the same architecture.

## Disclaimer

This project is for research and educational purposes only. It is not intended for production use.
