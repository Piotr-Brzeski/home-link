//
//  pin.h
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2017-08-19.
//  Copyright © 2017-2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <array>
#include <cassert>

static constexpr uint8_t NumberOfPins = 11;

enum class PinMode { None, Output, DigitalInPullup, /*, OneWire, DigitalIn*/ };

class PinClass
{
public:
  explicit PinClass(uint8_t address)
    : m_address(address)
  {
  }
  
  PinClass(PinClass&&) = default;
  PinClass& operator=(PinClass&&) = default;
  
  void set(PinMode mode);
  
  bool read() const;
  void write(bool digital_value);
  // 0...255
  void write(int analog_value);
  
  uint8_t address() const {
    return m_address;
  }
  
private:
  PinClass(const PinClass&) = delete;
  PinClass& operator=(const PinClass&) = delete;
  
  uint8_t const m_address;
  PinMode m_mode = PinMode::None;
};  //  class PinClass

PinClass& Pin(std::size_t pin_number);


class BuiltInLed
{
public:
  static void On() {
    Pin(4).write(true);
  }

  static void Off() {
    Pin(4).write(false);
  }
};  //  class BuiltInLed
