#include "http_parser.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

using namespace siweb::http;

static void printError(const char* err) {
#ifdef DEBUG
    std::cerr << err << std::endl;
#endif
}

void http_parser::parse(const std::string& input) {
    if (state_complete)
        return;

    this->input = this->input + input;

    int last_start, last_current;
    try {
        while (!isAtEnd() && !state_complete) {
            last_start = start;
            last_current = current;
            parse();
            start = current;
        }
    } catch (...) {
        start = last_start;
        current = last_start;
    }
}

void http_parser::parse() {
    if (state_method_uri) {
        skipWhitespace();
        parse_method();
        skipWhitespace();
        parse_uri();
        while (peek() != '\n' && !isAtEnd()) {
            advance();
        }
        state_method_uri = false;
        if (!isAtEnd())
            advance();
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
    }
}

void http_parser::parse_uri() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }

    this->uri = makeString();
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
        printError("Expected ':' after header identifier.");
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
        printError("Expected '\\n' after header identifier.");
        panic();
    }
}

void http_parser::panic() {
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
    start = current;
    return r;
}