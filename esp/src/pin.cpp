//
//  pin.cpp
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2017-08-19.
//  Copyright © 2017-2023 Brzeski.net. All rights reserved.
//

#include "pin.h"
#include <Arduino.h>

void PinClass::set(PinMode mode) {
  switch(mode) {
    case PinMode::Output:
      pinMode(m_address, OUTPUT);
      break;
    case PinMode::DigitalInPullup:
      pinMode(m_address, INPUT_PULLUP);
      break;
    default:
      assert(false);
      return;
  }
  m_mode = mode;
}

bool PinClass::read() const {
	auto state = digitalRead(m_address);
	return state == LOW;
}

void PinClass::write(bool digital_value) {
  if(m_mode != PinMode::Output) {
    assert(m_mode == PinMode::None);
    if(m_mode == PinMode::None) {
      set(PinMode::Output);
    }
  }
  digitalWrite(m_address, digital_value ? LOW : HIGH);
}

void PinClass::write(int analog_value) {
  constexpr int max_value = 255;
  if(m_mode != PinMode::Output) {
    assert(m_mode == PinMode::None);
    if(m_mode == PinMode::None) {
      set(PinMode::Output);
    }
  }
  if(analog_value < 0) {
    analog_value = 0;
  }
  else if(analog_value > max_value) {
    analog_value = max_value;
  }
  analogWrite(m_address, max_value - analog_value);
}

PinClass& Pin(std::size_t pin_number) {
  // https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
  static std::array<PinClass, NumberOfPins> pins {{
    PinClass(16), // D0 - Don't use (Does not support DigitalInPullup nor Interrupts)
    PinClass(5),  // D1
    PinClass(4),  // D2
    PinClass(0),  // D3
    PinClass(2),  // D4 - Builtin LED (LOW = On, HIGH = Off)
    PinClass(14), // D5
    PinClass(12), // D6
    PinClass(13), // D7
    PinClass(15), // D8
    PinClass(3),  // RX - Serial
    PinClass(1)   // TX - Serial
  }};
  assert(pin_number < pins.size());
  return pins[pin_number];
}
