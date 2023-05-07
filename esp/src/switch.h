//
//  switch.h
//  home-link
//  ESP
//
//  Created by Brzeski, Piotr on 2018-03-28.
//  Copyright © 2018-2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <cstdint>
#include <functional>

class Switch
{
public:
  using Callback = std::function<void()>;
  
  Switch(uint8_t pin_number, bool pressed_state = true);
  
  void set_state_change_callback(Callback callback);
  void set_click_callback(Callback callback);
  bool get_state() const {
    return m_current_state;
  }
  void interrupt(bool current_raw_state);
  
  static void attach_interrupts();
  
private:
  Switch(const Switch&) = delete;
  Switch(Switch&&) = delete;
  Switch& operator=(const Switch&) = delete;
  Switch& operator=(Switch&&) = delete;
  
  const uint8_t       m_pin;
  const bool          m_pressed_state;
  bool                m_current_state;
  Callback            m_state_change_callback;
  Callback            m_click_callback;
  
};  //  class Switch
