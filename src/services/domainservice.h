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

#include <tuple>
using std::tuple;

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
using namespace Poco::Net;

#include <Poco/JSON/Object.h>
using Poco::JSON::Object;

#include "../daos/domaindao.h"
using namespace auth::daos;

namespace auth::services
{

class DomainService
{

    const DomainDAO domainDAO;

public:
    inline DomainService() :
        domainDAO(Globals::getInstance()->getConnection())
    {
    }
    AUTH_NO_COPY_NO_MOVE(DomainService)

    /**
     * @brief insert new domain
     * @param scheme of auth
     * @param authInfo JWT token
     * @param body request
     * @return domain insert
     */
    Object insert(const string &scheme, const string &authInfo, HTTPServerResponse &response, const string &&body) const noexcept;

    /**
     * @brief modify a domain
     * @param scheme of auth
     * @param authInfo JWT token
     * @param body request
     * @return domain modified
     */
    Object update(const string &scheme, const string &authInfo, HTTPServerResponse &response, const string &&body) const noexcept;

    /**
     * @brief get a domain
     * @param scheme of auth
     * @param authInfo JWT token
     * @param domainName request
     * @return domain request
     */
    Object get(const string &scheme, const string &authInfo, HTTPServerResponse &response, const string &domainName) const noexcept;

private:

    /**
     * @brief before insert or update
     * @param scheme of auth
     * @param authInfo JWT token
     * @param body request
     */
    tuple<Object, Object::Ptr, HTTPResponse::HTTPStatus> before(const string &scheme, const string &authInfo, HTTPServerResponse &response, const string &body) const noexcept;

};

}
