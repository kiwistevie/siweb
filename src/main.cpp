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
#include "lambda_route.h"
#include "request.h"
#include "route.h"
#include "router.h"
#include "test_controller.h"

using namespace siweb::http;

int main(int argc, char* argv[]) {
    router rtr;
    test_controller tc;
    rtr.register_controller(tc);
    siweb_server server(rtr);
    return server.start(argc, argv);
}
