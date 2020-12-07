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

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include <Poco/JSON/JSONException.h>
using Poco::JSON::JSONException;

#include "httpstatuscontroller.h"

using namespace auth::controllers;

void AuthController::handleRESTRequest(const string &method, const vector<string> &uriSplitted, HTTPServerRequest &request, HTTPServerResponse &response) noexcept
{
    try
    {

        ///check JWT token request
        if (method == HTTPServerRequest::HTTP_GET && uriSplitted[0] == AUTH && uriSplitted[1] == AUTH_CHECK)
        {
            try
            {
                if (request.hasCredentials())
                {
                    string scheme;
                    string authInfo;

                    request.getCredentials(scheme, authInfo);

                     auto &&obj = HttpStatusController::buildObject(JSON_STATUS_OK);
                     obj.set(
                                 JSON_DATA,
                                 authService.check(scheme, authInfo, uriSplitted)
                             );

                    HttpStatusController::sendObject(response, obj);
                }
                else
                    HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_FORBIDDEN);
            }
            catch(const exception &e)
            {
                AUTH_GLOBAL_LOG(ERROR, e.what());
                HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
            }
        }

        ///login request
        else if (method == HTTPServerRequest::HTTP_POST && uriSplitted[0] == AUTH)
        {

            try
            {

                ///perform login and generate JWT
                auto &&[check, data] = authService.login(string(istreambuf_iterator<char>(request.stream()), {}));
                if(check)
                {
                    HttpStatusController::sendObject(response, data);
                }
                else
                {
                    HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_UNAUTHORIZED, data);
                }

            }
            catch(const exception &e)
            {
                AUTH_GLOBAL_LOG(ERROR, e.what());
                HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
            }
        }

        else
            HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_METHOD_NOT_ALLOWED);

    }
    catch(const JSONException& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
        HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.message());
    }
    catch(const Poco::Exception& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
        HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.message());
    }
    catch(const out_of_range& e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.what());
        HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
    }
}


