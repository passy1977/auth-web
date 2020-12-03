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
using auth::pods::Domain;

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
using namespace Poco::JSON;

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
using namespace Poco::JWT;

#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
using namespace Poco::Crypto;

#include "../globals.h"

using namespace auth::services;

extern bool jwtCheck(const string &scheme, const string &authInfo, const string &seecret) noexcept;

tuple<bool, string> AuthService::login(const string &&body) const
{

    auto &&jsonParsed = Parser().parse(body);

    /// check obk type
    if (jsonParsed.type() == typeid(Object::Ptr))
    {
        ///parse body request
        Object::Ptr object = jsonParsed.extract<Object::Ptr>();

        ///check, extract data and retrive user
        if (!object->isNull(User::FIELD_EMAIL)
                && !object->isNull(User::FIELD_PASSWORD)
                && !object->isNull(User::FIELD_DOMAIN)
                )
        {
            auto &&user = userDAO.get(
                move(object->get(User::FIELD_EMAIL)),
                move(object->get(User::FIELD_PASSWORD)),
                move(object->get(User::FIELD_DOMAIN))
            );

            ///check if  credential are oh
            if (user != nullptr) {

                ///decode secret for JWT token
                CipherFactory &factory = CipherFactory::defaultFactory();
                Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
                string &&decrypted = cipher->decryptString(user->domain->secret, Cipher::ENC_BASE64);

                ///built JWT token
                auto &&now = Timestamp();

                Token token;
                token.setType("JWT");
                token.setId(move(UUIDGenerator().defaultGenerator().createRandom().toString()));
                token.setIssuer(SERVER_NAME);
                token.payload()
                        .set(User::FIELD_NAME, user->name)
                        .set(User::FIELD_EMAIL, user->email)
                        .set(User::FIELD_JSON_DATA, user->jsonData)
                        .set(User::FIELD_EXPIRATION_DATE, user->expirationDate)
                        .set(User::FIELD_PERMISSIONS, user->permissions)
                        .set(Domain::FIELD_NAME, user->domain->name)
                        .set(Domain::FIELD_EXPIRATION_DATE, user->domain->expirationDate);
                token.setIssuedAt(now);

                ///set expiration token
                if (user->domain->expirationJWT > 0) {
                    auto &&expirationJWT = Timestamp();

                    expirationJWT += Timespan(user->domain->expirationJWT, 0);

                    token.setExpiration(expirationJWT);
                }


                ///update user last login
                user->lastLogin = DateTimeFormatter::format(Timestamp(),  DateTimeFormat::SORTABLE_FORMAT);
                userDAO.update(user);

                ///sign JWT token
                string &&jwt = Signer(decrypted).sign(token, Signer::ALGO_HS256);

//                Token token2("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkb21haW5fZXhwaXJhdGlvbl9kYXRlIjoiIiwiZG9tYWluX25hbWUiOiJwZXJzb25hbC13ZWIiLCJlbWFpbCI6InBhc3N5LmxpbnV4QHpyZXNhLml0IiwiZXhwIjoxNjA2Mjk0NTEyLjQxMzE0MywiZXhwaXJhdGlvbl9kYXRlIjoiIiwiaWF0IjoxNjA2MjkwOTEyLjQxMzAwMSwiaWQiOjEsImpzb25fZGF0YSI6IntcInRlc3RcIjogXCJhIHN0cmluZ1wifSIsImp0aSI6ImM0NzNiZDVlLTU1MTEtNDcxNi05ZWEzLTk1ZDg3Y2Y4YjMwYyIsIm5hbWUiOiJBbnRvbmlvIFNhbHNpIiwicGVybWlzc2lvbnMiOlsgIlJPTEVfQURNSU4iLCAiUk9MRV9BVVRIX1dFQiIgXX0.36rUyr1G2LPLv1vm1RXRE7bRHOKyLpw_DOJuDFkyLq4");
//                std::ostringstream stream;
//                token2.payload().stringify(stream);

                return tuple(true, jwt);
            } else
                return tuple(false, "user not found");
        } else
            throw Poco::Exception("no valid param to attempt login");
    } else
        throw Poco::Exception("body request not contain object");
    return tuple(false, "");
}

bool AuthService::check(const string &scheme, const string &authInfo, const vector<string> &uriSplitted) const noexcept
{
    if (scheme != HEADER_AUTH_BEARER|| authInfo == "")
    {
        return false;
    }


    if (uriSplitted.size() != 4)
        return false;

    auto &&user = userDAO.get(uriSplitted[2], uriSplitted[3]);
    if (user == nullptr)
        return false;

    ///decode secret for JWT token
    CipherFactory &factory = CipherFactory::defaultFactory();
    Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
    string &&decrypted = cipher->decryptString(user->domain->secret, Cipher::ENC_BASE64);

    return jwtCheck(scheme, authInfo, decrypted);
}
