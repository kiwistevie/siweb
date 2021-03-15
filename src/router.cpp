#include "router.h"

using siweb::http::response;
using siweb::http::route;
using siweb::http::router;

response router::route(const request& req) const {
    for (auto& r : routes) {
        if (r->is_hit(req)) {
            return (*r)(req);
        }
    }

    response r(httpStatusCode::NotFound);
    return r;
}