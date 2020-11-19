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

#define AUTH_GLOBAL_LOG(_level_, msg) Globals::getInstance()->getLog()->write(LogService::Level::_level_, __LINE__, __FILE__, msg)

#include <memory>
using namespace std;

#include <Poco/AutoPtr.h>
using Poco::AutoPtr;

#include <Poco/Util/IniFileConfiguration.h>
using namespace Poco::Util;

#include <mysqlx/xdevapi.h>

#include "services/logservice.h"
using auth::services::LogService;

#include "constants.h"

namespace auth
{

/**
 * @brief The Globals conatin global param data container
 * @author Antonio Salsi
 */
class Globals final
{

    AutoPtr<IniFileConfiguration> config;

    shared_ptr<mysqlx::Client> client;

    LogService *log = nullptr;

public:
    Globals() = default;
    inline ~Globals() noexcept
    {
        delete log;
        log = nullptr;

        client->close();
    }

    AUTH_NO_COPY_NO_MOVE(Globals)

    /**
     * @brief getInstance singleton patern
     * @return
     */
    static inline const shared_ptr<Globals> &getInstance() noexcept
    {
        static shared_ptr<Globals> instance;
        if (!instance) {
            instance = make_shared<Globals>();
        }
        return instance;
    }

    /**
     * @brief init for inializate subsystem like MongoDb connection, logs, load gonfig file, ...
     */
    bool init(const string & = PATH_CONFIG) noexcept;

    /**
     * @brief getConfig get instance to config file file
     * @return config file reference
     */
    inline const AutoPtr<IniFileConfiguration> &getConfig() const noexcept
    {
        return config;
    }

    /**
     * @brief getConnection get MySql connection
     * @return MySql connection
     */
    inline shared_ptr<mysqlx::Client> getClient() const noexcept
    {
        return client;
    }

    /**
     * @brief getLog get log
     * @return
     */
    inline const LogService *getLog() const noexcept
    {
        return log;
    }

};

}


