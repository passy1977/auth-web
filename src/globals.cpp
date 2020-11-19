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

#include "globals.h"

#include <memory>
using std::move;

#include <Poco/AutoPtr.h>
using Poco::AutoPtr;

using namespace auth;

bool Globals::init(const std::string &path) noexcept
{

    try {
        ///init configuration
        config = AutoPtr<IniFileConfiguration>(new IniFileConfiguration(path));

        log = new LogService(config);

//        auto && host = config->getString(CONFIG_DB_HOST);
//        auto && port = config->getInt(CONFIG_DB_PORT);
//        auto && user = config->getString(CONFIG_DB_USER);
//        auto && password = config->getString(CONFIG_DB_PASSWORD);

//        AUTH_GLOBAL_LOG(DBG, "host:" + host);
//        AUTH_GLOBAL_LOG(DBG, "port:" + to_string(port));
//        AUTH_GLOBAL_LOG(DBG, "user:" + user);
//        AUTH_GLOBAL_LOG(DBG, "password:" + password);

//        ///copnnect to MySql
//        session = new mysqlx::Session(host, port, user, password);

        ///build connection string for MongoDb
        string uri = "mysqlx://";
        uri += config->getString(CONFIG_DB_USER);
        uri += ":";
        uri += config->getString(CONFIG_DB_PASSWORD);
        uri += "@";
        uri += config->getString(CONFIG_DB_HOST);
        uri += ":";
        uri += config->getString(CONFIG_DB_PORT);
        uri += "/";
        uri += config->getString(CONFIG_DB_DATABASE);

        AUTH_GLOBAL_LOG(DBG, uri);

        client = make_shared<mysqlx::Client>(uri);


    } catch (const mysqlx::abi2::r0::Error &e) {
        AUTH_GLOBAL_LOG(ERROR, e.what());
        return false;
    } catch (Poco::Exception &e) {
        AUTH_GLOBAL_LOG(ERROR, e.message());
        return false;
    }

    return true;
}
