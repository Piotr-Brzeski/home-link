//
//  test-link-controller.cpp
//  test-link-controller
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <home-link/controller.h>
#include <cpp-log/log.h>
#include <iostream>

#include "../configuration.h"

int main(int argc, const char * argv[]) {
	try {
		auto log = logger::start(logger::cout());
		auto controller = homelink::controller();
		controller.add({{{homelink::device_type::button, 1}, homelink::state_type::event}, homelink::state_value::click}, [](){
			std::cout << "CLICKED" << std::endl;
		});
		controller.start(configuration::port);
		int i = 0;
		std::cin >> i;
		return 0;
	}
	catch(...) {
		std::cerr << "Exception\n";
	}
	return 1;
}
