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


DomainService::Response DomainService::insert(const string &scheme, const string &authInfo, const string &&body) const
{
//    auto &&[jsonObj, object, status] = before(scheme, authInfo, response, body);
//    if (status != HTTPServerResponse::HTTP_OK)
//    {
//        return jsonObj;
//    }


//    response.setStatus(to_string(status));

//    Domain::Ptr domainToInsert = std::make_shared<Domain>(
//                0,
//                object->get(Domain::FIELD_NAME),
//                object->get(Domain::FIELD_SECRET),
//                static_cast<Domain::Status>(status),
//                object->get(Domain::FIELD_EXPIRATION_DATE),
//                object->get(Domain::FIELD_EXPIRATION_JWT)
//                );

//    domainDAO.insert(domainToInsert);

//    jsonObj.set(JSON_DATA, move(DomainHelper::toJson(domainDAO.getLast())));

//    return jsonObj;
}

DomainService::Response DomainService::update(const string &scheme, const string &authInfo, const string &&body) const
{

//    if (scheme != HEADER_AUTH_BEARER|| authInfo == "")
//    {
//        response.setStatus(move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_UNAUTHORIZED))));
//        return HttpStatusController::buildObject(HTTPResponse::HTTP_UNAUTHORIZED, JSON_STATUS_ERROR, "auth header not valid");
//    }


//    ///parse body request
//    auto object = Parser().parse(body).extract<Object::Ptr>();
//    if (!object->has(Domain::SENDER))
//    {
//        response.setStatus(move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_UNAUTHORIZED))));
//        return HttpStatusController::buildObject(HTTPResponse::HTTP_UNAUTHORIZED, JSON_STATUS_ERROR, "sender not found");
//    }

//    ///get domain
//    auto &&domain = domainDAO.get(move(object->get(Domain::SENDER).toString()));
//    if (domain == nullptr)
//    {
//        response.setStatus(move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_UNAUTHORIZED))));
//        return HttpStatusController::buildObject(HTTPResponse::HTTP_UNAUTHORIZED, JSON_STATUS_ERROR, "domain sender not found");
//    }

//    ///decode secret for JWT token
//    CipherFactory &factory = CipherFactory::defaultFactory();
//    Cipher *cipher = factory.createCipher(CipherKey("aes-256-ecb", Globals::getInstance()->getPassword()));
//    string &&decrypted = cipher->decryptString(domain->secret, Cipher::ENC_BASE64);

//    ///check il JWT is valid token
//    if (!jwtCheck(scheme, authInfo, decrypted))
//    {
//        response.setStatus(move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_UNAUTHORIZED))));
//        return HttpStatusController::buildObject(HTTPResponse::HTTP_UNAUTHORIZED, JSON_STATUS_ERROR, "JWT toke nont valid");
//    }

//    ///set http response status
//    response.setStatus(move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_OK))));

//    ///build json response object
//    Object jsonObj;
//    jsonObj.set(JSON_HTTP_STATUS, move(to_string(static_cast<uint16_t>(HTTPResponse::HTTP_OK))));
//    jsonObj.set(JSON_STATUS, JSON_STATUS_OK);


//    int status = object->get(Domain::FIELD_STATUS);

//    Domain::Ptr domainToInsert = std::make_shared<Domain>(
//                0,
//                object->get(Domain::FIELD_NAME),
//                object->get(Domain::FIELD_SECRET),
//                static_cast<Domain::Status>(status),
//                object->get(Domain::FIELD_EXPIRATION_DATE),
//                object->get(Domain::FIELD_EXPIRATION_JWT)
//                );


//    domainDAO.insert(domainToInsert);

//    domainDAO.getLast();

//    //jsonObj.set(JSON_DATA, );
//    return jsonObj;
}

DomainService::Response DomainService::get(const string &scheme, const string &authInfo, const string &domainName) const
{
    auto &&[jsonObj, status, user] = before(scheme, authInfo, domainName);

    if (user != nullptr)
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_OK);
        jsonObj.set(JSON_DATA, user->domain->toObject());
    }

    return DomainService::Response(jsonObj, HTTPResponse::HTTP_OK);
}

tuple<Object, HTTPResponse::HTTPStatus, User::Ptr> DomainService::before(const string &scheme, const string &authInfo, const string &domainName) const
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

    ///decode secret for JWT token
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

    if (std::find(user->permissions.begin(), user->permissions.end(), ROLE_AUTH_WEB) == user->permissions.end())
    {
        jsonObj.set(JSON_STATUS, JSON_STATUS_ERROR);
        jsonObj.set(JSON_DATA, "user no has role");
        return make_tuple(jsonObj, HTTPResponse::HTTP_UNAUTHORIZED, nullptr);
    }

    user->domain = domain;

    return make_tuple(jsonObj, HTTPResponse::HTTP_OK, user);

}
