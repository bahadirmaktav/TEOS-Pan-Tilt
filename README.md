# Teos Pan Tilt
Teos Pan Tilt is a robotic camera project that can pan horizontally and tilt vertically by controlling it via a user interface.</br>
Source codes and design files related to the software, firmware and mechanical design of the project are located in Teos-Pan-Tilt repository.

## Table of Contents
- [Getting Started](#getting-started)
    - [Firmware](#firmware)
    - [Software](#software)
    - [Mechanics](#mechanics)
- [How To Use](#how-to-use)
- [Demo](#demo)
- [Roadmap](#roadmap)
- [License](#license)

## Getting Started
The project basically consists of three folders.</br>
1. The folder named **firmware** contains the source codes running in the camera module used in the project.
2. The folder named **software** contains the source codes of the user interface.
3. The folder named **mechanics** contains files related to the mechanical design of the project.

### Firmware
The **firmware** runs on the ESP32-CAM module. The v5.1 version of the Espressif IoT Development Framework (IDF) was used in the development of the firmware.</br>
Basically, the functions of the firmware are as follows.</br>
1. Connects to the WiFi network specified in the configuration.
2. Starts a WebSocket server.
3. Rotates the pan and tilt cameras according to the commands coming from the user interface via websocket.
4. Sends camera images to the user interface via websocket according to the camera start/stop command coming from the user interface.

### Software
The **software** is a user interface developed with python (tkinter library) that can run in Windows or Linux environments.</br>
Basically, the functions of the software are as follows.</br>
1. Sends command messages to the websocket server according to the input from the user.
2. Displays the visual data coming from the server in video format on the user interface when the camera is started.

### Mechanics
The **mechanics** contain 3D design files in 3mf format developed with Fusion 360.</br>
The mechanical system was designed so that the module with the camera can move in two axes with servo motors.

![](https://github.com/bahadirmaktav/Teos-Pan-Tilt/tree/develop/demo/3d_design_example.jpg)

## How To Use
How To Use

## Demo
Demo

## Roadmap
Roadmap

## License
License