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

bool AbsSocket::to_connect(const std::string &nick, const std::string &host, const std::string &password, const std::string &channel) {
	if ( connect(file_desc, res->ai_addr, res->ai_addrlen) == -1 ) {
		std::cerr << "Error connecting!\n";
		return false;
	}
	//Init (Nick and Password)
	to_send( password.length() ? ("PASS " + password) : "");
	to_send("NICK " + nick);
	to_send("USER " + nick + " localhost " + host + " :" + nick);
	to_send("JOIN " + channel);

	return true;
}

bool AbsSocket::to_send(const std::string &phrase) {
	if ( send(file_desc, (phrase + "\r\n").c_str(), phrase.length() + 2, 0) == -1 ) { // length + 2 for the \r\n bytes!
		std::cerr << "Error sending!\n";
		return false;
	}

	return true;
}

std::string AbsSocket::to_receive() {
	std::vector<char> rec_buf (1024);
	int bytes_received = recv(file_desc, rec_buf.data(), 1024, 0);
	switch ( bytes_received ) {
	case 0:
		std::cerr << "Server closed connection.\n";
		return "CLOSED";
		
	case -1:
		std::cerr << "Error receiving.\n";
		return "ERROR";

	default:
		rec_buf.push_back('\0');
		std::string resp (rec_buf.begin(), rec_buf.end());
		return resp;
	}

}
