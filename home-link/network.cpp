//
//  network.cpp
//  home-link
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "network.h"
#include <cpp-log/log.h>

using namespace link;

void connection::start(int port,
                       std::function<void()> start_callback,
                       std::function<void(network::message const&)> recv_callback,
                       const char*, const char*) {
	m_server.callback = recv_callback;
	m_server.start(port);
	if(start_callback) {
		start_callback();
	}
}

void connection::broadcast(int port, network::buffer const& data) {
	m_sender.broadcast(port, data);
}

void connection::send(network::ipv4_address address, int port, network::buffer const& data) {
	m_sender.send(address, port, data);
}

void connection::loop() {
}
