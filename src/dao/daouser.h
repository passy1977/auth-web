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

#include "../globals.h"
#include "../pods/user.h"
using namespace auth::pods;

namespace auth::dao
{

/**
 * @brief The DAOUser class is a singleton for CRUD operation on user struct
 */
class DAOUser final
{
public:
    DAOUser() = default;
    DAOUser(const DAOUser&) = delete;
    DAOUser& operator = (const DAOUser&) = delete;
    DAOUser(DAOUser&&) = delete;
    DAOUser& operator = (DAOUser&&) = delete;

    static constexpr const char * COLLECTION = "users";

    static inline const shared_ptr<DAOUser> &getInstance() noexcept
    {
        static shared_ptr<DAOUser> instance;
        if (!instance) {
            instance = make_shared<DAOUser>();
        }
        return instance;
    }

    /**
     * @brief insert user
     */
    void insert(const SPUser &) const;

    /**
     * @brief update user
     */
    void update(const SPUser &) const;

    /**
     * @brief delete user
     */
    void remove(const SPUser &) const;



};

}
