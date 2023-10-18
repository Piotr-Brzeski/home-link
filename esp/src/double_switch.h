//
//  double_switch.h
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-10-18.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "switch.h"
#include "node.h"
#include <cpp-log/log.h>

#include "../../configuration.h"

class DoubleSwitch {
public:
	DoubleSwitch(std::uint8_t left_device, std::uint8_t right_device)
		: m_left_device_id{homelink::device_type::button, left_device}
		, m_right_device_id{homelink::device_type::button, right_device}
	{
	}

	void setup() {
		m_node.add(m_left_device_id);
		m_node.add(m_right_device_id);
		m_node.start(configuration::port, configuration::ssid, configuration::password);
		m_left_switch.set_click_callback([this](){
			logger::log("Left Click");
			m_node.send(m_left_device_id, homelink::state_type::event, homelink::state_value::click);
		});
		m_right_switch.set_click_callback([this](){
			logger::log("Right Click");
			m_node.send(m_right_device_id, homelink::state_type::event, homelink::state_value::click);
		});
		Switch::attach_interrupts();
	}

	void loop() {
		m_node.loop();
	}

private:
	Switch              m_left_switch = Switch(1);
	Switch              m_right_switch = Switch(2);
	homelink::device_id m_left_device_id;
	homelink::device_id m_right_device_id;
	homelink::node      m_node;
};
