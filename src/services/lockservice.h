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

#include <Poco/File.h>
using namespace Poco;

namespace auth::services
{

/**
 * @brief The LockService class avoid the execution of multiple service instances
 */
class LockService
{
    File lockFile;

    string statusMessage = "";
public:
    LockService() noexcept;

    /**
     * @brief start create the lock on application starup
     * @return true if all ok
     */
    bool start() noexcept;

    /**
     * @brief stop remove lock on application destoy
     */
    void stop() noexcept;

    /**
     * @brief getStatusMessage check if a service it'a already run
     * @return "" if not run
     */
    inline const string & getStatusMessage() const noexcept
    {
        return statusMessage;
    }
};

}

