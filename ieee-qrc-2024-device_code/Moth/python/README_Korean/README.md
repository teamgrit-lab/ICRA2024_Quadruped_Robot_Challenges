# MOTH - Python

MOTH 서버를 활용한 동작(Unitree Go2)

## 목차

1. [소개](#소개)
2. [각 Class별 동작](#각-class별-동작)
3. [client.py](#clientpy)
4. [video_moth.py](#video_mothpy)
5. [sensor_msg.py](#sensor_msgpy)

## 소개

이 프로젝트는 Moth 서버를 활용하여 Unitree Go2의 동작을 제어하고, 영상 및 센서 데이터를 처리하는 Python 기반의 소프트웨어입니다.

## 각 Class별 동작

각각의 Class는 아래와 같은 그림의 블록다이어그램으로 동작합니다.

![Block diagram](https://github.com/teamgrit-lab/ieee-qrc-2024-device_code/blob/main/Moth/python/Image/Block%20diagram.png)

### client.py

client.py는 웹에서 보내는 조이스틱 컨트롤 값을 수신하고, Unitree Go2의 정면 비디오를 웹으로 송신하는 역할을 합니다.

- 조이스틱 수신 값

|파라미터|타입|
|:---:|:---:|
|Moth.lx|Float|
|Moth.ly|Float|
|Moth.rx|Float|
|Moth.ry|Float|
|Moth.keys|uint16_t|
|Moth.mode|array[16]|

`Moth.lx` `Moth.ly` `Moth.rx` `Moth.ry` `Moth.keys`는 각각 [Unitree Development Guide](https://support.unitree.com/home/en/developer/Get_remote_control_status)의 `joystick.lx` `joystick.ly` `joystick.rx` `joystick.ry` `joystick.keys`에 해당하는 값이고, `Moth.mode`는 `Moth.keys`를 이진수 array형태(LSB)로 변환한 값입니다.

`client.py`의 사용방법은 각 참가자 별로 제공된 channel값을 입력하여 실행합니다.

```
python3 client.py channel_name
```

만약 당신의 모델에 Moth Class를 직접 넣어 사용 할 것이라면 코드 내의 `__main__`을 참고하세요.

Unitree Go2를 사용하지 않는 참가자의 경우 아래의 표와 [Gamepad Tester](https://hardwaretester.com/gamepad) 링크를 참고하여 게임패드를 세팅 할 수 있습니다.

|표준|파라미터|값|
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

`video_moth.py`는 Unitree Go2의 정면 카메라를 사용하기 위한 GStreamer Pipeline이 정의되어있고, Class 내에는 15프레임의 이미지를 받아오는 getStream함수가 존재합니다.

Unitree Go2의 Multimedia service에 대한 설명은 [Unitree Development Guide - Multimedia Service](https://support.unitree.com/home/en/developer/Multimedia_Services)를 참고하세요.

`client.py`에서 `video_moth.py`에 있는 `Go2Camera` Class를 사용하여 영상을 웹에 실시간으로 송신합니다.

만약 당신의 로봇이 Unitree Go2가 아니라 다른 로봇을 사용하고 있다면 그에 맞추어 Class 속 `self.pipeline_cmd` 변수를 변경해주어야 합니다.

IEEE-QRC 페이지에서는 H.264 디코딩 방식을 사용하고 있습니다.

만약 당신이 카메라를 사용하지 않을 시 `client.py`내에서 Go2Camera를 사용하는 라인을 모두 삭제해주어야 합니다.(Line No.23, 24, 37, 40, 43, 44)

비디오의 스트리밍에 관련하여 기술적인 질문이나 요청사항이 있다면 jhs10429@teamgrit.kr로 문의 주시길 바랍니다.

### sensor_msg.py

`sensor_msg.py`는 Unitree Go2의 센서값들을 받아오기 위해 정의된 Class입니다.

`client.py`에서 `sensor_msg.py`에 저장되어있는 센서값들을 웹에 실시간으로 송신합니다.

`client.py`에서 `Moth.msg`에 `sensor_msg`를 사용하는 것을 볼 수 있습니다.

```
self.msg = smsg()
```

class내의 파라미터에 사용자가 직접 값을 입력해주면 그 값을 웹에서 받아 볼 수 있습니다.

- smsg 파라미터 값

|이름|타입|설명|
|:---:|:---:|:---:|
|Moth.msg.motor_state_q|float array[12]|현재 모터의 라디안 값 [Hip, Thigh, Calf]|
|Moth.msg.velocity|float array[3]|현재 속도 및 각속도 값 [Vx, Vy, ωz]|
|Moth.msg.RPY|float array[3]|현재 Roll, Pitch, Yaw 값|

경기가 진행되는동안 참가자가 웹으로 전송한 데이터를 파일로 저장하고, 모든 파일은 경기가 끝난후에 다운로드 할 수 있습니다.

추가적인 센서값 전송이 필요하다면 jhs10429@teamgrit.kr로 문의해주세요.
