//
//  types.cpp
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "types.h"

using namespace link;

namespace {

bool validate(state_type state_type) {
	return state_type == state_type::current ||
	       state_type == state_type::target ||
	       state_type == state_type::event;
}

}

bool device_id::validate() const {
	return type == device_type::button;
}

bool device_state::validate() const {
	return type.device.validate() && ::validate(type.state);
}

std::string link::description(buffer const& buffer) {
	constexpr std::size_t max_size = 15;
	auto str = std::string("{size=");
	auto size = buffer.size();
	str += std::to_string(size);
	if(size > 0) {
		str += ",{";
		str += std::to_string(buffer[0]);
		for(std::size_t i = 1; i < std::min(size, max_size); ++i) {
			str += ',';
			str += std::to_string(buffer[i]);
		}
		if(size > max_size) {
			str += ",...";
		}
		str += '}';
	}
	str += '}';
	return str;
}
