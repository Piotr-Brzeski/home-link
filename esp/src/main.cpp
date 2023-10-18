//
//  main.cpp
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

// #include "encoder_switch.h"
#include "double_switch.h"
#include "cpp-log/log.h"
#include <Arduino.h>

// Encoders
constexpr std::uint8_t livingroom = 1;
constexpr std::uint8_t bathroom1 = 2;
constexpr std::uint8_t kitchen = 3;
constexpr std::uint8_t office = 4;
// Double switches
constexpr std::uint8_t hall_left = 5;
constexpr std::uint8_t hall_right = 6;

// auto device = EncoderSwitch(office);
auto device = DoubleSwitch(hall_left, hall_right);

void setup() {
	Serial.begin(9600);
	delay(10);
	logger::log("START");
	device.setup();	
}

void loop() {
	device.loop();
}
