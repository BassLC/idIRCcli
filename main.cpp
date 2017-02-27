#include <iostream>
#include "defs.hpp"
#include <string>

int main() {
	const std::string host = "irc.freenode.net";
	const std::string port = "6667";
	const std::string nick = "asxcvbghty";
	const std::string channel = "#r/globaloffensive";
	const std::string password = "";

	AbsSocket connection(host, port);
	if ( connection.to_connect(nick, host, password, channel) ) {
		while ( true ) {
			std::string received = connection.to_receive();
			if ( received == "CLOSED" ) {
				return EXIT_SUCCESS;
			} else if ( received == "ERROR" ) {
				return EXIT_FAILURE;
			}

			std::cout << received;
		}
	}

	return 0;
}
