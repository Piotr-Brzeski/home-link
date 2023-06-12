//
//  switch.cpp
//  home-link
//  ESP
//
//  Created by Brzeski, Piotr on 2018-03-28.
//  Copyright © 2018-2023 Brzeski.net. All rights reserved.
//

#include "switch.h"
#include "pin.h"
#include <Arduino.h>
#include <Ticker.h>
#include <memory>

namespace
{

template<class T>
class Vector
{
public:
	Vector()
		: m_capacity(2)
		, m_size(0)
		, m_data(new T[m_capacity])
	{
	}

	T& operator[](std::size_t i) {
		return m_data[i];
	}

	void push_back(const T& t) {
		if(m_capacity < m_size) {
			m_capacity += 2;
			std::unique_ptr<T[]> new_data(new T[m_capacity]);
			for(std::size_t i = 0; i < m_size; ++i) {
				new_data[i] = std::move(m_data[i]);
			}
			m_data = std::move(new_data);
		}
		m_data[m_size] = t;
		++m_size;
	}

	std::size_t size() const {
		return m_size;
	}

	bool empty() const {
		return m_size == 0;
	}

	T* begin() const {
		return m_data.get();
	}

	T* end() const {
		return begin() + m_size;
	}

private:
	std::size_t           m_capacity;
	std::size_t           m_size;
	std::unique_ptr<T[]>  m_data;
};

Vector<Switch*>& switches_for_pin(uint8_t pin_number) {
	static std::array<Vector<Switch*>, NumberOfPins> switches;
	return switches[pin_number];
}

template<uint8_t pin_number>
void interrupt_function() {
	static uint16_t raw_state_history = 0;
	static bool raw_state = false;
	bool current_raw_state = Pin(pin_number).read();
	raw_state_history <<= 1;
	raw_state_history |= (current_raw_state ? 1 : 0);
	bool changed = false;
	if(raw_state) {
		if((raw_state_history & 0xF00F) == 0xF000) {
			raw_state = false;
			raw_state_history = 0x0000;
			changed = true;
		}
	}
	else {
		if((raw_state_history & 0xF00F) == 0x000F) {
			raw_state = true;
			raw_state_history = 0xFFFF;
			changed = true;
		}
	}
	if(changed) {
		auto &input_switches = switches_for_pin(pin_number);
		for(std::size_t i = 0; i < input_switches.size(); ++i) {
			input_switches[i]->interrupt(raw_state);
		}
	}
}

std::array<void(*)(), NumberOfPins> interrupt_functions = {
	&interrupt_function<0>,
	&interrupt_function<1>,
	&interrupt_function<2>,
	&interrupt_function<3>,
	&interrupt_function<4>,
	&interrupt_function<5>,
	&interrupt_function<6>,
	&interrupt_function<7>,
	&interrupt_function<8>,
	&interrupt_function<9>,
	&interrupt_function<10>
};

} //  namespace

Switch::Switch(uint8_t pin_number, bool pressed_state)
	: m_pin(pin_number)
	, m_pressed_state(pressed_state)
	, m_current_state(Pin(pin_number).read() == pressed_state)
{
	switches_for_pin(pin_number).push_back(this);
}

void Switch::set_click_callback(Callback callback)
{
	m_click_callback = callback;
}

void Switch::set_state_change_callback(Callback callback)
{
	m_state_change_callback = callback;
}

void Switch::interrupt(bool current_raw_state)
{
	bool new_state = current_raw_state == m_pressed_state;
	if(new_state != m_current_state) {
		m_current_state = new_state;
		if(m_state_change_callback) {
			m_state_change_callback();
		}
		if(m_click_callback && !m_current_state) {
			m_click_callback();
		}
	}
}

void Switch::attach_interrupts()
{
	static Ticker ticker;
	static Vector<void(*)()> functions;
	for(uint8_t pin_number = 0; pin_number < NumberOfPins; ++pin_number) {
		auto& input_switches = switches_for_pin(pin_number);
		if(!input_switches.empty()) {
			auto& pin = Pin(pin_number);
			pin.set(PinMode::DigitalInPullup);
			functions.push_back(interrupt_functions[pin_number]);
		}
	}
	ticker.attach_ms(1, [](){
		for(auto function : functions) {
			function();
		}
	});
}
