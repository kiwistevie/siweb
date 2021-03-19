#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include "context.h"
#include "server.h"

#include <memory>
#include "middlewares/basic_auth_middleware.h"
#include "request.h"
#include "routing/file_route.h"
#include "routing/lambda_route.h"
#include "routing/route.h"
#include "routing/router.h"
#include "test_controller.h"

using namespace siweb::http;
using namespace siweb::http::routing;
using namespace siweb::http::middlewares;

int main(int argc, char* argv[]) {
    router rtr;
    file_route fr{"/", ""};
    rtr.add_route(fr);
    siweb_server server(rtr);
    return server.start(argc, argv);
}
