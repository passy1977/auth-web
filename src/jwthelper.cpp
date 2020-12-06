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

#include "jwthelper.h"

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JWT/JWTException.h>
using namespace Poco::JWT;
using Timestamp = Poco::Timestamp;

#include<Poco/DateTimeParser.h>
using namespace Poco;

#include "pods/domain.h"
#include "pods/user.h"
using namespace auth::pods;

#include "globals.h"

using namespace auth;

bool jwtCheck(const string &scheme, const string &authInfo, const string &seecret, const User::Ptr &user) noexcept
{
    if (scheme != "Bearer" || authInfo == "")
    {
        return false;
    }

    Token token;
    Signer signer(seecret);

    try {
        if (!signer.tryVerify(authInfo, token))
            return false;
    }  catch (const SignatureGenerationException &e) {
        return false;
    }


    auto &&now = Timestamp();
    auto &&expiration = token.getExpiration();
    if (now > expiration)
        return false;

    auto &&payload = token.payload();
    if (!payload.has(User::FIELD_EXPIRATION_DATE))
    {
        DateTime date;
        int tzd = 0;
        if (DateTimeParser::tryParse( DateTimeFormat::SORTABLE_FORMAT, payload.get(User::FIELD_EXPIRATION_DATE).toString(), date, tzd))
            if (now > date.timestamp())
                return false;
    }

    if (!payload.has(Domain::FIELD_EXPIRATION_DATE))
    {
        DateTime date;
        int tzd = 0;
        if (DateTimeParser::tryParse( DateTimeFormat::SORTABLE_FORMAT, payload.get(Domain::FIELD_EXPIRATION_DATE).toString(), date, tzd))
            if (now > date.timestamp())
                return false;
    }


    if (!payload.has(Domain::FIELD_NAME))
        return false;

    if (!payload.has(User::FIELD_EMAIL))
        return false;

    if (user != nullptr)
    {

        user->email = payload.get(User::FIELD_EMAIL).toString();

        if(payload.has(User::FIELD_EXPIRATION_DATE))
            user->expirationDate = payload.get(User::FIELD_EXPIRATION_DATE).toString();

        if(payload.has(User::FIELD_JSON_DATA))
            user->expirationDate = payload.get(User::FIELD_JSON_DATA).toString();

        if(payload.has(User::FIELD_JSON_DATA))
            user->jsonData = payload.get(User::FIELD_JSON_DATA).toString();

        user->permissions = token.getAudience();

        user->domain = make_shared<Domain>(
                    0,
                    payload.get(User::FIELD_NAME).toString(),
                    "",
                    Domain::Status::ACTIVE,
                    "",
                    0
                    );

        if(payload.has(Domain::FIELD_EXPIRATION_DATE))
            user->domain->expirationDate = payload.get(Domain::FIELD_EXPIRATION_DATE).toString();

    }

    return true;
}
