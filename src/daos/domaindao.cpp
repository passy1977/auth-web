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

#include "domaindao.h"

#include <mariadb++/exceptions.hpp>

using namespace auth::daos;

DomainPtr DomainDAO::deserialize(const result_set_ref &rs) const
{
    return make_shared<Domain>(
                rs->get_signed32(Domain::FIELD_ID),
                rs->get_string(Domain::FIELD_NAME),
                rs->get_string(Domain::FIELD_SECRET),
                static_cast<Domain::Status>(rs->get_signed8(Domain::FIELD_STATUS)),
                rs->get_string(Domain::FIELD_EXPIRATION_DATE)
                );
}

void DomainDAO::testDb() const
{
    auto &&stm = connection->create_statement("SELECT * FROM domains");

    result_set_ref rs = stm->query();

    while (rs->next())
    {




        try
        {
//            string name = rs->get_string("name");
//            AUTH_GLOBAL_LOG(DBG, name);

//            name = rs->get_string("name");
//            AUTH_GLOBAL_LOG(DBG, name);
            auto && domain = deserialize(rs);



            AUTH_GLOBAL_LOG(DBG, to_string(domain->id));




        }
        AUTH_CATCH_EXCEPTIONS
    }
}
