/********************************************************************** Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/

#include "client.hpp"
#include <gst/gst.h>
#include <gst/app/app.h>

#include <unitree/robot/go2/obstacles_avoid/obstacles_avoid_client.hpp>


GstSample* global_sample;
uint16_t key = 0;
float lx = 0.0;
float ly = 0.0;
float rx = 0.0;
float ry = 0.0;
auto start = std::chrono::high_resolution_clock::now();
bool control_flag = true;
bool videoflag = false;
bool cp_control = false;
bool gst_flag_ = true;

std::string url;
json globalData;

std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

std::chrono::system_clock::time_point control_time = std::chrono::system_clock::now();

Custom::Custom(client* c, websocketpp::connection_hdl hdl) : c(c), hdl(hdl)
{
	sport_client.SetTimeout(10.0f);
	sport_client.Init();

	suber.reset(new unitree::robot::ChannelSubscriber<unitree_go::msg::dds_::SportModeState_>(TOPIC_HIGHSTATE));
	suber->InitChannel(std::bind(&Custom::HighStateHandler, this, std::placeholders::_1), 1);

	lowstate_subscriber.reset(new ChannelSubscriber<unitree_go::msg::dds_::LowState_>(TOPIC_LOWSTATE));
	lowstate_subscriber->InitChannel(std::bind(&Custom::LowStateMessageHandler, this, std::placeholders::_1), 1);
};

void Custom::GetInitState()
{
	px0 = state.position()[0];
	py0 = state.position()[1];
	yaw0 = state.imu_state().rpy()[2];
	std::cout << "initial position: x0: " << px0 << ", y0: " << py0 << ", yaw0: " << yaw0 << std::endl;
};

void Custom::MessageSend(client* c, websocketpp::connection_hdl hdl)
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> now_time = now - start_time;
	std::chrono::duration<double, std::milli> control_ = now - control_time;
	if (control_.count()/100 > 0.3){
		lx = 0.0;
		ly = 0.0;
		rx = 0.0;
		ry = 0.0;
		key = 0;
	}
	j["data"]["timestamp"] = now_time.count() / 1000;
	j["type"] = "lab";
	j["data"]["RPY"]["Roll"] = std::round(state.imu_state().rpy()[0]*180/3.1415926535 * 10)/10;
	j["data"]["RPY"]["Pitch"] = std::round(state.imu_state().rpy()[1]*180/3.1415926535 * 10)/10;
	j["data"]["RPY"]["Yaw"] = std::round(state.imu_state().rpy()[2]*180/3.1415926535 * 10)/10;
	j["data"]["control"] = globalData;
	j["data"]["motionParameters"]["Vx"] = state.velocity()[0];
	j["data"]["motionParameters"]["Vy"] = state.velocity()[1];
	j["data"]["motionParameters"]["Wz"] = state.velocity()[2];
	j["data"]["motionParameters"]["bodyHeight"] = state.body_height();
	j["data"]["motionParameters"]["footRaise"] = state.foot_raise_height();
	j["data"]["capture"] = cp_control;
	j["data"]["timestamp"] = now_time.count() / 1000;
	j["data"]["legAngles"]["RF"]["Hip"] = std::round(low_state.motor_state()[0].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["RF"]["Thigh"] = std::round(low_state.motor_state()[1].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["RF"]["Calf"] = std::round(low_state.motor_state()[2].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LF"]["Hip"] = std::round(low_state.motor_state()[3].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LF"]["Thigh"] = std::round(low_state.motor_state()[4].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LF"]["Calf"] = std::round(low_state.motor_state()[5].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["RR"]["Hip"] = std::round(low_state.motor_state()[6].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["RR"]["Thigh"] = std::round(low_state.motor_state()[7].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["RR"]["Calf"] = std::round(low_state.motor_state()[8].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LR"]["Hip"] = std::round(low_state.motor_state()[9].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LR"]["Thigh"] = std::round(low_state.motor_state()[10].q()*180/3.1415926535 * 10)/10;
	j["data"]["legAngles"]["LR"]["Calf"] = std::round(low_state.motor_state()[11].q()*180/3.1415926535 * 10)/10;
	std::string jsonString = j.dump();
	std::vector<uint8_t> qdata(jsonString.begin(), jsonString.end());
	c->send(hdl,qdata.data(),qdata.size(), websocketpp::frame::opcode::binary);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	if (cp_control == true){
		cp_control = false;
	}
}

void Custom::HighStateHandler(const void *message)
{
	state = *(unitree_go::msg::dds_::SportModeState_ *)message;
};

void Custom::LowStateMessageHandler(const void* message)
{
	low_state = *(unitree_go::msg::dds_::LowState_*)message;
}

void on_open(client* c, websocketpp::connection_hdl hdl){
	std::string msg = "";
	msg = "video/h264;width=1280;height=720;framerate=15;codecs=avc1.42002A";
	c->send(hdl, msg, websocketpp::frame::opcode::text);
	c->get_alog().write(websocketpp::log::alevel::app, "Sent Message: " + msg);
}

void on_fail(client* c, websocketpp::connection_hdl hdl){
	c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	control_time = std::chrono::system_clock::now();
	c->get_alog().write(websocketpp::log::alevel::app, "Received Reply: " + msg->get_payload());
	try{
		start = std::chrono::high_resolution_clock::now();
		json jsonData = json::parse(msg->get_payload());
		globalData = jsonData;
		std::string type = jsonData["type"];

		if (type == "capture"){
			cp_control = true;
		}

		lx = jsonData["direction"]["lx"];
		ly = jsonData["direction"]["ly"];
		rx = jsonData["direction"]["rx"];
		ry = jsonData["direction"]["ry"];
		key = jsonData["direction"]["mode"];

	} catch (...){
		std::cerr << "Not JSON" << std::endl;
	}
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
	c->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
}

GstFlowReturn on_new_sample(GstAppSink *appsink, gpointer user_data){
	global_sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
	if (global_sample != NULL){
		GstBuffer *buffer = gst_sample_get_buffer(global_sample);
		videoflag = true;
	}
	return GST_FLOW_OK;
}

std::future<void> asyncVideo(client* c, websocketpp::connection_hdl hdl) {
	return std::async(std::launch::async, [c, hdl]() {
		if (c->get_con_from_hdl(hdl)){
			start_time = std::chrono::system_clock::now();
			while (true) {
				if (videoflag){
					try {
						GstBuffer *buffer = gst_sample_get_buffer(global_sample);
						GstMapInfo map;
						if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
							c->send(hdl, map.data, map.size, websocketpp::frame::opcode::binary);
						}
					}catch (const websocketpp::exception& e) {
						std::cerr << "Err: " << e.what() << std::endl;
					}
					videoflag = false;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}
	});
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
		exit(-1);
	}

	unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);

	unitree::robot::go2::ObstaclesAvoidClient obstacles_avoid_client;
	obstacles_avoid_client.Init();
	sleep(1);
	obstacles_avoid_client.SwitchSet(0);

	ChannelPublisher<unitree_go::msg::dds_::WirelessController_> publisher(TOPIC_JOYSTICK);
	publisher.InitChannel();

	gst_init(&argc, &argv);
	const char *pipeline_cmd = "";
	pipeline_cmd = "udpsrc address=230.1.1.1 port=1720 multicast-iface=enp130s0 ! queue ! application/x-rtp, media=video, encoding-name=H264 ! rtph264depay ! h264parse config-interval=1 ! video/x-h264, stream-format=byte-stream, alignment=au ! queue leaky=2 ! appsink name=moth sync=false max-buffers=2 emit-signals=true drop=true";
	GstElement *pipeline = gst_parse_launch(pipeline_cmd, NULL);
	GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "moth");
	g_signal_connect(appsink, "new-sample", G_CALLBACK(on_new_sample), NULL);
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	std::cout << "arg2: " << argv[2] << std::endl;
	std::ostringstream oss;
	oss << "ws://ieee-qrc.agilertc.com:8276/pang/ws/pub?channel=" << argv[2] << "&track=video&mode=bundle";
	url = oss.str();
	client c;
	try{
		c.clear_access_channels(websocketpp::log::alevel::frame_header);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);

		c.init_asio();

		c.set_open_handler(bind(&on_open, &c, ::_1));
		c.set_fail_handler(bind(&on_fail, &c, ::_1));
		c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
		c.set_close_handler(bind(&on_close, &c, ::_1));

		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(url, ec);
		std::cout << "url: " << url << std::endl;
		url.clear();
		if (!con) {
			std::cerr << "Failed to create connection" << std::endl;
		}
		try{
			c.connect(con);
		}catch (const websocketpp::exception& e){
			std::cerr << "Websocket exception: " << e.what() << std::endl;
		}catch (const std::exception& e){
			std::cerr << "standard exception: " << e.what() << std::endl;
		}

		std::thread asyncThreadVideo([&c, con]() {
			auto asyncResultVideo = asyncVideo(&c, con);
		});

		std::thread asyncC([&c]() {
			c.run();
		});

		sleep(1); // Wait for 1 second to obtain a stable state

		Custom custom(&c, con);

		sleep(1); // Wait for 1 second to obtain a stable state

		custom.GetInitState(); // Get initial position

		unitree::common::ThreadPtr threadPtr = unitree::common::CreateRecurrentThread(custom.dt * 1000000, std::bind(&Custom::MessageSend, &custom, &c, con));

		while (true)
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = end - start;
			if(elapsed.count() < 1.0){
				unitree_go::msg::dds_::WirelessController_ msg;
				msg.lx() = lx;
				msg.ly() = ly;
				msg.rx() = rx;
				msg.ry() = ry;
				msg.keys() = key;
				publisher.Write(msg);
				if (control_flag == false){
					control_flag = true;
				}
				sleep(0.1);
			}
			else{
				if (control_flag == true){
					unitree_go::msg::dds_::WirelessController_ msg;
					msg.lx() = 0.0;
					msg.ly() = 0.0;
					msg.rx() = 0.0;
					msg.ry() = 0.0;
					msg.keys() = 0;
					publisher.Write(msg);
					control_flag = false;
				}
				sleep(0.01);
			}
		}

	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "Other Exception!!" << std::endl;
	}


	return 0;
}


