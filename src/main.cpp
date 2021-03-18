#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include "context.h"
#include "server.h"

#include <memory>
#include "basic_auth_middleware.h"
#include "file_route.h"
#include "lambda_route.h"
#include "request.h"
#include "route.h"
#include "router.h"
#include "test_controller.h"

using namespace siweb::http;
using namespace siweb::http::middlewares;

int main(int argc, char* argv[]) {
    const char* rel_path = argc > 2 ? argv[3] : "/";
    router rtr;
    file_route fr{"/", rel_path};
    rtr.add_route(fr);
    siweb_server server(rtr);
    return server.start(argc, argv);
}
