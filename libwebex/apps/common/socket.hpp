#ifndef WEBEX_COMMON_SOCKET_HPP
#define WEBEX_COMMON_SOCKET_HPP

#include <string>
#include <cstring>

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

namespace common
{


struct Http_socket
{
	using Address_In = struct sockaddr_in*;
	using Address = struct sockaddr*;
	using Host = struct hostent*;
	Http_socket(std::string const& host)
		: fd_(-1), addr_(), host_(host.c_str())
	{
		// Create the socket.
		if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw std::string("Failed to create socket");
		}

		// Get host info.
		Host hst = gethostbyname(host_);
		if (!hst) {
			throw std::string("Failed to get host name for " + host);
		}

		// Set socket address fields.
		addr_->sin_family = AF_INET;
		addr_->sin_port = htons(80);
		addr_->sin_addr = *((struct in_addr*)hst->h_addr);
		std::memset(&addr_->sin_zero, 0, 8);

		// Connect to host.
		if (connect(fd_, (Address)addr_, sizeof(struct sockaddr)) == -1) {
			throw std::string("Failed to connect");
		}
	}


	bool
	fetch(std::string const& path, char* reply)
	{
		// Create the request for the given path at the predetermined host.
		char request[1024];
		snprintf(request, 1024, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", path.c_str(), host_);
		
		// Send the request.
		int res;
		if ((res = send(fd_, request, strlen(request), 0)) < 0) {
			throw std::string("Failed to send request");
			return false;
		}

		

	}


	// Data Members.
	//
	// Socket file descriptor.
	int fd_;
	// Socket address.
	Address_In	addr_;
	// Host domain.
	char const* host_;
};

}

#endif