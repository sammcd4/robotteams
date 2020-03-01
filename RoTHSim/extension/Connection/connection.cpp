#include "connection.h"

connection::connection(std::string address, std::string port) {
	sock_fd = INVALID_SOCKET;

	// Create and populate the address struct.
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve server address(es).
	struct addrinfo* result = NULL;
	int err = getaddrinfo(address.c_str(), ("" + port).c_str(), &hints, &result);

	if (err != 0) {
		sock_fd = INVALID_SOCKET;
		return;
	}

	// Try connecting to all returned addresses;
	struct addrinfo* ptr = NULL;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		sock_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (sock_fd == INVALID_SOCKET) {
			return;
		}

		err = connect(sock_fd, ptr->ai_addr, ptr->ai_addrlen);

		// Fail if unable to connect.
		if (err == SOCKET_ERROR) {
			closesocket(sock_fd);

			sock_fd = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);
}

connection::~connection() {
	closesocket(sock_fd);
}

bool connection::is_valid() {
	return sock_fd != INVALID_SOCKET;
}

void connection::write_message(google::protobuf::Message* message) {
	// Create the size header.
	size_t header_len = sizeof(uint64_t);
	char* header_buf = (char*) malloc(header_len);

	const uint64_t message_len = message->ByteSize();
	memcpy(header_buf, &message_len, header_len);

	// Send the header.
	send(sock_fd, header_buf, header_len, 0);
	free(header_buf);

	// Serialize the body data.
	int data_len = message->ByteSize();
	char* data_buf = (char*) malloc(data_len);
	message->SerializeToArray(data_buf, data_len);

	// Send the body data.
	send(sock_fd, data_buf, data_len, 0);
}

char* connection::read_frame(uint64_t& message_len) {
	// Read the header.
	recv(sock_fd, (char*) &message_len, sizeof(message_len), 0);

	if (message_len == 0) {
		return NULL;
	}

	// Allocate the message body.
	char* message_buf = (char*) malloc((size_t) message_len);

	// Read the message body.
	recv(sock_fd, message_buf, (size_t) message_len, 0);

	// UNCOMMENT FOR DEBUG PRINTING
	/*
	printf("size: %d\n", message_len);
		
	printf("contents: [");
	for (unsigned int i = 0; i < message_len; ++i)
	{
		printf("%d ", message_buf[i]);
	}
	printf("]\n");
	*/

	return message_buf;
}