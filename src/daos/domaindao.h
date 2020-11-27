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

#include "../pods/domain.h"
using auth::pods::Domain;


namespace auth::daos
{

///forward declaration
class UserDAO;

/**
 * @brief The DomainDAO class CRUD oparetion on Domain
 */
class DomainDAO final : private DAO<Domain>
{
    static const constexpr inline char* COLLECTION_NAME = "domains";

public:
    DomainDAO() = delete;
    explicit DomainDAO(const connection_ref &connection) :
        DAO(COLLECTION_NAME, connection)
    {

    }
    AUTH_NO_COPY_NO_MOVE(DomainDAO)

    /**
     * @brief update T
     * @exceptions mariadb::exception::base, std::out_of_range
     */
    void insert(const Domain::Ptr &) const override;

    /**
     * @brief update T
     * @exceptions mariadb::exception::base, std::out_of_range
     */
    void update(const Domain::Ptr &) const override;

private:

    /**
     * @brief deserialize data from db into Domain
     * @param first curret valid record result_set_tef
     * @param second optional prefix field
     * @return DomainPtr
     * @exceptions mariadb::exception::base, std::out_of_range
     */
    virtual Domain::Ptr deserialize(const result_set_ref &, const string & = "") const override;

    friend UserDAO;
};

}
