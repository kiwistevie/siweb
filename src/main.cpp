#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include "context.h"
#include "server.h"

#include <memory>
#include "file_route.h"
#include "request.h"
#include "router.h"

using namespace siweb::http;

int main(int argc, char* argv[]) {
    router rtr;
    std::unique_ptr<route> rt = std::make_unique<lambda_route>(
        [](const request& req) { return req.get_uri() == "/test"; },
        [](const request& req) {
            return response(req.get_body().c_str(), req.get_body().length(),
                            httpStatusCode::NotImplemented);
        });

    rtr.add_route(std::make_unique<file_route>("/src/", ""));
    rtr.add_route(std::move(rt));
    siweb_server server(rtr);
    return server.start(argc, argv);
}
