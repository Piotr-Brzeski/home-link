//
//  network.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <cpp-network/server.h>

namespace homelink {

class connection {
public:
	void start(int port,
	           std::function<void()> start_callback,
	           std::function<void(network::message const&)> recv_callback,
	           const char* = nullptr, const char* = nullptr);
	void wait();

	void broadcast(int port, network::buffer const& data);
	void send(network::ipv4_address address, int port, network::buffer const& data);

	void loop();

private:
	network::udp_server m_server;
	network::udp_socket m_sender;
};

}
