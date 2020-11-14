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

namespace auth::dao
{

/**
 * @brief The DAOUser class is a singleton for CRUD operation on User struct
 * @author Antonio Salsi
 */
template<typename T>
class DAOUser final
{

    const Connection &connection;

public:
    inline explicit DAOUser(const Connection &connection) :
        connection(connection)
    {}

    AUTH_NO_COPY_NO_MOVE(DAOUser)

    /**
     * @brief insert T
     * @exceptions
     */
    void insert(T &&t) const
    {
        insert(t);
    }

    /**
     * @brief insert T
     * @exceptions
     */
    void insert(const T &) const
    {

    }


    /**
     * @brief update T
     * @exceptions
     */
    void update(T &&t) const
    {
        update(t);
    }


    /**
     * @brief update T
     * @exceptions
     */
    void update(const T &) const
    {

    }

    /**
     * @brief delete T
     * @exceptions
     */
    inline void remove(T &&t) const
    {
        remove(t);
    }

    /**
     * @brief delete T
     * @exceptions
     */
    void remove(const T &) const
    {

    }

protected:
    string collection;

};

}
