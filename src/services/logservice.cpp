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

#include "logservice.h"

#include <Poco/Logger.h>
using Poco::Logger;

#include<Poco/DateTimeFormatter.h>
#include<Poco/DateTimeFormat.h>
using namespace Poco;

#include "../constants.h"

using namespace auth::services;


LogService::LogService(const AutoPtr<IniFileConfiguration> &config) noexcept : config(config)
{
    ///configure log if needed
    if (config->has(CONFIG_PATH_LOG) && config->has(CONFIG_LOG_ROTATION))
    {
        AutoPtr<SimpleFileChannel> logChannel = AutoPtr<SimpleFileChannel>(new SimpleFileChannel);
        logChannel->setProperty("path", move(config->getString(CONFIG_PATH_LOG)));
        logChannel->setProperty("rotation", move(config->getString(CONFIG_LOG_ROTATION)));
        Logger::root().setChannel(logChannel);
    }
}


void LogService::write(LogService::Level &&level, unsigned int line, const string &source, string message) const noexcept
{

    string sourceCopy = source;
//    string delimiter = "/";
//    size_t pos = 0;
//    string token;
//    while ((pos = source.find(delimiter)) != string::npos) {
//        token = source.substr(0, pos);
//        sourceCopy.erase(0, pos + delimiter.length());
//    }

    ///message building
    string ret = DateTimeFormatter::format(Timestamp(),  DateTimeFormat::SORTABLE_FORMAT);
    ret += " line:";
    ret += std::to_string(line);
    ret += " source:";
    ret += sourceCopy;
    ret += ":";
    ret += message;

    switch (level) {
    case LogService::Level::TRACE:
        poco_trace(Logger::root(), ret);
        break;
    case LogService::Level::DBG:
        poco_debug(Logger::root(), ret);
        break;
    case LogService::Level::INFO:
        poco_information(Logger::root(), ret);
        break;
    case LogService::Level::WARN:
        poco_warning(Logger::root(), ret);
        break;
    case LogService::Level::ERROR:
        poco_error(Logger::root(), ret);
        break;
    case LogService::Level::FATAL:
        poco_fatal(Logger::root(), ret);
        break;
    }

}
