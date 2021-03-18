#ifndef siweb_test_controller_h
#define siweb_test_controller_h

#include <functional>
#include "controller.h"
#include "request.h"
#include "response.h"

namespace siweb::http {
class test_controller : public controller {
   public:
    test_controller();
    response get(const request& req);
};
}  // namespace siweb::http

#endif