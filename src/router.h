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

#include <Poco/Net/HTTPRequestHandlerFactory.h>
using namespace Poco::Net;

#include "constants.h"

namespace auth
{

/**
 * @brief The Router class base compoment that manage routing to other ruoter class
 * @author Antonio Salsi
 */
class Router final : public HTTPRequestHandlerFactory
{
public:
    Router() = default;
    AUTH_NO_COPY_NO_MOVE(Router)

    /**
     * @brief Creates a new request handler for the given HTTP request.
     *
     * The method should inspect the given HTTPServerRequest object (e.g., method
     * and URI) and create an appropriate HTTPRequestHandler object to handle the
     * request.
     *
     * If the request contains a "Expect: 100-continue" header, it's possible
     * to prevent the server from sending the default 100 Continue response
     * by setting the status of the response object that can be obtained through
     * the request object (request.response()) to something other than 200 OK.
     * @return Http frequest poiter
     */
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) override;
};

}
