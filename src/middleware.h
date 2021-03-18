#ifndef siweb_middleware_h
#define siweb_middleware_h

#include <memory>
#include <optional>
#include "request.h"
#include "response.h"
#include "types.h"

namespace siweb::http::middlewares {
class middleware {
   public:
    virtual bool accept(const request& req, response_t resp) = 0;

   private:
};
}  // namespace siweb::http::middlewares

#endif