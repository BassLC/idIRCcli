#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include "defs.hpp"

void send_phrase(AbsSocket &socket ,std::atomic<bool> &ref) {
	std::string buffer;
	while ( ref.load() ) {
		std::getline(std::cin, buffer);
		socket.parse_and_send(buffer);
	}
}

int main() {
	AbsSocket connection;
	if ( connection.to_connect() ) {
		std::atomic<bool> run(true);
		std::thread input (send_phrase, std::ref(connection), std::ref(run));
		while ( run.load() ) {
			auto received = connection.to_receive();
			if ( received == "CLOSED"  || received == "ERROR" ) {
				run.store(false);
			}
			std::cout << received;
		}
		run.store(false);
		input.join();
	}

	return 0;
}
