#ifndef __TG_MOTH_CLIENT_HPP__
#define __TG_MOTH_CLIENT_HPP__


#include <cmath>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <chrono>
#include "json.hpp"
#include <string>
#include <mutex>

#include <unitree/robot/go2/sport/sport_client.hpp>
#include <unitree/robot/channel/channel_subscriber.hpp>
#include <unitree/idl/go2/SportModeState_.hpp>

#include <unitree/robot/channel/channel_publisher.hpp>
#include <unitree/idl/go2/WirelessController_.hpp>

#include <unitree/idl/go2/LowState_.hpp>

#include <thread>
#include <functional>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <stdint.h>



#define TOPIC_HIGHSTATE "rt/sportmodestate"
#define TOPIC_LOWSTATE "rt/lowstate"
#define TOPIC_JOYSTICK "rt/wirelesscontroller"
#define TOPIC_CLOUD "rt/utlidar/cloud"

using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace unitree::robot;
using namespace unitree::common;

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class Custom {
public:
	Custom(client* c, websocketpp::connection_hdl hdl);
	void GetInitState();
	void HighStateHandler(const void *message);
	void LowStateMessageHandler(const void* message);
	void MessageSend(client* c, websocketpp::connection_hdl hdl);
	float dt = 0.005;      // 控制步长0.001~0.01

private:
	unitree_go::msg::dds_::SportModeState_ state;
	unitree::robot::go2::SportClient sport_client;
	unitree::robot::ChannelSubscriberPtr<unitree_go::msg::dds_::SportModeState_> suber;

	double px0, py0, yaw0; // 初始状态的位置和偏航
	double ct = 0;         // 运行时间
	int flag = 0;          // 特殊动作执行标志
//	float dt = 0.005;      // 控制步长0.001~0.01
	ChannelSubscriberPtr<unitree_go::msg::dds_::LowState_> lowstate_subscriber;

	unitree_go::msg::dds_::LowState_ low_state{};

	client* c;
	websocketpp::connection_hdl hdl;
	json j;
	std::mutex mtx;
};

void on_open(client* c, websocketpp::connection_hdl hdl);
void on_fail(client* c, websocketpp::connection_hdl hdl);
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
void on_close(client* c, websocketpp::connection_hdl hdl);

//GstFlowReturn on_new_sample(GstAppSink *appsink, gpointer user_data);
//std::future<void> asyncVideo(client*c, websocketpp::connection_hdl hdl);

#endif
