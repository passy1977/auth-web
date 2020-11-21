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

#include "authservice.h"

#include <Poco/JSON/Object.h>
using namespace Poco::JSON;


#include "../globals.h"

using namespace auth::services;

void AuthService::testDB(HTTPServerResponse &response) const noexcept
{

//    try
//    {
//        auto &&user = userDAO.get(1);

//        AUTH_GLOBAL_LOG(DBG, user->email);
//        AUTH_GLOBAL_LOG(DBG, user->domain->name);
//    }
//    catch(const std::bad_alloc &e)
//    {
//         AUTH_GLOBAL_LOG(DBG, e.what());
//    }

}

tuple<bool, string> AuthService::login(Var &&jsonParsed) const
{
    if (jsonParsed.type() == typeid(Object::Ptr))
    {

        Object::Ptr object = jsonParsed.extract<Object::Ptr>();

        if (!object->isNull(User::FIELD_EMAIL)
                && !object->isNull(User::FIELD_PASSWORD)
                && !object->isNull(User::FIELD_DOMAIN)
                )
        {
            auto &&email = object->get(User::FIELD_EMAIL);
            auto &&password = object->get(User::FIELD_PASSWORD);
            auto &&domain = object->get(User::FIELD_DOMAIN);
            AUTH_GLOBAL_LOG(DBG, email);
            AUTH_GLOBAL_LOG(DBG, password);
            AUTH_GLOBAL_LOG(DBG, domain);

            auto &&user =  userDAO.get(email, password, domain);

            return tuple(true, user->name);
        } else
            throw Poco::Exception("no valid param to attempt login");
    } else
        throw Poco::Exception("body request not contain object");
    return tuple(false, "");
}
