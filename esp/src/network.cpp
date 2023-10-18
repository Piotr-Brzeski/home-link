//
//  network.cpp
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "network.h"
#include <cpp-log/log.h>

using namespace homelink;

void connection::start(int port,
                       std::function<void()> start_callback,
                       std::function<void(network::message const&)> recv_callback,
                       const char* ssid, const char* password) {
  // WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  logger::log(WiFi.localIP());
  logger::log("WiFi connected."); 
  // UDP
  auto result = m_udp.begin(port);
	if(result != 1) {
		throw exception("UDP begin failed.");
	}
  // Callback
  if(start_callback) {
    start_callback();
  }
  m_recv_callback = recv_callback;
}

void connection::broadcast(int port, network::buffer const& data) {
  send(IP4_ADDR_BROADCAST, port, data);
}

void connection::send(network::ipv4_address address, int port, network::buffer const& data) {
	auto result = m_udp.beginPacket(address, port);
	if(result != 1) {
		throw exception("UDP beginPacket failed.");
	}
	auto size = m_udp.write(data.data(), data.size());
	if(size != data.size()) {
		throw exception("UDP write failed.");
	}
	result = m_udp.endPacket();
	if(result != 1) {
		throw exception("UDP endPacket failed.");
	}
}

void connection::loop() {
	auto size = m_udp.parsePacket();
	if(size == 0) {
		return;
	}
	m_message.content.resize(size);
	auto result = m_udp.read(m_message.content.data(), size);
	if(result != size) {
		throw exception("UDP read failed.");
	}
	m_message.source = m_udp.remoteIP();
  if(m_recv_callback) {
    m_recv_callback(m_message);
  }
}
