#include "router.h"

using siweb::http::response;
using siweb::http::response_t;
using siweb::http::route;
using siweb::http::router;

response_t router::route(const request& req) const {
    for (auto& r : routes) {
        if (r->is_hit(req)) {
            auto resp = std::make_shared<response>();

            auto it = middlewares.begin();
            while (it != middlewares.end()) {
                if (!(*it)->accept(req, resp))
                    return resp;
                it++;
            }

            auto result = (*r)(req);
            resp->set_content_type(result.get_content_type());
            resp->set_status_code(result.get_status_code());
            resp->set_content(result.get_content());

            auto rit = middlewares.rbegin();
            while (rit != middlewares.rend()) {
                if (!(*rit)->accept(req, resp))
                    return resp;
                rit++;
            }

            return resp;
        }
    }

    return std::make_shared<response>(httpStatusCode::NotFound);
}
