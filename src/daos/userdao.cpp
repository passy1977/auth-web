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

#include "userdao.h"

#include "Poco/StringTokenizer.h"
using Poco::StringTokenizer;

#include <mariadb++/exceptions.hpp>

#include "domaindao.h"

using namespace auth::daos;

UserPtr UserDAO::deserialize(const result_set_ref &rs, const string &fieldPrefix) const
{
    vector<string> permissions;
    auto &&str = rs->get_string(User::FIELD_PERMISSIONS);
    StringTokenizer token(str,
                       ",",
                       StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY
                       );


    for (auto s : token)
    {
        permissions.push_back(s);
    }




    DomainPtr domain = nullptr;
    if (fieldPrefix != "")
    {
        DomainDAO domainDAO(connection);
        domain = domainDAO.deserialize(rs, fieldPrefix);
    }

    return make_shared<User>(
                rs->get_unsigned32(User::FIELD_ID),
                rs->get_string(User::FIELD_NAME),
                rs->get_string(User::FIELD_EMAIL),
                rs->get_string(User::FIELD_PASSWORD),
                rs->get_string(User::FIELD_JSON_DATA),
                permissions,
                static_cast<User::Status>(rs->get_unsigned8(User::FIELD_STATUS)),
                rs->get_string(User::FIELD_LAST_LOGIN),
                rs->get_string(User::FIELD_EXPIRATION_DATE),
                domain
                );
}

void UserDAO::insert(const UserPtr &user) const
{
    string query("INSERT INTO users (");
           query += User::FIELD_NAME;
           query += ", ";
           query += User::FIELD_EMAIL;
           query += ", ";
           query += User::FIELD_PASSWORD;
           query += ", ";
           query += User::FIELD_JSON_DATA;
           query += ", ";
           query += User::FIELD_PERMISSIONS;
           query += ", ";
           query += User::FIELD_STATUS;
           query += ", ";
           query += User::FIELD_LAST_LOGIN;
           query += ", ";
           query += User::FIELD_EXPIRATION_DATE;
           query += ", ";
           query += User::FIELD_ID_DOMAIN;
           query += ", ";
           query += ") values (?, ?, ?, ?, ?, ?, ?, ?, ?) ";

           AUTH_GLOBAL_LOG(DBG, query);

    string permissions;

    for (auto &&s : user->permissions)
    {
        permissions += s;
        permissions += ",";
    }

    if (permissions != "")
        permissions = permissions.substr(0, permissions.size() - 1);

    statement_ref stmt = connection->create_statement(query);
    stmt->set_string(0, user->name);
    stmt->set_string(1, user->email);
    stmt->set_string(2, user->password);
    stmt->set_string(3, user->jsonData);
    stmt->set_string(4, permissions);
    stmt->set_unsigned8(5, static_cast<uint8_t>(user->status));
    stmt->set_string(6, user->lastLogin);
    stmt->set_string(7, user->expirationDate);
    stmt->set_unsigned32(9, user->domain->id);
    stmt->insert();
}

void UserDAO::update(const UserPtr &user) const
{
    string query("UPDATE users SET ");
           query += User::FIELD_NAME;
           query += " = ?, ";
           query += User::FIELD_EMAIL;
           query += " = ?, ";
           query += User::FIELD_PASSWORD;
           query += " = ?, ";
           query += User::FIELD_JSON_DATA;
           query += " = ?, ";
           query += User::FIELD_PERMISSIONS;
           query += " = ?, ";
           query += User::FIELD_STATUS;
           query += " = ?, ";
           query += User::FIELD_LAST_LOGIN;
           query += " = ?, ";
           query += User::FIELD_EXPIRATION_DATE;
           query += " = ?, ";
           query += User::FIELD_ID_DOMAIN;
           query += " = ? ";
           query += " WHERE id = ?";

           AUTH_GLOBAL_LOG(DBG, query);

    string permissions;

    for (auto &&s : user->permissions)
    {
        permissions += s;
        permissions += ",";
    }

    if (permissions != "")
        permissions = permissions.substr(0, permissions.size() - 1);

    statement_ref stmt = connection->create_statement(query);
    stmt->set_string(0, user->name);
    stmt->set_string(1, user->email);
    stmt->set_string(2, user->password);
    stmt->set_string(3, user->jsonData);
    stmt->set_string(4, permissions);
    stmt->set_unsigned8(5, static_cast<uint8_t>(user->status));
    stmt->set_string(6, user->lastLogin);
    stmt->set_string(7, user->expirationDate);
    stmt->set_unsigned32(8, user->domain->id);
    stmt->set_unsigned32(9, user->id);
    stmt->insert();
}

auth::pods::UserPtr UserDAO::get(const string email, const string password, const string domain) const
{
    UserPtr ret = nullptr;

    string query = "SELECT a.*, b.id b_id, b.domain_name b_domain_name, b.domain_secret b_domain_secret, b.domain_status b_domain_status, b.domain_expiration_date b_domain_expiration_date, b.domain_expiration_jwt b_domain_expiration_jwt FROM users a "
                   "LEFT JOIN domains b ON a.id_domain  = b.id "
                   "WHERE a.email = ? "
                   "AND a.password = ? "
                   "AND domain_name = ?";
    string fieldPrefix = "b_";

    AUTH_GLOBAL_LOG(DBG, query);

    auto &&stm = connection->create_statement(query);
    stm->set_string(0, email);
    stm->set_string(1, password);
    stm->set_string(2, domain);

    auto &&rs = stm->query();
    if (rs->next())
    {
        ret = deserialize(rs, fieldPrefix);
    }


    return ret;
}
