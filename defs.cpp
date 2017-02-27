#include "defs.hpp"
#include <iostream>
#include <vector>

AbsSocket::AbsSocket(const std::string &node, const std::string &service) {
	const int status = getaddrinfo(node.c_str(), service.c_str(), NULL, &res);

	if ( status != 0 ) {
		std::cerr << "Error getting address information: " << gai_strerror(status) << '\n';
		freeaddrinfo(res);
		return;
	}
	
	file_desc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if ( file_desc == -1 ) {
		std::cerr << "Error getting socket.\n";
		return;
	}
}

AbsSocket::~AbsSocket() {
	freeaddrinfo(res);
	shutdown(file_desc, 2);
}

bool AbsSocket::to_connect() {
	if ( connect(file_desc, res->ai_addr, res->ai_addrlen) == -1 ) {
		std::cerr << "Error connecting!\n";
		return false;
	}

	return true;
}

bool AbsSocket::to_send(const std::string &phrase) {
	if ( send(file_desc, phrase.c_str(), phrase.length(), 0) == -1 ) {
		std::cerr << "Error sending!\n";
		return false;
	}

	return true;
}

std::string AbsSocket::to_receive() {
	std::vector<char> rec_buf (1024);

	switch (recv(file_desc, rec_buf.data() , 1024, 0)) {
	case 0:
		std::cerr << "Server closed connection.\n";
		return "CLOSED";
		
	case -1:
		std::cerr << "Error receiving.\n";
		return "ERROR";

	default:
		std::string resp (rec_buf.begin(), rec_buf.end());
		return resp;
	}

}
