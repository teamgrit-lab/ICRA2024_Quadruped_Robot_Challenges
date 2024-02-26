# MOTH

Moth server is a real-time live streaming media server developed to enable the use of a variety of data streams, including videos, even on low-performance, low-power MPU-based IoT devices that are equipped with cameras and Wi-Fi.

It uses the Pub/Sub API Communication model to send and receive data using Web-Friendly technologies (WebSocket, WebCodec, MIME, WebTransport... ).

# IEEE-QRC (Unitree Go2) Architecture Using MOTH Servers

Provides IEEE-QRC participants with remote control services using MOTH servers by default.

![IEEE-QRC Architecture](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/blob/main/Moth/image/IEEE-QRC%20Architecture%20Using%20MOTH%20Servers.jpg)

The web page sends the control data from the connected controller to a miniPC connected to Unitree Go2 in real time, receives the data from the miniPC and performs the same actions as Joystick.

In contrast, mini-PCs send Camera data and Sensor data provided by Unitree Go2 to web pages in real time, and web pages provide participants with UI based on that data.

The IEEE-QRC page is available on the [IEEE-QRC Web Page](https://ieee-qrc.agilertc.com/).

A mini-PC will be provided as an NUC that can upload an AI Model, and another [Jetson Origin NX (16GB)](https://developer.download.nvidia.com/assets/embedded/secure/jetson/orin_nx/docs/Jetson_Orin_NX_DS-10712-001_v0.5.pdf?gRHazndBgO3Fg306ny-dAeY-49gAbpy7RmzIFpkkYXhHwas1HDSs0W69SQ6lNqO6oBqlAbm8PsH38Kx2p_41o6vPsj6ZsaSp_vxS9cnlauvrmf0xketv-Mm9dpof2_xarA0fPiT-Q3OnySr9NYOG-qgAarD1EdO3uNXABRb_3QX02cTX-iImaSnQb37p3A==&t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczovL3d3dy5nb29nbGUuY29tLyJ9) will be provided connected to the mini-PC.

||Mini PC specification|
|:---|:---|
|CPU|13th Gen intel(r) CoreT i9-13800H|
|GPU|NA|
|WIFI|WIFI6|
|Power|90W|
|RAM|16(8+8)GB|

## Development

Check out the [Python MOTH Development Guide](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/tree/main/Moth/python) for additional information on Python development.

If you want to check the example of a MOTH server operating in C++, go to [example/moth](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/tree/main/unitree_sdk2-main/example/moth)
