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

#include <memory>
using namespace std;

#include <Poco/AutoPtr.h>
using Poco::AutoPtr;

#include <Poco/Util/IniFileConfiguration.h>
using namespace Poco::Util;

#include "Poco/MongoDB/Connection.h"
using Poco::MongoDB::Connection;

#include "constants.h"

namespace auth {

class Globals
{

    AutoPtr<IniFileConfiguration> config;

    Connection db;

public:
    Globals() = default;
    Globals(const Globals&) = delete;
    Globals& operator = (const Globals&) = delete;
    Globals(Globals&&) = delete;
    Globals& operator = (Globals&&) = delete;

    static inline const shared_ptr<Globals> &getInstance() noexcept
    {
        static shared_ptr<Globals> instance;
        if (!instance) {
            instance = make_shared<Globals>();
        }
        return instance;
    }

    void init(const string & = PATH_CONFIG) noexcept;


    inline const AutoPtr<IniFileConfiguration> getConfig() const noexcept
    {
        return config;
    }

    inline const Connection & getDb() const noexcept
    {
        return db;
    }
};

}


