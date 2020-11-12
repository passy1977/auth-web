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

#include <iostream>
#include <vector>
using namespace std;

#include <Poco/Util/ServerApplication.h>
using namespace Poco::Util;

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
using namespace Poco::Net;

#include "router.h"
using auth::Router;


namespace auth
{

/**
 * @brief The Application class load configuration and start up server
 */
class Application final : public ServerApplication
{
   Poco::UInt16 port = 9'100;
   int maxQueued = 100;
   int maxThreads = 2;

protected:

    /**
     * @brief main override of ServerApplication
     * @return status of application
     */
    int main(const vector<string> &) override;
};


}
