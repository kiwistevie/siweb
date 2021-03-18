#include "router.h"

using siweb::http::response;
using siweb::http::response_t;
using siweb::http::route;
using siweb::http::router;

response_t router::route(const request& req) const {
    for (auto& r : routes) {
        if (r->is_hit(req)) {
            return (*r)(req);
        }
    }

    return std::make_unique<response>(httpStatusCode::NotFound);
}