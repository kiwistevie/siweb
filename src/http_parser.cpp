#include "http_parser.h"

#include <cstring>
#include <iostream>

using namespace siweb::http;

void http_parser::parse(const std::string& input) {
	if (state_complete)
		return;

	std::vector<std::string> lines;
	for (const auto& ch : input) {
		if (ch != '\n') {
			curr << ch;
		} else {
			lines.push_back(siweb::utils::trim_copy(curr.str()));
			curr.str("");
			curr.clear();
		}
	}

	for (const auto& line : lines) {
		parse_line(line);
		std::cout << line << std::endl;
	}
	
	if (state_body && curr.str().length() > 0) {
		body << curr.str();
		if (body.str().length() >= get_content_length()) {
			state_complete = true;
		}
	}
}

void http_parser::parse_header_impl(const char* str) {
	std::ostringstream identifier;
	for (int i = 0; i < std::strlen(str); i++) {
		if (str[i] != ':') {
			identifier << (char)std::tolower(str[i]);
		} else {
			std::string value = "";
			if (i + 1 < std::strlen(str)) {
				value = std::string(&str[i + 1]);
			}
			headers[identifier.str()] = value;
			break;
		}
	}
}

void http_parser::parse_header(const std::string& line) {
	if (line.length() == 0) {
		if (get_content_length() == 0) {
			state_complete = true;
		} else {
			state_body = true;
		}
	} else {
		parse_header_impl(line.c_str());
	}
}

int http_parser::get_content_length() {
	return std::atoi(headers["content-length"].c_str());
}

void http_parser::parse_body(const std::string& line) {
	body << line << std::endl;
	if (body.str().length() >= get_content_length()) {
		state_complete = true;
	}
}

void http_parser::parse_method(const char* str) {
	std::ostringstream method;
	for (int i = 0; i < std::strlen(str); i++) {
		if (str[i] == ' ') {
			if (i + 1 < std::strlen(str)) {
				parse_uri(&str[i + 1]);
			}
			break;
		} else {
			method << str[i];
		}
	}
	
	std::string m = method.str();
	if (m == "GET") {
		this->method = httpMethod::GET;
	} else if (m == "POST") {
		this->method = httpMethod::POST;
	}
}

void http_parser::parse_uri(const char* str) {
	std::ostringstream uri;
	for (int i = 0; i < std::strlen(str); i++) {
		if (str[i] == ' ') {
			break;
		} else {
			uri << str[i];
		}
	}
	this->uri = uri.str();
}

void http_parser::parse_method_uri(const std::string& line) {
	parse_method(line.c_str());
	state_method_uri = true;
}

void http_parser::parse_line(const std::string& line) {
	if (state_complete) return;
	if (!state_method_uri) {
		parse_method_uri(line);
	} else {
		if (state_body) {
			parse_body(line);
		} else {
			parse_header(line);
		}
	}
}