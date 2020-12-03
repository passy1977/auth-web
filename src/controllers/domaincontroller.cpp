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

#include "domaincontroller.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include <Poco/JSON/JSONException.h>
using Poco::JSON::JSONException;

#include "httpstatuscontroller.h"


using namespace auth::controllers;

void DomainController::handleRESTRequest(const string &method, const vector<string> &uriSplitted, HTTPServerRequest &request, HTTPServerResponse &response)
{
    try
    {

        ///insert domain
        if (method == HTTPServerRequest::HTTP_POST && uriSplitted[0] == _DOMAIN)
        {
            if (request.hasCredentials())
            {
                string scheme;
                string authInfo;

                request.getCredentials(scheme, authInfo);

                HttpStatusController::sendObject(response,
                                                 domainService.insert(
                                                     scheme,
                                                     authInfo,
                                                     response,
                                                     string(istreambuf_iterator<char>(request.stream()), {})
                                                     )
                                                 );
            }
            else
                HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::FORBIDDEN);
        }

        ///update domain
        else if (method == HTTPServerRequest::HTTP_PUT && uriSplitted[0] == _DOMAIN)
        {
            if (request.hasCredentials())
            {
                string scheme;
                string authInfo;

                request.getCredentials(scheme, authInfo);

                HttpStatusController::sendObject(response,
                                                 domainService.update(
                                                     scheme,
                                                     authInfo,
                                                     response,
                                                     string(istreambuf_iterator<char>(request.stream()), {})
                                                     )
                                                 );
            }
            else
                HttpStatusController::sendErrorObject(response, HttpStatusController::HttpStatus::FORBIDDEN);
        }

        else if (method == HTTPServerRequest::HTTP_GET && uriSplitted[0] == _DOMAIN)
        {
            if (request.hasCredentials())
            {
                string scheme;
                string authInfo;

                request.getCredentials(scheme, authInfo);

                HttpStatusController::sendObject(response, domainService.get(scheme, authInfo, response, uriSplitted[1]));
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
