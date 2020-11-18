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

#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco::Net;

#include "../services/authservice.h"
using auth::services::AuthService;

#include "../constants.h"

namespace auth::controllers
{

class Controller : public HTTPRequestHandler
{
    const string method;
    const string uri;

public:
    inline explicit Controller(const string &method, const string &uri) : method(method), uri(uri)
    {}
    AUTH_NO_COPY_NO_MOVE(Controller)

    virtual void handleRESTRequest(const string &, const string &, HTTPServerRequest &, HTTPServerResponse &) = 0;

    inline void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override
    {
        handleRESTRequest(method, uri, request, response);
    }

};


}
