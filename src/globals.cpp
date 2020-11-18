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

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"
using namespace Poco::MongoDB;

using namespace auth;

void Globals::init(const std::string &path) noexcept
{

    try {
        ///init configuration
        config = AutoPtr<IniFileConfiguration>(new IniFileConfiguration(path));

        log = new LogService(config);

        ///build connection string for MongoDb
        string uri = "mongodb://";
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

        ///copnnect to MongoDB
        Connection::SocketFactory sf;
        connection.connect(uri, sf);


    }  catch (Poco::Exception &e) {
        AUTH_GLOBAL_LOG(ERROR, e.message());
    }

}
