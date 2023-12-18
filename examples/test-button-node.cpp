//
//  test-button-node.cpp
//  test-button-node
//
//  Created by Piotr Brzeski on 2023-04-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <home-link/node.h>
#include <cpp-log/log.h>
#include <functional>
#include <iostream>
#include <string>

#include "../configuration.h"

class encoder {
public:
	encoder(std::uint8_t number)
		: id{homelink::device_type::button, number} {
	}
	
	void click() {
		if(on_click) {
			on_click();
		}
	}
	void plus() {
		if(on_plus) {
			on_plus();
		}
	}
	void minus() {
		if(on_minus) {
			on_minus();
		}
	}

	const homelink::device_id id;
	std::function<void()> on_click;
	std::function<void()> on_plus;
	std::function<void()> on_minus;
};


int main(int argc, const char * argv[]) {
	try {
		auto log = logger::start(logger::cout());
		auto node = homelink::node();
		auto button = encoder(4);
		button.on_click = [&]() {
			node.send(button.id, homelink::state_type::event, homelink::state_value::click);
		};
		button.on_plus = [&]() {
			node.send(button.id, homelink::state_type::event, homelink::state_value::plus);
		};
		button.on_minus = [&]() {
			node.send(button.id, homelink::state_type::event, homelink::state_value::minus);
		};
		node.add(button.id);
		node.start(configuration::port);
		
		std::string cmd;
		while(true) {
			std::cin >> cmd;
			if(cmd == "q") {
				break;
			}
			if(cmd == "+") {
				button.plus();
			}
			else if(cmd == "-") {
				button.minus();
			}
			else {
				button.click();
			}
		}
		
		return 0;
	}
	catch(...) {
		std::cerr << "Exception\n";
	}
	return 1;
}
