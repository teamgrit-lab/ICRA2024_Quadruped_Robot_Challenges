import gi
gi.require_version("Gst", "1.0")
from gi.repository import Gst

Gst.init(None)

class Go2Camera():
	def __init__(self):
		self.pipeline_cmd = "udpsrc address=230.1.1.1 port=1720 multicast-iface=enp130s0 ! application/x-rtp, media=video, encoding-name=H264 ! rtph264depay ! h264parse config-interval=1 ! video/x-h264, alignment=au, stream-format=byte-stream ! appsink name=sink sync=false drop=true emit-signals=true max-buffers=2"
		self.pipeline = Gst.parse_launch(self.pipeline_cmd)
		self.sink = self.pipeline.get_by_name("sink")
		self.mime = "video/h264;width=1280;height=720;framerate=15;codecs=avc1.42002A"

	def start(self):
		print('start Video Stream')
		self.pipeline.set_state(Gst.State.PLAYING)

	def stop(self):
		print('stop Video Stream')
		self.pipeline.set_state(Gst.State.NULL)

	def getStream(self):
		sample = self.sink.emit("pull-sample")
		if sample:
			buf = sample.get_buffer()
			stream = buf.extract_dup(0, buf.get_size())
			return stream

		else:
			return False

if __name__ == "__main__":
	cam = Go2Camera()
	cam.start()
	while True:
		video_data = cam.getStream()
		if video_data:
			print(video_data[0:11])
