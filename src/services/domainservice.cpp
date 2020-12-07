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

#include "domainservice.h"

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
using namespace Poco::JSON;

#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
using namespace Poco::Crypto;

#include "../helpers/domainhelper.h"
using auth::helpers::DomainHelper;

#include "../controllers/httpstatuscontroller.h"
using auth::controllers::HttpStatusController;

#include "../pods/user.h"
using auth::pods::User;

using namespace auth::services;

extern bool jwtCheck(const string &scheme, const string &authInfo, const string &seecret, const User::Ptr &) noexcept;


DomainService::Response DomainService::insert(const string &scheme, const string &authInfo, const string &domainName, const string &&body) const
{

    ///check JWT token and get user info
    auto &&[jsonObj, status, user] = check(scheme, authInfo, domainName);

    ///check if user is nullptr
    if (user.get())
    {

        ///check user role
        if (find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB_WRITE) == user->permissions.end())
        {
            jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
            jsonObj.set(JSON_DATA, "user no has write role");
            return DomainService::Response(jsonObj, HTTPResponse::HTTP_FORBIDDEN);
        }
        else
        {

            ///parse body request
            auto object = Parser().parse(body).extract<Object::Ptr>();

            ///check if domain fields are full fill
            if(
                    object->has(Domain::FIELD_STATUS)
                    && object->has(Domain::FIELD_NAME)
                    && object->has(Domain::FIELD_STATUS)
                    && object->has(Domain::FIELD_EXPIRATION_DATE)
                    && object->has(Domain::FIELD_EXPIRATION_JWT)
                )
            {
                ///encrypt secret for JWT token
                CipherFactory &factory = CipherFactory::defaultFactory();
                Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
                string &&encrypt = cipher->encryptString(object->get(Domain::FIELD_SECRET), Cipher::ENC_BASE64);
                delete cipher;

                ///build object to insert
                int status = object->get(Domain::FIELD_STATUS);
                Domain::Ptr domainToInsert = std::make_shared<Domain>(
                            0,
                            object->get(Domain::FIELD_NAME),
                            encrypt,
                            static_cast<Domain::Status>(status),
                            object->get(Domain::FIELD_EXPIRATION_DATE),
                            object->get(Domain::FIELD_EXPIRATION_JWT)
                            );

                ///insert into db
                domainDAO.insert(domainToInsert);

                ///add data do json obj
                jsonObj.set(JSON_STATUS, JSON_STATUS_OK);
                jsonObj.set(JSON_DATA, move(domainDAO.getLast()->toObject()));

                return DomainService::Response(jsonObj, HTTPResponse::HTTP_OK);
            }
            else
            {
                jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
                jsonObj.set(JSON_DATA, "domain not complete");
                return DomainService::Response(jsonObj, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            }
        }
    }
    else
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "user in token not valid");
        return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
    }
}

DomainService::Response DomainService::update(const string &scheme, const string &authInfo, const string &domainName, const string &&body) const
{
    ///check JWT token and get user info
    auto &&[jsonObj, status, user] = check(scheme, authInfo, domainName);

    ///check if user is nullptr
    if (user.get())
    {

        ///check user role
        if (find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB_WRITE) == user->permissions.end())
        {
            jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
            jsonObj.set(JSON_DATA, "user no has write role");
            return DomainService::Response(jsonObj, HTTPResponse::HTTP_FORBIDDEN);
        }
        else
        {

            ///parse body request
            auto object = Parser().parse(body).extract<Object::Ptr>();

            ///check if domain fields are full fill
            if(
                    object->has(Domain::FIELD_ID)
                    && object->has(Domain::FIELD_STATUS)
                    && object->has(Domain::FIELD_NAME)
                    && object->has(Domain::FIELD_STATUS)
                    && object->has(Domain::FIELD_EXPIRATION_DATE)
                    && object->has(Domain::FIELD_EXPIRATION_JWT)
                )
            {
                ///encrypt secret for JWT token
                CipherFactory &factory = CipherFactory::defaultFactory();
                Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
                string &&encrypt = cipher->encryptString(object->get(Domain::FIELD_SECRET), Cipher::ENC_BASE64);
                delete cipher;

                ///build object to insert
                int status = object->get(Domain::FIELD_STATUS);
                Domain::Ptr domainToInsert = std::make_shared<Domain>(
                            object->get(Domain::FIELD_ID),
                            object->get(Domain::FIELD_NAME),
                            encrypt,
                            static_cast<Domain::Status>(status),
                            object->get(Domain::FIELD_EXPIRATION_DATE),
                            object->get(Domain::FIELD_EXPIRATION_JWT)
                            );

                ///insert into db
                domainDAO.update(domainToInsert);

                ///add data do json obj
                jsonObj.set(JSON_STATUS, JSON_STATUS_OK);
                jsonObj.set(JSON_DATA, move(domainDAO.getLast()->toObject()));

                return DomainService::Response(jsonObj, HTTPResponse::HTTP_OK);
            }
            else
            {
                jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
                jsonObj.set(JSON_DATA, "domain not complete");
                return DomainService::Response(jsonObj, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            }
        }
    }
    else
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "user in token not valid");
        return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
    }
}

DomainService::Response DomainService::get(const string &scheme, const string &authInfo, const string &domainName, const string &domainToGet) const
{
    ///check JWT token and get user info
    auto &&[jsonObj, status, user] = check(scheme, authInfo, domainName);

    ///check if user is nullptr
    if (user.get())
    {
        ///check read user role
        if (find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB_READ) == user->permissions.end())
        {
            jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
            jsonObj.set(JSON_DATA, "user no has read role");
            return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
        }
        else
        {

            auto &&domain = domainDAO.get(domainToGet);

            if (domain.get())
            {

                ///decrypt secret for JWT token
                CipherFactory &factory = CipherFactory::defaultFactory();
                Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
                string &&decrypted = cipher->decryptString(domain->secret, Cipher::ENC_BASE64);
                delete cipher;

                domain->secret = decrypted;

                if (
                        find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB_WRITE) != user->permissions.end()
                        || domain->name == user->domain->name
                   )
                {
                    ///add data do json obj
                    jsonObj.set(JSON_STATUS, JSON_STATUS_OK);
                    jsonObj.set(JSON_DATA, move(domain->toObject()));
                    return DomainService::Response(jsonObj, HTTPResponse::HTTP_OK);
                }
                else
                {
                    jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
                    jsonObj.set(JSON_DATA, "user no has write role");
                    return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
                }

            }
            else
            {
                ///domain not found
                jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
                jsonObj.set(JSON_DATA, "");
                return DomainService::Response(jsonObj, HTTPResponse::HTTP_NOT_FOUND);
            }
        }
    }
    else
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "user in token not valid");
        return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
    }
}


DomainService::Response DomainService::del(const string &scheme, const string &authInfo, const string &domainName, const string &domainToDel) const
{
    ///check JWT token and get user info
    auto &&[jsonObj, status, user] = check(scheme, authInfo, domainName);

    ///check if user is nullptr
    if (user.get())
    {
        ///check write user role
        if (find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB_WRITE) == user->permissions.end())
        {
            jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
            jsonObj.set(JSON_DATA, "user no has write role");
            return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
        }
        else
        {
            ///del domain by name
            jsonObj.set(JSON_STATUS, JSON_STATUS_OK);
            jsonObj.set(JSON_DATA, to_string(domainDAO.del(domainToDel)));
            return DomainService::Response(jsonObj, HTTPResponse::HTTP_OK);
        }
    }
    else
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "user in token not valid");
        return DomainService::Response(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED);
    }
}

tuple<Object, HTTPResponse::HTTPStatus, User::Ptr> DomainService::check(const string &scheme, const string &authInfo, const string &domainName) const
{
    Object jsonObj;

    if (scheme != HEADER_AUTH_BEARER || authInfo == "")
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "auth header not valid");
        return make_tuple(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED, nullptr);
    }


    ///get domain
    auto &&domain = domainDAO.get(domainName);
    if (domain == nullptr)
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        return make_tuple(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED, nullptr);
    }

    ///decrypt secret for JWT token
    CipherFactory &factory = CipherFactory::defaultFactory();
    Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
    string &&decrypted = cipher->decryptString(domain->secret, Cipher::ENC_BASE64);
    delete cipher;

    ///get user info and check tocken
    User::Ptr user = make_shared<User>();
    if (!jwtCheck(scheme, authInfo, decrypted, user))
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "token not valid");
        return make_tuple(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED, nullptr);
    }

    ///add secret decrypted
    domain->secret = move(decrypted);

    user->domain = domain;

    return make_tuple(jsonObj, HTTPResponse::HTTP_OK, user);
}
