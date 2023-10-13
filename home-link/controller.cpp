//
//  controller.cpp
//  home-link
//
//  Created by Piotr Brzeski on 2023-04-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "controller.h"
#include "message.h"
#include <cpp-log/log.h>

using namespace homelink;

template<>
void controller::handle(advertisement const &message, network::ipv4_address source) {
	if(message.get_source() == advertisement::source::node) {
		auto const node_advertisement = advertisement(advertisement::source::controller);
		send(source, node_advertisement);
	}
}

template<>
void controller::handle(list const &message, network::ipv4_address source) {
	auto response = accept();
	for(auto device_id : message) {
		if(m_devices.contains(device_id)) {
			response.add(device_id);
			m_devices[device_id] = source;
		}
	}
	if(response.size() > 0) {
		send(source, response);
	}
}

template<>
void controller::handle(state const &message, network::ipv4_address source) {
	for(auto device_state : message) {
		auto operation_it = m_operations.find(device_state);
		if(operation_it != m_operations.end()) {
			for(auto& operation : operation_it->second) {
				operation();
			}
		}
	}
}

template<typename T>
void controller::handle(T const& message, network::ipv4_address source) {
	logger::log("Ignoring unknown message from " + network::ipv4_str(source));
}

void controller::add(device_id device_id) {
	m_devices[device_id] = network::invalid_ipv4_address;
}

void controller::add(device_state device_state, operation operation) {
	add(device_state.type.device);
	m_operations[device_state].push_back(operation);
}

void controller::start(int port) {
	m_port = port;
	m_network.start(m_port, [this](){
		logger::log("NET STARTED");
		// Send adv(controller)
		auto const node_advertisement = advertisement(advertisement::source::controller);
		broadcast(node_advertisement);
	}, [this](network::message const& network_message){
		logger::log("recv: " + description(network_message.content) + " from " + network::ipv4_str(network_message.source));
		auto const& message = get_message(network_message.content);
		if(!message) {
			logger::log("Invalid message");
			return;
		}
		std::visit([this, source = network_message.source](auto&& message){
			handle(message, source);
		}, *message);
	});
}

void controller::wait() {
	m_network.wait();
}

void controller::broadcast(message const& message) {
	logger::log("broadcast: " + message.description());
	m_network.broadcast(m_port, message.get_content());
}

void controller::send(network::ipv4_address address, message const& message) {
	logger::log("send: " + (message.description() + " to: ") + network::ipv4_str(address));
	m_network.send(address, m_port, message.get_content());
}
