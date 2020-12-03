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

#include <Poco/JSON/Object.h>
using Poco::JSON::Object;

#include <string>
using std::string;

#include "../constants.h"

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
    AUTH_NO_COPY_NO_MOVE(HttpStatusController)


    void handleRequest(HTTPServerRequest &, HTTPServerResponse &) override;

    /**
     * @brief build error constroller
     * @param error HttpStatus
     * @return controller
     */
    static inline HttpStatusController * build(HttpStatus error) noexcept {
        return new HttpStatusController(error);
    }

    /**
     * @brief sendErrorObject to client JSON error
     */
    inline static void sendErrorObject(HTTPServerResponse &response, HttpStatus httpStatus, const string &errorMsg = "") noexcept
    {
        send(response, httpStatus, JSON_STATUS_ERROR, errorMsg);
    }

    /**
     * @brief sendObject to client JSON object pass only data
     */
    inline static void sendObject(HTTPServerResponse &response, const string &data) noexcept
    {
        send(response, HttpStatus::OK, JSON_STATUS_OK, data);
    }

    /**
     * @brief sendObject to client JSON object
     */
    static void sendObject(HTTPServerResponse &, HttpStatus, const Object &) noexcept;


    /**
     * @brief sendObject to client JSON object
     */
    static void sendObject(HTTPServerResponse &, const Object &&) noexcept;

    /**
     * @brief buildObject
     */
    static Object buildObject(HttpStatus, const string &, const string & = "") noexcept;

private:
    HttpStatus httpStatus;

    inline explicit HttpStatusController(HttpStatus httpStatus) : httpStatus(httpStatus) {}

    /**
     * @brief sendErrorObject to client JSON error
     */
    static void send(HTTPServerResponse &response, HttpStatus httpStatus, const string &jsonStatus, const string &errorMsg = "") noexcept
    {
        ///build json object response
        sendObject(response, httpStatus, std::move(buildObject(httpStatus, jsonStatus, errorMsg)));
    }
};

}

