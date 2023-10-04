//
//  types.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <cstdint>
#include <string>
#ifdef __cpp_impl_three_way_comparison
	#include <compare>
#endif

namespace homelink {

enum class device_type: std::uint8_t {
	invalid_type = 0,
	button = 1
};

struct device_id {
	device_type  type = device_type();
	std::uint8_t number = 0;
	
	bool validate() const;
	
#ifdef __cpp_impl_three_way_comparison
	auto operator<=>(device_id const& other) const = default;
#else
	bool operator==(device_id const& other) const {
		return type == other.type && number == other.number;
	}
#endif
};

enum class state_type: std::uint8_t {
	invalid = 0,
	current = 1,
	target = 2,
	event = 3
};

struct device_state_type {
	device_id   device = device_id();
	state_type  state = state_type::invalid;
	
#ifdef __cpp_impl_three_way_comparison
	auto operator<=>(device_state_type const&) const = default;
#else
	bool operator==(device_state_type const& other) const {
		return state == other.state && device == other.device;
	}
#endif
};

enum class state_value: std::uint8_t {
	invalid   = 0,
	click     = 1,
	plus      = 2,
	minus     = 3,
	alt_plus  = 4,
	alt_minus = 5,
};

struct device_state {
	device_state_type type = device_state_type();
	state_value       value = state_value::invalid;
	
	bool validate() const;
	
#ifdef __cpp_impl_three_way_comparison
	auto operator<=>(device_state const&) const = default;
#else
	bool operator==(device_state const& other) const {
		return value == other.value && type == other.type;
	}
#endif
};

using buffer = std::basic_string<std::uint8_t>;
std::string description(buffer const& buffer);

}
