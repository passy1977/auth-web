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

#include "httpstatuscontroller.h"

#include <string>
using namespace std;

#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

using namespace auth::controllers;

void HttpStatusController::handleRequest(HTTPServerRequest &, HTTPServerResponse &response)
{
    response.setChunkedTransferEncoding(true);

    sendErrorObject(response, httpStatus);
}

void HttpStatusController::sendObject(HTTPServerResponse &response, HTTPResponse::HTTPStatus httpStatus, const Object &obj) noexcept
{
    //response.setKeepAlive(true);

    //Sets mime type text/html application/json etc.
    response.setContentType(HEADER_CONTENT_TYPE);

    //Sets http status
    response.setStatus(move(to_string(httpStatus)));

    obj.stringify(response.send());
}

void HttpStatusController::sendObject(HTTPServerResponse &response, const Object &obj) noexcept
{
    //Sets mime type text/html application/json etc.
    response.setContentType(HEADER_CONTENT_TYPE);

    //Sets http status
    response.setStatus(move(to_string(HTTPResponse::HTTP_OK)));

    obj.stringify(response.send());
}

Object HttpStatusController::buildObject(const string &jsonStatus, const string &errorMsg) noexcept
{
    ///build json object response
    Object jsonObj;
    jsonObj.set(JSON_STATUS, jsonStatus);
    if (errorMsg != "")
    {
        jsonObj.set(JSON_DATA, errorMsg);
    }
    return jsonObj;
}



