//
//  controller.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-04-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "types.h"
#include "network.h"
#include <map>
#include <vector>
#include <functional>

namespace homelink {

class message;

class controller {
public:
	using operation = std::function<void()>;
	
	void add(device_id device_id);
	void add(device_state device_state, operation operation);
	
	void start(int port);
	
private:
	using operations = std::vector<operation>;
	
	template<typename T>
	void handle(T const& message, network::ipv4_address source);
	
	void broadcast(message const& message);
	void send(network::ipv4_address address, message const& message);
	
	connection                                 m_network;
	int                                        m_port = -1;
	std::map<device_id, network::ipv4_address> m_devices;
	std::map<device_state, operations>         m_operations;
};

}
