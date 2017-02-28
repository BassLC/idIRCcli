#include "defs.hpp"
#include <iostream>
#include <vector>

AbsSocket::AbsSocket() {
	//Setting nick, server, password, port and channel
	std::cout << "Input password: ";
	std::getline(std::cin, password);

	std::cout << "Input your nick (default: env_name): ";
	std::getline(std::cin, nick);
	nick = (nick.length() ? nick : getenv("USER"));

	std::cout << "Input your host (default: irc.freenode.net): ";
	std::getline(std::cin, host);
	host = (host.length() ? host : "irc.freenode.net");
	
	std::cout << "Input port (default: 6667): ";
	std::getline(std::cin, port);
	port = (port.length() ? port : "6667");
	
	channel = "";

	const int status = getaddrinfo(host.c_str(), port.c_str(), NULL, &res);
	if ( status != 0 ) {
		std::cerr << "Error getting address information: " << gai_strerror(status) << '\n';
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
	//Init (Nick and Password)
	to_send( password.length() ? ("PASS " + password) : "");
	to_send("NICK " + nick);
	to_send("USER " + nick + " localhost " + host + " :" + nick);

	return true;
}

bool AbsSocket::to_send(const std::string &phrase) {
	if ( send(file_desc, (phrase + "\r\n").c_str(), phrase.length() + 2, 0) == -1 ) {
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
	}

	std::string resp (rec_buf.begin(), rec_buf.end());
	if ( resp.substr(0,4) == "PING" ) { to_send("PONG :" + resp.substr(7));} // So we're not automatically removed 

	return resp;
}

void AbsSocket::parse_and_send(std::string &phrase) {
	if ( phrase[0] == '/' ) { 
		switch( phrase[1] ) {
		case 'j':
			to_send("PART " + channel); //Exit old channel
			channel = phrase.substr(3);
			to_send("JOIN " + channel);
			break;

		case 'q':
			to_send("QUIT");
			break;

		case 'r':
			to_send(phrase.substr(3));
			break;

		case 's':
		{
			std::string other_person = phrase.substr(3, phrase.find_first_of(' ', 3));
			std::string message = phrase.substr(phrase.find_first_of(' ', 3) + 1);
			to_send("PRIVMSG " + other_person + " :" + message);
			break;
		}

		default:
			std::cout << "Command not found!\n";
			break;
		}

		return;
	}

	to_send("PRIVMSG "+ channel + " :" + phrase);
}



