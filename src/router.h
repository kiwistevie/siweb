#ifndef siweb_router_h
#define siweb_router_h

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include "request.h"
#include "response.h"

namespace siweb::http {
class route {
   public:
    virtual bool is_hit(const request& req) const = 0;
    virtual response operator()(const request& req) const = 0;
};

class lambda_route : public route {
    using predicate_t = std::function<bool(const request&)>;
    using action_t = std::function<response(const request&)>;

   public:
    lambda_route(predicate_t predicate, action_t action)
        : predicate(predicate), action(action) {}

    bool is_hit(const request& req) const override { return predicate(req); }
    response operator()(const request& req) const override {
        return action(req);
    }

   private:
    predicate_t predicate;
    action_t action;
};

class router {
   public:
    response route(const request& req) const;
    void add_route(std::unique_ptr<siweb::http::route> r) {
        routes.emplace_back(std::move(r));
    }

   private:
    std::vector<std::unique_ptr<siweb::http::route>> routes;
};
}  // namespace siweb::http

#endif