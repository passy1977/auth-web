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

#include <Poco/JSON/JSONException.h>
using Poco::JSON::JSONException;

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
using namespace Poco::JWT;
using Timestamp = Poco::Timestamp;

#include "httpstatuscontroller.h"

using namespace auth::controllers;

void AuthController::handleRESTRequest(const string &method, const string &partialUri, HTTPServerRequest &request, HTTPServerResponse &response)
{
    //response.setKeepAlive(true);

    response.setChunkedTransferEncoding(true);

    //Sets mime type text/html application/json etc.
    response.setContentType(HEADER_CONTENT_TYPE);

    //allow CORS
    response.set(HEADER_CORS, "*");

    try
    {
        ///login request
        if (method == HTTPServerRequest::HTTP_POST && partialUri == AUTH)
        {

            ///perform login and generate JWT
            auto &&[check, data] = authService.login(string(istreambuf_iterator<char>(request.stream()), {}));
            if(check)
            {
                HttpStatusController::sendObject(response, data);
            }
            else
            {
                HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::UNAUTHIRIZED, data);
            }
        }

        ///check JWT token request
        else if (method == HTTPServerRequest::HTTP_GET && partialUri.rfind(AUTH_CHECK, 0) == 0)
        {
            if (request.hasCredentials())
            {
                string scheme;
                string authInfo;

                request.getCredentials(scheme, authInfo);

                HttpStatusController::sendObject(response, authService.check(scheme, authInfo, partialUri) ? "true" : "false");
            }
            else
                HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::FORBIDDEN);
        }

        else
            HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::METHOD_NOT_ALOWED);

    }
    catch(const JSONException& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
        HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::INTERNAL_SERVER_ERROR, e.message());
    }
    catch(const Poco::Exception& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
        HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::INTERNAL_SERVER_ERROR, e.message());
    }
    catch(const out_of_range& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.what());
        HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::INTERNAL_SERVER_ERROR, e.what());
    }
}


