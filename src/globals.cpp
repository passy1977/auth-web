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

#include <Poco/FileStream.h>
#include <Poco/CountingStream.h>
#include <Poco/TeeStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/AutoPtr.h>
using namespace Poco;


#include <mariadb++/exceptions.hpp>
using mariadb::exception::base;


using namespace auth;

bool Globals::init(const std::string &path) noexcept
{

    try
    {
        ///init configuration
        config = AutoPtr<IniFileConfiguration>(new IniFileConfiguration(path));

        ///init log service
        log = new LogService(config);

        ///init mysql
        auto &&host = config->getString(CONFIG_DB_HOST);
        auto &&port = config->getInt(CONFIG_DB_PORT);
        auto &&user = config->getString(CONFIG_DB_USER);
        auto &&password = config->getString(CONFIG_DB_PASSWORD);
        auto &&database = config->getString(CONFIG_DB_DATABASE);

        AUTH_GLOBAL_LOG(DBG, "host:" + host);
        AUTH_GLOBAL_LOG(DBG, "port:" + to_string(port));
        AUTH_GLOBAL_LOG(DBG, "user:" + user);
        AUTH_GLOBAL_LOG(DBG, "password num char:" + to_string(password.size()));
        AUTH_GLOBAL_LOG(DBG, "database:" + database);

        auto &&account = account::create(host,
                                        user,
                                        password,
                                        database,
                                        port);

        this->connection = connection::create(account);

        ///copnnect to MySql
        if (!connection->connect())
        {
            AUTH_GLOBAL_LOG(FATAL, connection->error());
            connection->disconnect();
            connection = nullptr;
            return false;
        }


        AUTH_GLOBAL_LOG(DBG, "server version:" + string(mysql_get_client_info()));

        ///load creation script
        if (config->has(CONFIG_DB_SCRIPT))
        {
            /// check il db exist
            if (connection->query(CHECK_DB))
            {
                AUTH_GLOBAL_LOG(FATAL, connection->error());
                connection->disconnect();
                connection = nullptr;
                return false;
            }


            try
            {
                auto &&res = connection->query(CHECK_DB);
                if (!res->next())
                {
                    ///read sql script
                    string script;
                    FileInputStream istr(move(config->getString(CONFIG_DB_SCRIPT)));
                    CountingInputStream countingIstr(istr);
                    StreamCopier::copyToString(countingIstr, script);
                    istr.close();

                    ///execute script
                    connection->query(script);
                }
            }
            catch (const base &e)
            {
                AUTH_GLOBAL_LOG(FATAL, connection->error());
                return false;
            }
        }

    }
    catch (const mariadb::exception::connection &e)
    {
        AUTH_GLOBAL_LOG(FATAL, e.what());
        return false;
    }
    catch (Poco::Exception &e)
    {
        AUTH_GLOBAL_LOG(FATAL, e.message());
        return false;
    }

    return true;
}
