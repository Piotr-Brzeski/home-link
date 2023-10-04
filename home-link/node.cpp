//
//  node.cpp
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "node.h"
#include "message.h"
#include "logger.h"

using namespace homelink;

template<>
void node::handle(advertisement const &message, network::ipv4_address source) {
	if(message.get_source() == advertisement::source::controller) {
		auto response = list();
		for(auto& device : m_devices) {
			response.add(device.first);
		}
		send(source, response);
	}
}

template<>
void node::handle(accept const &message, network::ipv4_address source) {
	for(auto device_id : message) {
		m_devices[device_id] = source;
	}
}

//template<>
//void node::handle(get_state const &message, network::ipv4_address source) {
//}

template<typename T>
void node::handle(T const& message, network::ipv4_address source) {
	log("Ignoring unknown message from " + network::ipv4_str(source));
}

void node::add(device_id device_id) {
	m_devices[device_id] = network::invalid_ipv4_address;
}

void node::set(device_id device_id, state_type state_type, state_value state_value) {
	send(device_id, state_type, state_value);
	if(state_type != state_type::event) {
		m_device_states[{device_id, state_type}] = state_value;
	}
}

void node::send(device_id device_id, state_type state_type, state_value state_value) {
	auto& controller_address = m_devices[device_id];
	if(controller_address) {
		auto update = state();
		update.add(device_id, state_type, state_value);
		send(controller_address, update);
	}
}

void node::start(int port, const char* ssid, const char* password) {
	m_port = port;
	m_network.start(m_port, [this](){
		log("NET STARTED");
		// Send adv(node)
		auto const node_advertisement = advertisement(advertisement::source::node);
		broadcast(node_advertisement);
	}, [this](network::message const& network_message) {
		log("recv: " + description(network_message.content) + " from " + network::ipv4_str(network_message.source));
		auto const& message = get_message(network_message.content);
		if(!message) {
			log("Invalid message");
			return;
		}
		std::visit([this, source = network_message.source](auto&& message){
			handle(message, source);
		}, *message);
	},
	ssid, password);
}

void node::loop() {
	m_network.loop();
}

void node::broadcast(message const& message) {
	log("broadcast: " + message.description());
	m_network.broadcast(m_port, message.get_content());
}

void node::send(network::ipv4_address address, message const& message) {
	log("send: " + (message.description() + " to: ") + network::ipv4_str(address));
	m_network.send(address, m_port, message.get_content());
}
