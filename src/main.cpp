#include <iostream>
#include <unistd.h>
#include "server.h"
#include "context.h"
#include<sys/socket.h>
#include <sstream>

#include "httpParser.h"

#define READ_BUFFER_SIZE 32

void server_process(int fd, const context& ctx) {
	std::cout << "Got request from " << ctx.ip_addr << " and port " << ctx.port << std::endl;

	int ret;
	char buff[READ_BUFFER_SIZE];
	std::ostringstream input;

	HttpParser parser;
	while(true) {
		if((ret = recv(fd, buff, sizeof(buff), 0)) > 0) {
			parser.parse(std::string(buff, buff + ret));
			if (parser.is_message_complete()) {
				break;
			}
		} else {
			exit(0);
		}
	}

	input << "<html><body><h1>Hello</h1></body></html>";

	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK" << std::endl;
	oss << "Server: SiWeb/0.0.1 (Unix)" << std::endl;
	oss << "Content-Length: " << input.str().length() << std::endl;
	oss << "Content-Type: text/html" << std::endl;
	oss << "Connection: Closed" << std::endl;
	oss << std::endl;
	oss << input.str();
	
	std::string response = oss.str();
	write(fd, response.c_str(), response.length());
	close(fd);
}

int main(int argc, char* argv[]) {
	server_start(argc, argv);
}