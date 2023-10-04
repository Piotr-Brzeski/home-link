//
//  main.cpp
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "pin.h"
#include "encoder.h"
#include "node.h"
#include "logger.h"
#include <Arduino.h>

#include "../../configuration.h"

constexpr std::uint8_t livingroom = 1;
constexpr std::uint8_t bathroom1 = 2;
constexpr std::uint8_t kitchen = 3;

auto encoder = Encoder(3, 2, 1);
auto switch_device_id = link::device_id{link::device_type::button, kitchen};
auto node = link::node();

void setup() {
	Serial.begin(9600);
	delay(10);
	log("START");
	delay(10);
	node.add(switch_device_id);
	node.start(configuration::port, configuration::ssid, configuration::password);
	delay(10);
	encoder.set_click_callback([&](){
		log("Click");
		node.send(switch_device_id, link::state_type::event, link::state_value::click);
	});
	encoder.set_callbacks([&](){
		log("Minus");
		node.send(switch_device_id, link::state_type::event, link::state_value::minus);
	}, [&](){
		log("Plus");
		node.send(switch_device_id, link::state_type::event, link::state_value::plus);
	});
	encoder.set_alt_callbacks([&](){
		log("Alt Minus");
		node.send(switch_device_id, link::state_type::event, link::state_value::alt_minus);
	}, [&](){
		log("Alt Plus");
		node.send(switch_device_id, link::state_type::event, link::state_value::alt_plus);
	});
	Switch::attach_interrupts();
}

void loop() {
	node.loop();
}
