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
using namespace std;

namespace auth::pods
{

/**
 * @brief The Domain struct data rappresentation of Domain
 * @author Antonio Salsi
 */
struct Domain final
{

    static inline constexpr const char *FIELD_ID = "id";
    static inline constexpr const char *FIELD_NAME = "name";
    static inline constexpr const char *FIELD_SECRET = "secret";
    static inline constexpr const char *FIELD_STATUS = "status";
    static inline constexpr const char *FIELD_EXPIRATION_DATE = "expiration";

    /**
     * @brief The Status enum status of Domain
     */
    enum class Status : u_int8_t {
        UNACTIVE = 0,
        ACTIVE = 1,
        LOCK = 2
    };

    /**
     * @brief id PK
     */
    int id;

    /**
     * @brief domain name
     */
    string name;

    /**
     * @brief seecret password for JWT
     */
    string seecret;

    /**
     * @brief last login date,
     * @details if nullptr never loged in
     */
    Status status;

    /**
     * @brief lastLogin expiration date permit to maintain allows the user to log in until a specific data
     * @details if nullptr the feature i disable
     */
    string expirationDate;

    inline Domain(
            int id,
            string name,
            string seecret,
            Status status,
            string expirationDate
            ) :
        id(move(id)),
        name(move(name)),
        seecret(move(seecret)),
        status(move(status)),
        expirationDate(move(expirationDate))
    {}

};

/**
 * @brief SPDomain shared pointer of Domain
 */
typedef shared_ptr<Domain> DomainPtr;

}
