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

#include "lockservice.h"

#include<Poco/Process.h>
#include<Poco/FileStream.h>
#include <Poco/CountingStream.h>
#include <Poco/TeeStream.h>
#include<Poco/StreamCopier.h>
#include<Poco/DateTimeFormatter.h>
#include<Poco/DateTimeFormat.h>
using namespace Poco;

#include "../constants.h"

using namespace auth::services;

LockService::LockService() noexcept :
    lockFile(FILE_LOCK_SERVICE)
{
    ///if fie exist then another instance run
    if (lockFile.exists())
    {
        ///read creation lock file
        auto &&createdTime = Timestamp::fromUtcTime(lockFile.created().utcTime());

        ///read pid
        string pid;
        FileInputStream istr(lockFile.path());
        CountingInputStream countingIstr(istr);
        StreamCopier::copyToString(countingIstr, pid);
        istr.close();

        ///build message
        serrviceOnMessage = "pid:" + pid + " created:" + DateTimeFormatter::format(createdTime,  DateTimeFormat::SORTABLE_FORMAT);
    }

}

bool LockService::start() noexcept
{
#ifndef NO_CHECK_PID
    ///if another instance run return false
    if (serrviceOnMessage != "") {
        return false;
    }
#endif

    ///then write lock file whith porocess pid
    FileOutputStream out;

    ///write pid
    FileOutputStream ostr(lockFile.path());
    CountingOutputStream countingOstr(ostr);
    TeeOutputStream tos(countingOstr);
    tos << std::to_string(Process::id());
    ostr.close();

    return true;

}

void LockService::stop() noexcept
{
    lockFile.remove();
}
