#include "router.h"

#include <iostream>
using namespace std;

#include "controllers/errorcontroller.h"
using auth::controllers::ErrorController;

#include <Poco/Net/HTTPServerRequest.h>
using namespace Poco::Net;

using namespace auth;

HTTPRequestHandler *Router::createRequestHandler(const HTTPServerRequest &request)
{



    cout << "uri:" << request.getURI() << endl;

     auto &r = const_cast<HTTPServerRequest &>(request);
    cout << "body:" << string(std::istreambuf_iterator<char>(r.stream()), {}) << endl;




    return ErrorController::build(ErrorController::HttpStatus::OK);
}


