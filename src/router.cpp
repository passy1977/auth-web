// MIT License
//
// Copyright (c) 2020 Antonio Salsi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "router.h"

#include <iostream>
using namespace std;

#include <Poco/Net/HTTPServerRequest.h>
using namespace Poco::Net;

#include "globals.h"
#include "controllers/authcontroller.h"
#include "controllers/usercontroller.h"
#include "controllers/domaincontroller.h"
#include "controllers/httpstatuscontroller.h"
using namespace auth::controllers;

using namespace auth;

HTTPRequestHandler *Router::createRequestHandler(const HTTPServerRequest &request)
{


    if (request.getURI().rfind("/api/v1/auth", 0) == 0) {
        return new AuthController;
    } else if (request.getURI().rfind("/api/v1/user", 0) == 0) {
        return new UserController;
    } else if (request.getURI().rfind("/api/v1/domain", 0) == 0) {
        return new DomainController;
    }


    return HttpStatusController::build(HttpStatusController::HttpStatus::METHOD_NOT_ALOWED);
}


