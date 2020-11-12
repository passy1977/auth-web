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

namespace auth::controllers
{

class HttpStatusController final : public HTTPRequestHandler
{
public:

    enum class HttpStatus : uint16_t {
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        BAD_REQUEST = 400,
        UNAUTHIRIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALOWED = 405,
        INTERNAL_SERVER_ERROR = 500,
        SERVICE_UNAVAILABLE = 503,
    };

    HttpStatusController() = delete;
    HttpStatusController(const HttpStatusController&) = delete;
    HttpStatusController& operator = (const HttpStatusController&) = delete;
    HttpStatusController(HttpStatusController&&) = delete;
    HttpStatusController& operator = (HttpStatusController&&) = delete;

    void handleRequest(HTTPServerRequest &, HTTPServerResponse &) override;

    static inline HttpStatusController * build(HttpStatus error) noexcept {
        return new HttpStatusController(error);
    }

private:
    HttpStatus httpStatus;

    inline explicit HttpStatusController(HttpStatus httpStatus) : httpStatus(httpStatus) {}
};

}

