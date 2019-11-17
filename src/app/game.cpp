#include "StartUpMenu.hpp"
#include "LocalGameLoop.hpp"
#include "Args.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
	std::string type, ip;
	Args *options = new Args();

	if (argc >= 2) {
		type = argv[1];
		if (type.compare("host") == 0) {
			options->isOnline = true;
			options->isHost = true;
			// Set host's ip to 0.0.0.0 by default 
			options->ip = "0.0.0.0";

			if (argc >= 3) {
				options->port = atoi(argv[2]);
			} else {
				std::cerr << "Host: Please provide port number." << std::endl;
				exit(1);
			}
		} else {
			options->isOnline = true;
			options->isHost = false;

			if (argc >= 3) {
				ip = argv[2];
				options->ip = ip;
			} else {
				std::cerr << "Client: Please provide IP address." << std::endl;
				exit(2);
			}

			if (argc >= 4) {
				options->port = atoi(argv[3]);
			} else {
				std::cerr << "Client: Please provide port number." << std::endl;
				exit(3);
			}
		}
	} else {
		options->isOnline = false;
	}

	launch(options);
	return 0;
}
