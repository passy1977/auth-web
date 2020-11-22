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
using namespace Poco::JSON;

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
using namespace Poco::JWT;

#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
using namespace Poco::Crypto;

using namespace Poco;

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
            auto &&user = userDAO.get(
                move(object->get(User::FIELD_EMAIL)),
                move(object->get(User::FIELD_PASSWORD)),
                move(object->get(User::FIELD_DOMAIN))
            );

            if (user != nullptr) {

                auto &&now = Poco::Timestamp();

                Token token;
                token.getExpiration();
                token.setType("JWT");
                token.setId(move(UUIDGenerator().defaultGenerator().createRandom().toString()));
                token.payload()
                        .set(User::FIELD_ID, user->id)
                        .set(User::FIELD_NAME, user->name)
                        .set(User::FIELD_EMAIL, user->email)
                        .set(User::FIELD_JSON_DATA, user->jsonData)
                        .set(User::FIELD_EXPIRATION_DATE, user->expirationDate)
                        .set(User::FIELD_PERMISSIONS, user->permissions)
                        .set(User::FIELD_ID, user->id)
                        .set(Domain::FIELD_NAME, user->domain->name)
                        .set(Domain::FIELD_EXPIRATION_DATE, user->domain->expirationDate);
                token.setIssuedAt(now);
                if (user->domain->expirationJWT > 0) {
                    auto &&expirationJWT = Timestamp();

                    expirationJWT += Timespan(user->domain->expirationJWT, 0);

                    token.setExpiration(expirationJWT);
                }


                auto &&factory = CipherFactory::defaultFactory();

                // Creates a 256-bit AES cipher
                Cipher* cipher = factory.createCipher(move(CipherKey("AES-256-ECB", Globals::getInstance()->getPassword())));

                string secretDecript = cipher->decryptString(user->domain->secret, Cipher::ENC_BASE64);


                Signer signer(secretDecript);
                std::string jwt = signer.sign(token, Signer::ALGO_HS256);


                delete cipher;

                return tuple(true, jwt);
            } else
                return tuple(false, "user not found");
        } else
            throw Poco::Exception("no valid param to attempt login");
    } else
        throw Poco::Exception("body request not contain object");
    return tuple(false, "");
}
