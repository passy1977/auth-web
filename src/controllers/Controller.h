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

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
using namespace Poco::Net;

#include "../constants.h"

namespace auth::controllers
{

/**
 * @brief The Controller basi class of all contreer, to semplify some action
 */
class Controller : public HTTPRequestHandler
{
    const string method;
    const vector<string> uriSplitted;

public:
    inline Controller(const string &method, const vector<string> &uriSplitted);
    AUTH_NO_COPY_NO_MOVE(Controller)

    /**
     * @brief handleRESTRequest each controller impement this vitual method and iomplement business logic
     */
    virtual void handleRESTRequest(const string &, const  vector<string> &, HTTPServerRequest &, HTTPServerResponse &) noexcept = 0;

    inline void handleRequest(HTTPServerRequest &, HTTPServerResponse &) override;

};

Controller::Controller(const string &method, const vector<string> &uriSplitted) :
    method(method),
    uriSplitted(uriSplitted)
{
    if (uriSplitted.empty())
        throw Poco::Exception("Uri splitted empty");
}

void Controller::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    response.setChunkedTransferEncoding(true);

    //Sets mime type text/html application/json etc.
    response.setContentType(HEADER_CONTENT_TYPE);

    //allow CORS
    response.set(HEADER_CORS, "*");

    handleRESTRequest(method, uriSplitted, request, response);
}


}
