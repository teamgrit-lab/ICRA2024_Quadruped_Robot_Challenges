# MOTH - Python

Operation with MOTH Server (Unitree Go2)

## Table

1. [Introduction](#introduction)
2. [Action by each class](#action-by-each-class)
3. [client.py](#clientpy)
4. [video_moth.py](#video_mothpy)
5. [sensor_msg.py](#sensor_msgpy)

## Introduction

This project is Python-based software that utilizes Moth servers to control the behavior of Unitree Go2 and processes image and sensor data.

## Action by each class

Each Class operates as a block diagram in the following illustration.

![Block diagram](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/blob/main/Moth/python/Image/Block%20diagram.png)

### client.py

`client.py` receives joystick control values from the web and sends front-facing video of Unitree Go2 to the web.

- Joystick received value

|Parameters|Type|
|:---:|:---:|
|Moth.lx|Float|
|Moth.ly|Float|
|Moth.rx|Float|
|Moth.ry|Float|
|Moth.keys|uint16_t|
|Moth.mode|array[16]|

`Moth.lx` `Moth.ly` `Moth.rx` `Moth.ry` and `Moth.keys` are values corresponding to `joystick.lx` `joystick.ly` `joystick.rx` `joystick.ry` and `joystick.keys` in the [Unitree Development Guide](https://support.unitree.com/home/en/developer/Get_remote_control_status), respectively, and `Moth.mode` is the transformation of `Moth.keys` into binary array form (LSB).

Use `client.py` by entering the channel values provided for each participant.

```
python3 client.py "channel_name"
```

If you are going to put Moth Class directly into your model, refer to `"__main__"` in the code.

For participants who do not use Unitree Go2, you can set the gamepad by referring to the table below and the [Gamepad Tester link](https://hardwaretester.com/gamepad).

|Standard|Parameters|Value|
|:---:|:---:|:---:|
|AXIS 0|Moth.lx|-1.0 ~ 1.0|
|AXIS 1|Moth.ly|-1.0 ~ 1.0|
|AXIS 2|Moth.rx|-1.0 ~ 1.0|
|AXIS 3|Moth.ry|-1.0 ~ 1.0|
|B0|Moth.mode[9]|0000_0000_1000_0000|
|B1|Moth.mode[8]|0000_0000_0100_0000|
|B2|Moth.mode[11]|0000_0000_0010_0000|
|B3|Moth.mode[10]|0000_0000_0001_0000|
|B4|Moth.mode[1]|0100_0000_0000_0000|
|B5|Moth.mode[0]|1000_0000_0000_0000|
|B6|Moth.mode[5]|0000_0100_0000_0000|
|B7|Moth.mode[4]|0000_1000_0000_0000|
|B8|Moth.mode[3]|0001_0000_0000_0000|
|B9|Moth.mode[2]|0010_0000_0000_0000|
|B10|Moth.mode[6]|0000_0010_0000_0000|
|B11|Moth.mode[7]|0000_0001_0000_0000|
|B12|Moth.mode[12]|0000_0000_0000_1000|
|B13|Moth.mode[14]|0000_0000_0000_0010|
|B14|Moth.mode[15]|0000_0000_0000_0001|
|B15|Moth.mode[13]|0000_0000_0000_0100|

### video_moth.py

`video_moth.py` defines a GStreamer Pipeline for using the front camera of Unitree Go2, and there is a `getStream` function in the Class that receives 15 frames of image.

For a description of the Multimedia service in Unitree Go2, see the [Unitree Development Guide - Multimedia Service](https://support.unitree.com/home/en/developer/Multimedia_Services).

Use the `Go2Camera` Class in `video_moth.py` at `client.py` to send the video to the web in real time.

If your robot is using a different robot than Unitree Go2, you should change the `self.pipeline_cmd` variable in Class accordingly.

The IEEE-QRC page uses H.264 decoding, and participants using other encoding methods should contact TeamGRIT.

If you don't use the camera, you should delete all lines using `Go2Camera` within `client.py` (Line No. 23, 24, 37, 40, 43, 44)

### sensor_msg.py

`sensor_msg.py` is a class defined to get sensor values of Unitree Go2.

`client.py` sends the sensor values stored in `sensor_msg.py` to the web in real time.

You can see `sensor_msg` being used for `Moth.msg` at `client.py` .

```
class Moth():
  def __init__(self, channel):
    ...
    self.msg = smsg()
    ...
```

If the user enters the value directly into the parameter in the class, the value can be received from the web.

- smsg parameter value

|Parameters|Data type|Description|
|:---:|:---:|:---:|
|Moth.msg.motor_state_q|float array[12]|Radian value for current motor [Hip, Thigh, Calf]|
|Moth.msg.velocity|float array[3]|Current speed and angular velocity values [Vx, Vy, ωz]|
|Moth.msg.RPY|float array[3]|Current Roll, Pitch, and Yaw values [Roll, Pitch, Yaw]|

During the match, data sent to the web by participants is saved as a file, and all files can be downloaded after the match.

If you need to send additional sensor values, please contact jhs10429@teamgrit.kr.
