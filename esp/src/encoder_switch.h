//
//  encoder_switch.h
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-10-18.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "encoder.h"
#include "node.h"
#include <cpp-log/log.h>

#include "../../configuration.h"

class EncoderSwitch {
public:
	EncoderSwitch(std::uint8_t device)
		: m_device_id{homelink::device_type::button, device}
	{
	}

	void setup() {
		m_node.add(m_device_id);
		m_node.start(configuration::port, configuration::ssid, configuration::password);
		m_encoder.set_click_callback([this](){
			logger::log("Click");
			m_node.send(m_device_id, homelink::state_type::event, homelink::state_value::click);
		});
		m_encoder.set_callbacks([this](){
			logger::log("Minus");
			m_node.send(m_device_id, homelink::state_type::event, homelink::state_value::minus);
		}, [this](){
			logger::log("Plus");
			m_node.send(m_device_id, homelink::state_type::event, homelink::state_value::plus);
		});
		m_encoder.set_alt_callbacks([this](){
			logger::log("Alt Minus");
			m_node.send(m_device_id, homelink::state_type::event, homelink::state_value::alt_minus);
		}, [this](){
			logger::log("Alt Plus");
			m_node.send(m_device_id, homelink::state_type::event, homelink::state_value::alt_plus);
		});
		Switch::attach_interrupts();
	}

	void loop() {
		m_node.loop();
	}

private:
	Encoder             m_encoder = Encoder(3, 2, 1);
	homelink::device_id m_device_id;
	homelink::node      m_node;
};
