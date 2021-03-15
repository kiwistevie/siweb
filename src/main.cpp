#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include "context.h"
#include "server.h"

#include <memory>
#include "httpParser.h"
#include "request.h"
#include "router.h"

using namespace siweb::http;

int main(int argc, char* argv[]) {
    router rtr;
    std::unique_ptr<route> rt = std::make_unique<lambda_route>(
        [](const request& req) { return true; },
        [](const request& req) {
            std::ifstream t(req.get_uri().substr(1));
            std::string str((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
            return response(str.c_str(), str.length(), httpStatusCode::Ok);
        });

    rtr.add_route(std::move(rt));
    siweb_server server(rtr);
    server.start(argc, argv);
}
