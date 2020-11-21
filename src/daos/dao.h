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

#include <map>
#include <vector>
#include <iostream>
using namespace std;

#include "../globals.h"

namespace auth::pods {

class Domain;
class User;

}


namespace auth::daos
{



/**
 * @brief The DAOUser class is a singleton for CRUD operation on User struct
 * @author Antonio Salsi
 */
template<typename T>
class DAO
{

protected:

    const string &table;

    const connection_ref &connection;


public:
    DAO() = delete;
    inline explicit DAO(const string &table, const connection_ref &connection)
        : table(table),
          connection(connection)
        {

        }


    AUTH_NO_COPY_NO_MOVE(DAO)

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
    void insert(const T &t) const
    {
//        auto &&insertRequest = db.createInsertRequest(collectionName);

        if constexpr (!std::is_same_v<T, auth::pods::User> && !std::is_same_v<T, auth::pods::Domain> )
        {
//            insertRequest =  t.toDocument();
//            connection.sendRequest(insertRequest);

//            string lastError = db.getLastError(connection);
//            if (!lastError.empty())
//            {
//                std::cout << "Last Error: " << db.getLastError(connection) << std::endl;
//            }
        }
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




    /**
     * @brief deserialize data from db into pod
     * @param rs curret valid record result_set_tef
     * @return shared_pointer<pod>
     */
    virtual shared_ptr<T> deserialize(const result_set_ref &) const = 0;

protected:
    string collection;

};

}
