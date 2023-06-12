//
//  encoder.h
//  home-link
//  ESP
//
//  Created by Brzeski, Piotr on 2018-10-06.
//  Copyright © 2018-2023 Brzeski.net. All rights reserved.
//

#pragma once
#include "switch.h"
#include <optional>

class Encoder
{
public:
	using Callback = Switch::Callback;

	Encoder(uint8_t pin_1_number, uint8_t pin_2_number);
	Encoder(uint8_t pin_1_number, uint8_t pin_2_number, uint8_t switch_pin_number);

	void set_callbacks(Callback left_callback, Callback right_callback);
	void set_shift_callbacks(Callback left_callback, Callback right_callback);
	void set_click_callback(Callback callback);

private:
	Encoder(const Encoder&) = delete;
	Encoder(Encoder&&) = delete;
	Encoder& operator=(const Encoder&) = delete;
	Encoder& operator=(Encoder&&) = delete;

	void update_switch();
	void update_encoder();

	enum class SwitchMode { None, Click, Shift };

	Switch                m_switch1;
	Switch                m_switch2;
	std::optional<Switch> m_switch;
	SwitchMode            m_switch_mode = SwitchMode::None;
	uint8_t               m_state;
	int                   m_position = 0;
	Callback              m_left_callback;
	Callback              m_right_callback;
	Callback              m_left_shift_callback;
	Callback              m_right_shift_callback;
	Callback              m_click_callback;
};
