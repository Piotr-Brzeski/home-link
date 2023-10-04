//
//  encoder.cpp
//  home-link
//  ESP
//
//  Created by Brzeski, Piotr on 2018-10-06.
//  Copyright © 2018-2023 Brzeski.net. All rights reserved.
//


#include "encoder.h"

#include <Arduino.h>

Encoder::Encoder(uint8_t pin_1_number, uint8_t pin_2_number)
	: m_switch1(pin_1_number)
	, m_switch2(pin_2_number)
	, m_state((m_switch1.get_state() ? 1 : 0) + (m_switch2.get_state() ? 2 : 0))
{
	m_switch1.set_state_change_callback([this](){ update_encoder(); });
	m_switch2.set_state_change_callback([this](){ update_encoder(); });
}

Encoder::Encoder(uint8_t pin_1_number, uint8_t pin_2_number, uint8_t switch_pin_number)
	: m_switch1(pin_1_number)
	, m_switch2(pin_2_number)
	, m_switch(std::in_place, switch_pin_number)
	, m_state((m_switch1.get_state() ? 1 : 0) + (m_switch2.get_state() ? 2 : 0))
{
	m_switch1.set_state_change_callback([this](){ update_encoder(); });
	m_switch2.set_state_change_callback([this](){ update_encoder(); });
	m_switch->set_state_change_callback([this](){ update_switch(); });
}

void Encoder::set_callbacks(Callback left_callback, Callback right_callback)
{
	m_left_callback = left_callback;
	m_right_callback = right_callback;
}

void Encoder::set_alt_callbacks(Callback left_callback, Callback right_callback)
{
	m_left_alt_callback = left_callback;
	m_right_alt_callback = right_callback;
}

void Encoder::set_click_callback(Callback callback)
{
	m_click_callback = callback;
}

void Encoder::update_switch()
{
	if(m_switch->get_state()) {
		m_switch_mode = SwitchMode::Click;
	}
	else {
		if(m_switch_mode == SwitchMode::Click && m_click_callback) {
			m_click_callback();
		}
		m_switch_mode = SwitchMode::None;
	}
}

void Encoder::update_encoder()
{
	uint8_t state = m_state;
	if(m_switch1.get_state()) {
		state += 4;
	}
	if(m_switch2.get_state()) {
		state += 8;
	}
	m_state = state >> 2;

	switch(state) {
		case 1: case 7: case 8: case 14:
			++m_position;
			break;
		case 3: case 12:
				m_position += 2;
				break;
		case 2: case 4: case 11: case 13:
			--m_position;
			break;
		case 6: case 9:
			m_position -= 2;
			break;
		default:
			break;
	}
	Callback* callback = nullptr;
	if(m_switch_mode == SwitchMode::None) {
		if(m_position > 0 && m_right_callback) {
			callback = &m_right_callback;
		}
		else if(m_position < 0 && m_left_callback) {
			callback = &m_left_callback;
		}
	}
	else {
		if(m_position > 0 && m_right_alt_callback) {
			callback = &m_right_alt_callback;
		}
		else if(m_position < 0 && m_left_alt_callback) {
			callback = &m_left_alt_callback;
		}
		m_switch_mode = SwitchMode::Shift;
	}
	if(callback) {
		while(m_position >= 4) {
			(*callback)();
			m_position -= 4;
		}
		while(m_position <= -4) {
			(*callback)();
			m_position += 4;
		}
	}
}
