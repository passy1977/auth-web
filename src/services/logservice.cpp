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

//#include <Poco/Logger.h>
//using Poco::Logger;
#include<iostream>
using namespace std;

#include<Poco/DateTimeFormatter.h>
#include<Poco/DateTimeFormat.h>
using namespace Poco;

#include "../constants.h"

using namespace auth::services;


LogService::LogService(const AutoPtr<IniFileConfiguration> &config) noexcept : config(config)
{
    ///configure log if needed
//    if (config->has(CONFIG_PATH_LOG) && config->has(CONFIG_LOG_ROTATION))
//    {
//        AutoPtr<SimpleFileChannel> logChannel = AutoPtr<SimpleFileChannel>(new SimpleFileChannel);
//        logChannel->setProperty("path", move(config->getString(CONFIG_PATH_LOG)));
//        logChannel->setProperty("rotation", move(config->getString(CONFIG_LOG_ROTATION)));
//        Logger::root().setChannel(logChannel);
//    }

}


void LogService::write(LogService::Level &&level, unsigned int line, const string & source, string message) const noexcept
{

    ///message building
    string src;
    size_t position = source.rfind('/');
    if (position) {
        position++;
        src = source.substr(position, source.size());
    }

    string strLevel;
    switch (level) {
    case LogService::Level::TRACE:
        strLevel = "TRACE";
        break;
    case LogService::Level::DBG:
        strLevel = "DEBUG";
        break;
    case LogService::Level::INFO:
        strLevel = "INFO";
        break;
    case LogService::Level::WARN:
        strLevel = "WARN";
        break;
    case LogService::Level::ERROR:
        strLevel = "ERROR";
        break;
    case LogService::Level::FATAL:
        strLevel = "FATAL";
        break;
    }
    string ret = DateTimeFormatter::format(Timestamp(),  DateTimeFormat::SORTABLE_FORMAT);
    ret += " level:";
    ret += strLevel;
    ret += " line:";
    ret += std::to_string(line);
    ret += " source:";
    ret += src;
    ret += " message:";
    ret += message;

    if (level == LogService::Level::ERROR || level == LogService::Level::FATAL)
    {
        cerr << ret << endl;
    }
    else
    {
        cout << ret << endl;
    }
}
