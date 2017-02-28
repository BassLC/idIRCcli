#ifndef DEFS_HPP
#define DEFS_HPP

#include <sys/socket.h>
#include <netdb.h>
#include <string>

class AbsSocket {
private:
	int file_desc;
	struct addrinfo *res;

	std::string port = "6667";
	std::string password;
	std::string nick;
	std::string host;
	std::string channel;

public:
	AbsSocket();
	~AbsSocket();

	bool to_connect();
	bool to_send(const std::string &phrase);
	std::string to_receive();
	void parse_and_send(std::string &phrase);
};

#endif
