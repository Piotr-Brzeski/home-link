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

auto encoder = Encoder(2, 3, 1);
auto switch_device_id = link::device_id{link::device_type::button, 1};
auto node = link::node();

void setup() {
	Serial.begin(9600);
	delay(1000);
	log("START");
	delay(500);
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
	Switch::attach_interrupts();
}

void loop() {
	node.loop();
}
