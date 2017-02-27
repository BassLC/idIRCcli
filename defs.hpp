#ifndef DEFS_HPP
#define DEFS_HPP

#include <sys/socket.h>
#include <netdb.h>
#include <string>

class AbsSocket {
private:
	int file_desc;
	struct addrinfo *res;
	
public:
	AbsSocket(const std::string &node, const std::string &service);
	~AbsSocket();

	bool to_connect();
	bool to_send(const std::string &phrase);
	std::string to_receive();
};


#endif