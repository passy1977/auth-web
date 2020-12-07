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

#include "user.h"

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
using namespace Poco::JSON;

using namespace auth::pods;

Object User::toObject()
{
    Object ret;
    ret.set(FIELD_ID, id);
    ret.set(FIELD_NAME, name);
    ret.set(FIELD_EMAIL, email);
    ret.set(FIELD_PASSWORD, password);
    if (jsonData != "")
    {
        auto &&jsonData = Parser().parse(this->jsonData).extract<Object::Ptr>();
        ret.set(FIELD_JSON_DATA, jsonData);
    }
    ret.set(FIELD_PERMISSIONS, permissions);
    ret.set(FIELD_STATUS, static_cast<int>(status));
    ret.set(FIELD_LAST_LOGIN, lastLogin);
    ret.set(FIELD_EXPIRATION_DATE, expirationDate);
    if (domain)
        ret.set(FIELD_DOMAIN, domain->toObject());

    return ret;
}
