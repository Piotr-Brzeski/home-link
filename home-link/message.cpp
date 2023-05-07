//
//  message.cpp
//  home-link
//
//  Created by Piotr Brzeski on 2023-02-01.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "message.h"
#include "logger.h"
#include <algorithm>

using namespace link;

namespace {

template<typename Element>
bool validate_list(buffer const& content) {
	auto size = content.size();
	if(size < 1) {
		return false;
	}
	auto number_of_elements = content[2];
	if(size != 2 + number_of_elements*sizeof(Element)) {
		return false;
	}
	auto element = reinterpret_cast<Element const*>(content.data() + 2);
	auto end = element + number_of_elements;
	for(; element < end; ++element) {
		if(!element->validate()) {
			return false;
		}
	}
	return true;
}

}

// MARK: - message
message::message(buffer content)
	: m_content(content) {
}

message::type message::get_type() const {
	if(!m_content.empty()) {
		switch(m_content[0]) {
			case type::advertisement:
			case type::list:
			case type::accept:
			case type::get_state:
			case type::state:
				return static_cast<message::type>(m_content[0]);
			default:
				break;
		}
	}
	return type::invalid_type;
}

buffer const& message::get_content() const {
	return m_content;
}

std::string message::description() const {
//	constexpr std::size_t max_log_message_size = 15;
//	auto log = std::string("{size=");
//	auto size = m_content.size();
//	log += std::to_string(size);
//	if(size > 0) {
//		log += ",{";
//		log += std::to_string(m_content[0]);
//		size = std::min(size, max_log_message_size);
//		for(std::size_t i = 1; i < size; ++i) {
//			log += ',';
//			log += std::to_string(m_content[i]);
//		}
//		log += '}';
//	}
//	log += '}';
//	return log;
	return ::description(m_content);
}

// MARK: - advertisement
advertisement::advertisement(advertisement::source source) {
	m_content.push_back(message::type::advertisement);
	m_content.push_back(source);
}

advertisement::advertisement(buffer content)
	: message(std::move(content)) {
}

bool advertisement::validate() const {
	return
		m_content.size() == 2 &&
		get_type() == type::advertisement &&
		(m_content[1] == source::controller ||
		 m_content[1] == source::node);
}

advertisement::source advertisement::get_source() const {
	return static_cast<advertisement::source>(m_content.at(1));
}

// MARK: - devices_list
devices_list::devices_list(message::type type) {
	m_content.push_back(type);
	m_content.push_back(0);
}

devices_list::devices_list(buffer content)
	: message(std::move(content)) {
}

void devices_list::add(device_id device_id) {
	m_content.append({device_id.type, device_id.number});
	++m_content[1];
}

//void devices_list::add(std::vector<device_id> const& device_ids) {
//	m_content.append(reinterpret_cast<std::uint8_t const*>(device_ids.data()), 2*device_ids.size());
//	m_content[1] += device_ids.size();
//}

std::uint8_t devices_list::size() const {
	return m_content[1];
}

device_id const* devices_list::begin() const {
	return reinterpret_cast<device_id const*>(m_content.data() + 2);
}

device_id const* devices_list::end() const {
	return begin() + size();
}

bool devices_list::validate(type expected_type) const {
	return get_type() == expected_type && validate_list<device_id>(m_content);
}

// MARK: - list
list::list()
	: devices_list(message::type::list) {
}

list::list(buffer content)
	: devices_list(std::move(content)) {
}

bool list::validate() const {
	return devices_list::validate(type::list);
}

// MARK: - accept
accept::accept()
	: devices_list(message::type::accept) {
}

accept::accept(buffer content)
	: devices_list(std::move(content)) {
}

bool accept::validate() const {
	return devices_list::validate(type::accept);
}

// MARK: - get_state
get_state::get_state()
	: devices_list(message::type::get_state) {
}

get_state::get_state(buffer content)
	: devices_list(std::move(content)) {
}

bool get_state::validate() const {
	return devices_list::validate(type::get_state);
}

// MARK: - state
state::state() {
	m_content.push_back(message::type::state);
	m_content.push_back(0);
}

state::state(buffer content)
	: message(std::move(content)) {
}

void state::add(device_id device_id, state_type state_type, state_value state_value) {
	m_content.append({device_id.type, device_id.number, state_type, state_value});
	++m_content[1];
}

std::uint8_t state::size() const {
	return m_content[1];
}

device_state const* state::begin() const {
	return reinterpret_cast<device_state const*>(m_content.data() + 2);
}

device_state const* state::end() const {
	return begin() + size();
}

bool state::validate() const {
	return get_type() == type::state && validate_list<device_state>(m_content);
}

// MARK: - some_message
some_message link::get_message(buffer buffer) {
	auto message = some_message();
	if(!buffer.empty()) {
		switch(buffer[0]) {
			case message::type::advertisement:
				message = advertisement(buffer);
				break;
			case message::type::list:
				message = list(buffer);
				break;
			case message::type::accept:
				message = accept(buffer);
				break;
			case message::type::get_state:
				message = get_state(buffer);
				break;
			case message::type::state:
				message = state(buffer);
				break;
			default:
				break;
		}
	}
	if(message) {
		auto valid = std::visit([](auto&& message){ return message.validate(); }, *message);
		if(!valid) {
			message.reset();
		}
	}
	return message;
}
