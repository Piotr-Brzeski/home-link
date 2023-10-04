//
//  node.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "map.h"
#include "types.h"
#include "network.h"

namespace homelink {

class message;

class node {
public:
	void add(device_id device_id);
	void set(device_id device_id, state_type state_type, state_value state_value);
	void send(device_id device_id, state_type state_type, state_value state_value);
	
	void start(int port, const char* ssid = nullptr, const char* password = nullptr);
	void loop();
	
private:
	constexpr static std::uint8_t max_number_of_devices = 4;
	
	template<typename T>
	void handle(T const& message, network::ipv4_address source);
	
	void broadcast(message const& message);
	void send(network::ipv4_address address, message const& message);
	
	connection                                                   m_network;
	int                                                          m_port = -1;
	map<device_id, network::ipv4_address, max_number_of_devices> m_devices;
	map<device_state_type, state_value, 2*max_number_of_devices> m_device_states;
};

}
