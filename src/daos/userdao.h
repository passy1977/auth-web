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

#include "dao.h"

#include "../pods/user.h"
using auth::pods::User;

namespace auth::daos
{

/**
 * @brief The UserDAO class CRUD oparetion on User
 */
class UserDAO final : private DAO<User>
{
    static const constexpr inline char* COLLECTION_NAME = "users";
public:
    UserDAO() = delete;
    UserDAO(const connection_ref &connection) :
        DAO(COLLECTION_NAME, connection)
    {}
    AUTH_NO_COPY_NO_MOVE(UserDAO)

    /**
     * @brief update User
     * @exceptions
     */
    void insert(const User::Ptr &) const override;

    /**
     * @brief update User
     * @exceptions
     */
    void update(const User::Ptr &) const override;

    /**
     * @brief get User by id
     * @return UserPtr instance, can be nullptr if not find User
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    User::Ptr get(const string &&email, const string &&password, const string &&domain) const;

    /**
     * @brief get User by id
     * @return UserPtr instance, can be nullptr if not find User
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    User::Ptr get(const string &email, const string &domain) const;

private:
    /**
     * @brief deserialize data from db into User
     * @param first curret valid record result_set_tef
     * @param second optional prefix field
     * @return UserPtr
     * @exceptions mariadb::exception::base, std::out_of_range
     */
    virtual User::Ptr deserialize(const result_set_ref &, const string & = "") const override;
};

}
