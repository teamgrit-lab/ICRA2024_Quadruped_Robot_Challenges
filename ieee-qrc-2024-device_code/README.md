# ieee-qrc-2024-device_code

Go to [Icra](https://2024.ieee-icra.org/)

# Moth Development Guide

If you want to use Moth Server, go to the [link](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/tree/main/Moth/python)

# Quick Start

## Dependency

```
sudo apt install libwebsocketpp-dev
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```

## Installation

```
cd {your_ws}/unitree_sdk2-main/
sudo ./install.sh
```

## Build examples

```
rm -rf build
mkdir build
cd build
cmake ..
make
```
