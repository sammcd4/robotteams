#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>

#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32")

#include <google/protobuf/message.h>


class connection {
public:
	connection(std::string address, std::string port);
	~connection();
	
	bool is_valid();

	void write_message(google::protobuf::Message* message);
	char* read_frame(uint64_t& message_len);

private:
	SOCKET sock_fd;
};