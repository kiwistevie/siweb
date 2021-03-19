#include "http_parser.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
#include "../debug.h"

using namespace siweb::http::parsing;

void http_parser::parse(const std::string& input) {
    if (state_complete)
        return;

    DEBUG_NEWLINE();
    DEBUG_INFO(std::string("Parsing ")
                   .append(std::to_string(input.length()))
                   .append(" bytes read from client ..."));
    this->input = this->input + input;

    int last_start;
    try {
        while (!isAtEnd() && !state_complete) {
            last_start = start;
            parse();
            start = current;
        }
    } catch (...) {
        start = last_start;
        current = last_start;
    }

#ifdef DEBUG
    if (state_complete) {
        DEBUG_STR_MAP("Headers", headers);
        std::cout << std::endl;
        DEBUG_STR_MAP("Parameters", parameters);
        DEBUG_INFO("Client message successfully parsed.");
    }
#endif
}

void http_parser::parse() {
    if (state_method_uri) {
        skipWhitespace();
        parse_method();
        skipWhitespace();
        parse_uri();
        while (!match('\n') && !isAtEnd())
            advance();
        if (isAtEnd())
            panic();
        state_method_uri = false;
    } else {
        if (state_body) {
            int content_length = get_content_length();
            if (content_length == 0) {
                state_complete = true;
                return;
            }
            int length = content_length - body.length();
            while (length > 0 && !isAtEnd()) {
                advance();
                length--;
            }
            body = body + makeString();
            if (body.length() == content_length) {
                state_complete = true;
            }
        } else {
            parse_header();
        }
    }
}

int http_parser::get_content_length() {
    return std::atoi(headers["content-length"].c_str());
}

void http_parser::parse_method() {
    while (isAlpha(peek())) {
        advance();
    }

    auto method = makeString();

    if (method == "GET") {
        this->method = httpMethod::GET;
    } else if (method == "POST") {
        this->method = httpMethod::POST;
    } else if (method == "DELETE") {
        this->method = httpMethod::DELETE;
    } else if (method == "OPTIONS") {
        this->method = httpMethod::OPTIONS;
    } else if (method == "PUT") {
        this->method = httpMethod::PUT;
    } else if (method == "CONNECT") {
        this->method = httpMethod::CONNECT;
    } else if (method == "TRACE") {
        this->method = httpMethod::TRACE;
    } else if (method == "PATCH") {
        this->method = httpMethod::PATCH;
    } else if (method == "HEAD") {
        this->method = httpMethod::HEAD;
    }
}

void http_parser::parse_uri() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }

    this->uri = makeString();

    if (match('?')) {
        do {
            while (isAlpha(peek()) || isDigit(peek())) {
                advance();
            }

            auto name = makeString();
            if (match('=')) {
                while (isAlpha(peek()) || isDigit(peek())) {
                    advance();
                }
                auto value = makeString();
                parameters[name] = value;
            } else {
                parameters[name] = "";
            }
        } while (match('&'));
    }
}

void http_parser::parse_header() {
    skipWhitespace();
    if (match('\n')) {
        if (isAtEnd() && get_content_length() == 0) {
            state_complete = true;
        } else {
            state_body = true;
        }
        return;
    }

    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }

    auto identifier = makeString();
    std::transform(std::begin(identifier), std::end(identifier),
                   std::begin(identifier),
                   [](unsigned char c) { return std::tolower(c); });
    if (!match(':')) {
        DEBUG_INFO("Expected ':' after header identifier.");
        panic();
    }

    skipWhitespace();
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }

    auto value = makeString();
    headers[identifier] = value;

    skipWhitespace();
    if (!match('\n')) {
        DEBUG_INFO("Expected '\\n' after header identifier.");
        panic();
    }
}

void http_parser::panic() {
    DEBUG_INFO("Entering Panic Mode");
    throw 0;
}

bool http_parser::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           c == '/' || c == '%' || c == '.' || c == '-';
}

bool http_parser::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool http_parser::isAtEnd() {
    return current >= input.length();
}

char http_parser::advance() {
    current++;
    return input[current - 1];
}

char http_parser::peek() {
    return input[current];
}

char http_parser::peekNext() {
    if (isAtEnd())
        return '\0';
    return input[current + 1];
}

bool http_parser::match(char expected) {
    if (isAtEnd())
        return false;
    if (input[current] != expected)
        return false;
    current++;
    start = current;
    return true;
}

void http_parser::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                start = current;
                break;
            default:
                start = current;
                return;
        }
    }
}

std::string http_parser::makeString() {
    auto r = input.substr(start, (current - start));
    if (r.length() == 0) {
        DEBUG_INFO("Made string is empty.");
        panic();
    }
    start = current;
    return r;
}