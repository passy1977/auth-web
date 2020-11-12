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


namespace auth
{

constexpr const char* PATH_CONFIG = "./config.ini";
constexpr const char* PATH_LOG = "log/auth_web.path";
constexpr const char* LOG_ROTATION = "2 K";
constexpr const char* CONFIG_SERVER_PORT = "server.port";
constexpr const char* CONFIG_SERVER_MAX_QUEUED = "server.maxQueued";
constexpr const char* CONFIG_SERVER_MAX_THREAD = "server.maxThreads";
constexpr const char* CONFIG_MYSQL_HOST= "mysql.host";
constexpr const char* CONFIG_MYSQL_DATABASE = "mysql.database";
constexpr const char* CONFIG_MYSQL_USER = "mysql.user";
constexpr const char* CONFIG_MYSQL_PASSWORD = "mysql.password";

}
