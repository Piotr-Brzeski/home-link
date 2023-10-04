//
//  map.h
//  home-link
//
//  Created by Piotr Brzeski on 2023-04-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <cstdint>
#include <utility>
#include <array>
#include <limits>

namespace homelink {

template<typename key_t, typename value_t, std::uint8_t max_size>
class map {
public:
	using size_type = std::uint8_t;
	using value_type = std::pair<key_t, value_t>;
	using iterator = value_type*;
	using const_iterator = value_type const*;
	
	iterator begin() {
		return m_storage.begin();
	}
	iterator end() {
		return m_storage.begin() + m_size;
	}
	const_iterator begin() const {
		return m_storage.begin();
	}
	const_iterator end() const {
		return m_storage.begin() + m_size;
	}
	
	size_type size() const {
		return m_size;
	}
	
	value_t& operator[](key_t const& key) {
		auto index = index_of(key);
		if(index == invalid_size) {
			if(m_size < invalid_size - 1) {
				index = m_size++;
				m_storage[index] = value_type(key, {});
			}
			else {
				throw false;
			}
		}
		return m_storage[index].second;
	}
	
private:
	constexpr static size_type invalid_size = std::numeric_limits<size_type>::max();
	
	uint8_t index_of(key_t const& key) const {
		for(size_type i = 0; i < m_size; ++i) {
			if(m_storage[i].first == key) {
				return i;
			}
		}
		return invalid_size;
	}
	
	std::array<value_type, max_size> m_storage;
	size_type                        m_size = 0;
};

}
