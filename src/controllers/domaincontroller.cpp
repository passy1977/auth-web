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

void DomainController::handleRESTRequest(const string &method, const vector<string> &uriSplitted, HTTPServerRequest &request, HTTPServerResponse &response) noexcept
{

    if (request.hasCredentials())
    {

        string scheme;
        string authInfo;

        request.getCredentials(scheme, authInfo);


        ///insert domain
        if (method == HTTPServerRequest::HTTP_POST && uriSplitted[0] == _DOMAIN)
        {
            try
            {

                auto &&[obj, status] = domainService.insert(
                         scheme,
                         authInfo,
                         uriSplitted[1],
                         string(istreambuf_iterator<char>(request.stream()), {})
                         );

                HttpStatusController::sendObject(response, status, obj);

            }
            catch(const exception &e)
            {
                AUTH_GLOBAL_LOG(ERROR, e.what());
                HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
            }
        }

        ///update domain
        else if (method == HTTPServerRequest::HTTP_PUT && uriSplitted[0] == _DOMAIN)
        {
            try
            {
                auto &&[obj, status] = domainService.update(
                         scheme,
                         authInfo,
                         string(istreambuf_iterator<char>(request.stream()), {})
                         );

                HttpStatusController::sendObject(response, status, obj);

            }
            catch(const exception &e)
            {
                AUTH_GLOBAL_LOG(ERROR, e.what());
                HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
            }
        }

        ///get domain
        else if (method == HTTPServerRequest::HTTP_GET && uriSplitted[0] == _DOMAIN)
        {
            try
            {

                request.getCredentials(scheme, authInfo);

                auto &&[obj, status] = domainService.get(
                        scheme,
                        authInfo,
                        uriSplitted[1]
                        );

                HttpStatusController::sendObject(response, status, obj);

            }
            catch(const exception &e)
            {
                AUTH_GLOBAL_LOG(ERROR, e.what());
                HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
            }
        }

        ///in other cases
        else
            HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_METHOD_NOT_ALLOWED);


    }

    ///no credential
    else
        HttpStatusController::sendErrorObject(response, HTTPResponse::HTTP_FORBIDDEN);


}
