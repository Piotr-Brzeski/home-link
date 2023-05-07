//
//  message.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "types.h"
#include <string>
//#include <vector>
#include <variant>
#include <optional>

namespace link {

// MARK: message
class message {
public:
	enum type: std::uint8_t {
		advertisement = 1,
		list          = 2,
		accept        = 3,
		get_state     = 4,
		state         = 5,
		invalid_type  = 255
	};
	
	message() = default;
	message(buffer content);
	
	type get_type() const;
	buffer const& get_content() const;
	
	std::string description() const;
	
protected:
	buffer m_content;
};

// MARK: - advertisement
class advertisement: public message {
public:
	enum source: std::uint8_t {
		node       = 1,
		controller = 2
	};
	
	advertisement(source source);
	advertisement(buffer content);
	
	bool validate() const;
	
	source get_source() const;
};

// MARK: - devices_list
class devices_list: public message {
public:
	devices_list(message::type type);
	devices_list(buffer content);
	
	void add(device_id device_id);
//	void add(std::vector<device_id> const& device_ids);
	
	std::uint8_t size() const;
	device_id const* begin() const;
	device_id const* end() const;
	
protected:
	bool validate(type expected_type) const;
};

// MARK: - list
class list: public devices_list {
public:
	list();
	list(buffer content);
	
	bool validate() const;
};

// MARK: - accept
class accept: public devices_list {
public:
	accept();
	accept(buffer content);
	
	bool validate() const;
};

// MARK: - get_state
class get_state: public devices_list {
public:
	get_state();
	get_state(buffer content);
	
	bool validate() const;
};

// MARK: - state
class state: public message {
public:
	state();
	state(buffer content);
	
	void add(device_id device_id, state_type state_type, state_value state_value);
	
	std::uint8_t size() const;
	device_state const* begin() const;
	device_state const* end() const;
	
	bool validate() const;
};

// MARK: - some_message
using some_message = std::optional<std::variant<advertisement, list, accept, get_state, state>>;

some_message get_message(buffer buffer);

}
