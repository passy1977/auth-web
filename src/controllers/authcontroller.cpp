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

#include "authcontroller.h"

#include <string>
using namespace std;

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
using namespace Poco::JSON;

#include <Poco/Dynamic/Var.h>
using Poco::Dynamic::Var;

#include "httpstatuscontroller.h"

using namespace auth::controllers;

void AuthController::handleRESTRequest(const string &method, const string &partialUri, HTTPServerRequest &request, HTTPServerResponse &response)
{
    response.setChunkedTransferEncoding(true);

    //Sets mime type text/html application/json etc.
    response.setContentType(CONTENT_TYPE);

    //Sets the response status 404, 200 etc.
    response.setStatus("200");


    string body(std::istreambuf_iterator<char>(request.stream()), {});


    try
    {
        if (method == HTTPServerRequest::HTTP_POST && partialUri == "auth")
        {

            auto &&[check, data] = authService.login(Parser().parse(body));
            if(check)
            {
                Object jsonError;
                jsonError.set("type", "ok");
                jsonError.set("data", data);
                jsonError.stringify(response.send());
            }
            else
            {
                HttpStatusController::sendObject(response, HttpStatusController::HttpStatus::UNAUTHIRIZED);
            }

        } else
            HttpStatusController::sendObject(response, HttpStatusController::HttpStatus::METHOD_NOT_ALOWED);

    }
    catch(const JSONException& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.what());
        HttpStatusController::sendObject(response, HttpStatusController::HttpStatus::INTERNAL_SERVER_ERROR, e.what());
    }
    catch(const Poco::Exception& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.what());
        HttpStatusController::sendObject(response, HttpStatusController::HttpStatus::INTERNAL_SERVER_ERROR, e.what());
    }
}


