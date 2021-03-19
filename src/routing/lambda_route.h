#ifndef siweb_lambda_route_h
#define siweb_lambda_route_h

#include <functional>
#include "../request.h"
#include "../response.h"
#include "../types.h"
#include "route.h"

namespace siweb::http::routing {
class lambda_route : public route {
    using predicate_t = std::function<bool(const request&)>;
    using action_t = std::function<result(const request&)>;

   public:
    lambda_route(predicate_t predicate, action_t action)
        : predicate(predicate), action(action) {}

    bool is_hit(const request& req) const override { return predicate(req); }
    result operator()(const request& req) const override { return action(req); }

   private:
    predicate_t predicate;
    action_t action;
};
}  // namespace siweb::http::routing

#endif