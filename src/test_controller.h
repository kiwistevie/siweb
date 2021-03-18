#ifndef siweb_test_controller_h
#define siweb_test_controller_h

#include <functional>
#include "controller.h"
#include "request.h"
#include "response.h"
#include "types.h"

namespace siweb::http {
class test_controller : public controller {
   public:
    test_controller();
    result get(const request& req);
    result post(const request& req);
};
}  // namespace siweb::http

#endif