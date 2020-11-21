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
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    void insert(shared_ptr<T> &&t) const
    {
        insert(t);
    }

    /**
     * @brief insert T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    virtual void insert(const shared_ptr<T> &t) const = 0;



    /**
     * @brief update T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    void update(shared_ptr<T> &&t) const
    {
        update(t);
    }


    /**
     * @brief update T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    virtual void update(const shared_ptr<T> &) const = 0;

    /**
     * @brief delete T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    inline void remove(shared_ptr<T> &&t) const
    {
        remove(t);
    }

    /**
     * @brief delete T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    void remove(const shared_ptr<T> &t) const
    {
        if constexpr (!std::is_same_v<T, auth::pods::User> && !std::is_same_v<T, auth::pods::Domain> )
        {
            string query = "DELETE FROM " + table + " WHERE id = " + t.id;
            AUTH_GLOBAL_LOG(DBG, query);
            connection->query(query);
        }
        else
            throw Poco::Exception("Not valid object");
    }

    /**
     * @brief get T by id
     * @return T instance, can be nullptr if not find T
     * @exceptions Poco::Exception, mariadb::exception::base, std::out_of_range
     */
    shared_ptr<T> get(uint32_t id) const
    {
        shared_ptr<T> t = nullptr;
        if constexpr (is_same_v<T, auth::pods::User> || is_same_v<T, auth::pods::Domain> )
        {
            string fieldPrefix = "";
            string query = "SELECT * FROM " + table + " WHERE id = " + to_string(id);
            if constexpr (is_same_v<T, auth::pods::User>)
            {
                query = "SELECT a.*, b.id b_id, b.name b_name, b.secret b_secret, b.status b_status, b.expiration b_expiration  FROM users a "
                        "LEFT JOIN domains b ON a.id_domain  = b.id "
                        "WHERE a.id = " + to_string(id);
                fieldPrefix = "b_";
            }
            AUTH_GLOBAL_LOG(DBG, query);
            auto && rs = connection->query(query);

            if (rs->next())
            {
                t = deserialize(rs, fieldPrefix);
            }

        }
        else
            throw Poco::Exception("Not valid object");
        return t;
    }

protected:
    string collection;

    /**
     * @brief deserialize data from db into pod
     * @param rs curret valid record result_set_tef
     * @return shared_pointer<pod>
     * @exceptions mariadb::exception::base, std::out_of_range
     */
    virtual shared_ptr<T> deserialize(const result_set_ref &, const string &fieldPrefix = "") const = 0;
};

}
