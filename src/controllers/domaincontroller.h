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

#include "controller.h"
#include "../globals.h"

#include "../services/domainservice.h"
using auth::services::DomainService;

namespace auth::controllers
{

class DomainController final : public Controller
{

    DomainService domainService;
    string scheme;
    string authInfo;
public:
    static inline constexpr const char *PATH = "/api/v1/domain";
    static inline constexpr const char *NAME = "domain";

    inline DomainController(const string &method, const vector<string> &uriSplitted) noexcept try :
        Controller(method, uriSplitted)
    {}
    catch(const Poco::Exception &e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
    }
    AUTH_NO_COPY_NO_MOVE(DomainController)

    void handleRESTRequest(const string &, const vector<string> &, HTTPServerRequest &, HTTPServerResponse &) override;
};

}
