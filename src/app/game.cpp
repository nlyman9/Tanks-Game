#include "StartUpMenu.hpp"
#include "GameLoop.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
	std::string type, ip;
	Args *options = new Args();

	if (argc >= 2) {
		type = argv[1];
		if (type.compare("host")) {
			options->isOnline = true;
			options->isHost = true;
			options->ip = "0.0.0.0";

			if (argc >= 3) {
				options->ip = atoi(argv[2]);
			} else {
				std::cerr << "Please provide port number." << std::endl;
			}
		} else {
			options->isOnline = true;
			options->isHost = false;

			if (argc >= 3) {
				ip = argv[2];
				options->ip = ip;
			} else {
				std::cerr << "Please provide IP address." << std::endl;
			}

			if (argc >= 4) {
				options->ip = atoi(argv[3]);
			} else {
				std::cerr << "Please provide port number." << std::endl;
			}
		}
	} else {
		options->isOnline = false;
	}


	StartUpMenu startUpMenu;
	startUpMenu.launch(options);
	return 0;
}
