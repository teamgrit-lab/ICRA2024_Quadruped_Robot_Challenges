import numpy as np
import json
from datetime import datetime
import math

class smsg():
	def __init__(self):
		self.motor_state_q = np.zeros(12, dtype=float)
		self.RPY = np.zeros(3, dtype=float)
		self.velocity = np.zeros(3, dtype=float)
		self.capture = False
		self.type = "lab"
		self.timestamp = f"{datetime.now().time().hour}h {datetime.now().time().minute}m {datetime.now().time().second}.{datetime.now().time().microsecond}s"
		self.data = None

	def parse_msg(self, lx, ly, rx, ry, mode):
		self.timestamp = f"{datetime.now().time().hour}h {datetime.now().time().minute}m {datetime.now().time().second}.{datetime.now().time().microsecond}s"
		data = {
			"type": self.type,
			"data":{
				"timestamp": self.timestamp,
				"capture": self.capture,
				"RPY": {
					"Roll": self.RPY[0],
					"Pitch": self.RPY[1],
					"Yaw": self.RPY[2]
				},
				"control": {
					"lx": lx,
					"ly": ly,
					"rx": rx,
					"ry": ry,
					"mode": mode
				},
				"legAngles": {
					"RF": {
						"Hip": int(math.degrees(float(self.motor_state_q[0]))*100)/100,
						"Thigh": int(math.degrees(float(self.motor_state_q[1]))*100)/100,
						"Calf": int(math.degrees(float(self.motor_state_q[2]))*100)/100
					},
					"LF": {
						"Hip": int(math.degrees(float(self.motor_state_q[3]))*100)/100,
						"Thigh": int(math.degrees(float(self.motor_state_q[4]))*100)/100,
						"Calf": int(math.degrees(float(self.motor_state_q[5]))*100)/100
					},
					"RR": {
						"Hip": int(math.degrees(float(self.motor_state_q[6]))*100)/100,
						"Thigh": int(math.degrees(float(self.motor_state_q[7]))*100)/100,
						"Calf": int(math.degrees(float(self.motor_state_q[8]))*100)/100
					},
					"LR": {
						"Hip": int(math.degrees(float(self.motor_state_q[9]))*100)/100,
						"Thigh": int(math.degrees(float(self.motor_state_q[10]))*100)/100,
						"Calf": int(math.degrees(float(self.motor_state_q[11]))*100)/100
					}
				},
				"motionParameters": {
					"Vx": self.velocity[0],
					"Vy": self.velocity[1],
					"Wz": self.velocity[2]
				}
			}
		}
		self.data = bytes(json.dumps(data), "UTF-8")
		if self.capture == True:
			self.capture = False

if __name__ == "__main__":
	msg = smsg()
	msg.motor_state_q = [0.123, 0.234, 0.345, 0.456, 1.0, 1.1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.01]
	msg.RPY = [0.1, 0.2, 0.3]
	msg.velocity = [0.1, 0.2, 0.3]
	data = msg.parse_msg()
	dat = json.loads(msg.data)
	print(dat)
