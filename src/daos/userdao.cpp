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

#include <algorithm>    // copy
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include <mariadb++/exceptions.hpp>

#include "domaindao.h"

using namespace auth::daos;

UserPtr UserDAO::deserialize(const result_set_ref &rs, const string &fieldPrefix) const
{
    vector<string> permissions;
    string str = rs->get_string(User::FIELD_EXPIRATION_DATE);
    regex re(",d+");

    sregex_token_iterator
        begin(str.begin(), str.end(), re),
        end;

    copy(begin, end, back_inserter(permissions));

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

void UserDAO::insert(const UserPtr &) const
{

}

void UserDAO::update(const UserPtr &) const
{

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
