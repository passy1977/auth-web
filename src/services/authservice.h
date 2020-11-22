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

#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include "../daos/userdao.h"
using namespace auth::daos;

#include <Poco/Dynamic/Var.h>
using Poco::Dynamic::Var;

#include <Poco/UUIDGenerator.h>
using Poco::UUIDGenerator;

#include <tuple>
using std::tuple;

#include"../globals.h"

namespace auth::services
{

/**
 * @brief In AuthService class
 */
class AuthService
{

    const UserDAO userDAO;

public:
    inline AuthService() :
        userDAO(Globals::getInstance()->getConnection())
    {

    }
    AUTH_NO_COPY_NO_MOVE(AuthService)

    void testDB(HTTPServerResponse &) const noexcept;

    /**
     * @brief login with email, password and domain,
     * @param jsonParsed from body
     * @throw Poco::Exception
     * @return if true it return even JWT token
     */
    tuple<bool, string> login(Var &&jsonParsed) const;

};

}
