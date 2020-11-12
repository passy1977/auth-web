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

#include <Poco/Logger.h>
using Poco::Logger;

#include <Poco/SimpleFileChannel.h>
using Poco::SimpleFileChannel;

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
    AutoPtr<SimpleFileChannel> channel(new SimpleFileChannel);
    channel->setProperty("path", PATH_LOG);
    channel->setProperty("rotation", LOG_ROTATION);
    Logger::root().setChannel(channel);

    //init configuration
    try {
        config = AutoPtr<IniFileConfiguration>(new IniFileConfiguration(path));

        db = Connection(config->getString(CONFIG_DB_HOST), config->getInt(CONFIG_DB_PORT));
    }  catch (Poco::Exception &e) {
        poco_warning_f1(Logger::root(), "Read configuration error, load deafult params", e);
    }

}
