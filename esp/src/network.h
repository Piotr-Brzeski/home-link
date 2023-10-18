//
//  network.h
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string>
#include <functional>

#include <stdexcept>

namespace network {

using buffer = std::basic_string<std::uint8_t>;
using ipv4_address = ::IPAddress;

static const auto invalid_ipv4_address = ::IPAddress();

struct message {
	ipv4_address source;
	buffer       content;
};

inline std::string ipv4_str(ipv4_address const& address) {
	return address.toString().c_str();
}

}

namespace homelink {

class connection {
public:
  void start(int port,
             std::function<void()> start_callback,
             std::function<void(network::message const&)> recv_callback,
             const char* ssid, const char* password);

  void broadcast(int port, network::buffer const& data);
  void send(network::ipv4_address address, int port, network::buffer const& data);

  void loop();

private:
  WiFiUDP                                      m_udp;
  network::message                             m_message;
  std::function<void(network::message const&)> m_recv_callback;
};

class exception : public std::runtime_error {
public:
	explicit exception(char const* message)
		: std::runtime_error(message) {
	}
};

}
