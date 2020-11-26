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

#include "jwthelper.h"

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
using namespace Poco::JWT;
using Timestamp = Poco::Timestamp;

#include "pods/domain.h"
#include "pods/user.h"
using namespace auth::pods;

using namespace auth;

bool jwtCheck(const string &scheme, const string &authInfo, const string &domain) noexcept
{
    if (scheme != "Bearer" || authInfo == "")
    {
        return false;
    }

    Token token;
    Signer signer;
    signer.tryVerify(authInfo, token);


    auto &&expiration = token.getExpiration();


    if (Timestamp() > expiration)
    {
        return false;
    }


    if (token.payload().has(User::FIELD_DOMAIN))
    {
        if (token.payload().get(User::FIELD_DOMAIN).toString() != domain)
        {
            return false;
        }
    }

    return true;
}
