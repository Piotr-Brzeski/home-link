//
//  logger.h
//  home-link
//  ESP
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <Arduino.h>
#include <string>

namespace logger {

template<typename T>
void log(T const& message) {
  Serial.println(message);
}

template<>
inline void log(std::string const& message) {
  Serial.println(message.c_str());
}

}
