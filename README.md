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

![](https://github.com/bahadirmaktav/Teos-Pan-Tilt/blob/develop/demo/3d_design_example.jpg)

## How To Use
Teos Pan Tilt is controlled from the user interface that appears on the screen by running the source code in the software folder.</br>
An example user interface image for the final version of the project is shared below.</br>

![](https://github.com/bahadirmaktav/Teos-Pan-Tilt/blob/develop/demo/gui_example.jpg)

As you can see on the GUI example image, user can control camera, motor and websocket connection on interface. *In this version, the user can first connect to the websocket server and then control the motors and camera after seeing the log message on the console stating that the connection has been started.*

## Demo
A gif will be added for the demo example.

## Roadmap
- [x] [Firmware]    Setup IDF environment.
- [x] [Firmware]    Develop servo motor controller interface.
- [x] [Firmware]    Develop WiFi and WebSocket server components.
- [x] [Firmware]    Implement esp32-camera library and get image from ESP32-Camera module.
- [x] [Firmware]    Develop command handler interface.
- [x] [Firmware]    Test command handler mechanism (with Postman), camera and motor controls separately.
- [ ] [Firmware]    Fix bug with camera and motors working together.
- [ ] [Firmware]    Make the camera configurable according to the incoming command from client.
- [x] [Software]    Setup python virtual environment.
- [x] [Software]    Develop websocket client interface.
- [x] [Software]    Develop command manager interface.
- [x] [Software]    Develop camera manager.
- [x] [Software]    Develop UI manager which setups window, buttons, display panel.
- [x] [Software]    Integrate command manager with other manager utilities and attach to UI.
- [x] [Software]    Test commands sended from GUI with firmware.
- [ ] [Software]    Integrate an indicator that shows the status of the websocket server. 
- [ ] [Software]    Add face detection algorithm for the images taken from the camera.
- [ ] [Software]    Add algorithm for controlling motors automatically according to the position of detected image.
- [ ] [Software]    Add GUI option for change the system mode between manual and auto.
- [ ] [Software]    Add GUI option for sending camera configurations to server for setting camera parameters such as resolution etc.
- [x] [Mechanics]   Develop basic 3D designs for servo motors and ESP32-Camera module.
- [x] [Mechanics]   Design mechanical parts of pan tilt project and print them with 3D printer.
- [x] [Mechanics]   Assemble the parts.
- [x] [Electronics] Make the necessary pin connections for the motors, power supply and camera module board.

## License
License information will be added.