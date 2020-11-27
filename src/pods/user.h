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

#include <memory>
#include <string>
#include <vector>
using namespace std;

#import "domain.h"

namespace auth::pods
{

/**
 * @brief The Domain struct data rappresentation of User
 */
struct User final
{
    static inline constexpr const char *FIELD_ID = "id";
    static inline constexpr const char *FIELD_NAME = "name";
    static inline constexpr const char *FIELD_EMAIL = "email";
    static inline constexpr const char *FIELD_PASSWORD = "password";
    static inline constexpr const char *FIELD_JSON_DATA = "json_data";
    static inline constexpr const char *FIELD_PERMISSIONS = "aud";
    static inline constexpr const char *FIELD_STATUS = "status";
    static inline constexpr const char *FIELD_LAST_LOGIN = "last_login";
    static inline constexpr const char *FIELD_EXPIRATION_DATE = "expiration_date";
    static inline constexpr const char *FIELD_ID_DOMAIN = "id_domain";
    static inline constexpr const char *FIELD_DOMAIN = "domain";

    /**
     * @brief SPUser shared pointer of User
     */
    typedef shared_ptr<User> Ptr;

    /**
     * @brief The Status enum status of User
     */
    enum class Status : uint8_t {
        UNACTIVE = 0,
        ACTIVE = 1,
        LOCK = 2
    };

    /**
     * @brief id PK
     */
    uint32_t id = 0;

    /**
     * @brief user name like Antonio Salsi
     */
    string name = "";

    /**
     * @brief email composite key with domain
     */
    string email = "";

    string password = "";

    /**
     * @brief additional data in json format
     */
    string jsonData = "";

    /**
     * @brief permissions to service access
     */
    vector<string> permissions = {};

    /**
     * @brief user status
     */
    Status status = Status::ACTIVE;

    /**
     * @brief last login date,
     * @details if nullptr never loged in
     */
    string lastLogin = "";

    /**
     * @brief lastLogin expiration date permit to maintain allows the user to log in until a specific data
     * @details if nullptr the feature i disable
     */
    string expirationDate = "";

    /**
     * @brief composite key with email
     * @details if nullptr the feature i disable
     */
    Domain::Ptr domain = nullptr;

    User() = default;

    inline User(
            uint32_t id,
            string name,
            string email,
            string password,
            string jsonData,
            vector<string> permissions,
            Status status,
            string lastLogin,
            string expirationDate,
            Domain::Ptr domain
            ) :
        id(move(id)),
        name(move(name)),
        email(move(email)),
        password(move(password)),
        jsonData(move(jsonData)),
        permissions(move(permissions)),
        status(move(status)),
        lastLogin(move(lastLogin)),
        expirationDate(move(expirationDate)),
        domain(move(domain))
    {}

};

}


