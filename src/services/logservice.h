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

#include<string>
using std::string;

#include <Poco/SimpleFileChannel.h>
using Poco::SimpleFileChannel;

#include <Poco/AutoPtr.h>
using Poco::AutoPtr;

#include <Poco/Util/IniFileConfiguration.h>
using namespace Poco::Util;

namespace auth::services
{


/**
 * @brief The LogService class Log helper fo berer log managing
 * @author Antonio Salsi
 */
class LogService
{

    const AutoPtr<IniFileConfiguration> &config;

public:

    /**
     * @brief The LogLevel enum to manage log sowing
     * @author Antonio Salsi
     */
    enum class Level {
        TRACE,
        DBG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    LogService(const AutoPtr<IniFileConfiguration> &) noexcept;

    /**
     * @brief LogService::write message to log
     * @param level of log
     * @param line in soure
     * @param source file
     * @param message to wite
     */
    void write(Level &&level, unsigned int line, const string &source, string message) const noexcept;
};


//unsigned int line, const string& source, string message
}
