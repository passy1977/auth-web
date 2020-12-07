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

#include "../services/authservice.h"
using auth::services::AuthService;

#include "controller.h"

namespace auth::controllers
{

class AuthController final : public Controller
{
    const AuthService authService;

public:
    static inline constexpr const char *PATH = "/api/v1/auth";
    static inline constexpr const char *NAME = "auth";

    inline AuthController(const string &method, const vector<string> &uriSplitted) try :
        Controller(method, uriSplitted)
    {}
    catch(const Poco::Exception &e)
    {
        AUTH_GLOBAL_LOG(ERROR, e.message());
    }
    AUTH_NO_COPY_NO_MOVE(AuthController)

    void handleRESTRequest(const string &, const vector<string> &, HTTPServerRequest &, HTTPServerResponse &) noexcept override;

};

}
