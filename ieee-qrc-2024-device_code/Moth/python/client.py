import sys
import asyncio
import websockets
import json
import time
import threading
from video_moth import *
from sensor_msg import *
import numpy as np


class Moth():
	def __init__(self, channel):
		self.url = f"ws://ieee-qrc.agilertc.com:8276/pang/ws/pub?channel={channel}&track=video&mode=bundle"
		self.ws = None
		self.lx = 0.0
		self.ly = 0.0
		self.rx = 0.0
		self.ry = 0.0
		self.keys = 0
		self.mode = np.zeros(16, dtype=int)
		self.data = [self.lx, self.ly, self.rx, self.ry, self.mode]
		self.cam = Go2Camera()
		self.cam.start()
		self.control_time = None
		self.msg = smsg()

	def decimal_to_binary_array(self, decimal_number):
		binary_string = bin(decimal_number)[2:]
		binary_array = [int(bit) for bit in binary_string]
		while len(binary_array) < 16:
			binary_array.insert(0, 0)
		return binary_array[::-1]

	async def send(self):
		t = time.time()
		await self.ws.send(self.cam.mime)
		self.control_time = time.time()
		while True:
			video_data = self.cam.getStream()
			self.msg.parse_msg(self.lx, self.ly, self.rx, self.ry, self.keys)
			await self.ws.send(self.msg.data)
			if video_data:
				await self.ws.send(video_data)
			if time.time() - t > 10:
				t = time.time()
				await self.ws.send(b"ping")
			if time.time() - self.control_time > 0.2 and time.time() - self.control_time < 5:
				self.lx = 0.0
				self.ly = 0.0
				self.rx = 0.0
				self.ry = 0.0
				self.keys = 0
				self.mode = np.zeros(16, dtype=int)
				self.data = [0.0, 0.0, 0.0, 0.0, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
			await asyncio.sleep(0.01)

	async def recv(self):
		self.control_time = time.time()
		while True:
			data = await self.ws.recv()
			self.control_time = time.time()
			try:
				jsonObject = json.loads(data)
				type = jsonObject.get("type")
				if type == "capture":
					self.msg.capture = True
				direction = jsonObject.get("direction")
				if direction:
					self.lx = direction.get("lx")
					self.ly = direction.get("ly")
					self.rx = direction.get("rx")
					self.ry = direction.get("ry")
					self.keys = int(direction.get("mode"))
					self.mode = self.decimal_to_binary_array(self.keys)
					self.data = [float(self.lx), float(self.ly), float(self.rx), float(self.ry), self.mode]
			except:
				pass

	async def main(self):
		async with websockets.connect(self.url, ping_timeout=None) as self.ws:
			t1 = asyncio.create_task(self.recv())
			t2 = asyncio.create_task(self.send())
			await asyncio.gather(t1, t2)

def run_moth():
	asyncio.run(moth.main())

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print('There was no channel name -> python3 client.py "channel_name"')
		sys.exit()
	moth = Moth(f"{sys.argv[1]}")
	threading.Thread(target=run_moth).start()

	while True:
		time.sleep(0.01)
		print("lx: ", moth.data[0], ", ly: ", moth.data[1], ", rx: ", moth.data[2], ", ry: ", moth.data[3], ", keys: ", moth.data[4])
