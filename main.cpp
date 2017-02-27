#include <iostream>
#include "defs.hpp"
#include <string>

int main() {
	const std::string host = "127.0.0.1";
	const std::string port = "7777";
	
	AbsSocket connection(host, port);
	if ( connection.to_connect() ) {
		while ( true ) {
			std::cout << connection.to_receive();
		}
	}

	return 0;
}
