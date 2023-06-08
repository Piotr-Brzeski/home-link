//
//  test-link-controller.cpp
//  test-link-controller
//
//  Created by Piotr Brzeski on 2023-05-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <home-link/controller.h>
#include <iostream>

#include "../configuration.h"

int main(int argc, const char * argv[]) {
	try {
		auto controller = link::controller();
		controller.add({{{link::device_type::button, 1}, link::state_type::event}, link::state_value::click}, [](){
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
