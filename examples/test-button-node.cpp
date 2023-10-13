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
	
	const homelink::device_id id;
	std::function<void()> on_click;
};


int main(int argc, const char * argv[]) {
	try {
		auto log = logger::start(logger::cout());
		auto node = homelink::node();
		auto button = encoder(1);
		button.on_click = [&]() {
			node.send(button.id, homelink::state_type::event, homelink::state_value::click);
		};
		
		node.start(configuration::port);
		
		std::string cmd;
		while(true) {
			std::cin >> cmd;
			if(cmd == "q") {
				break;
			}
			button.click();
		}
		
		return 0;
	}
	catch(...) {
		std::cerr << "Exception\n";
	}
	return 1;
}
