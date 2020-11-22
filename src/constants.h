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

///default catch exceptions
#define AUTH_CATCH_EXCEPTIONS \
    catch (const mariadb::exception::base &e) { \
        AUTH_GLOBAL_LOG(ERROR, e.what()); \
    }  catch (const std::out_of_range &e) { \
        AUTH_GLOBAL_LOG(ERROR, e.what());\
    }

/// Disable copy and reference constructror
#define AUTH_NO_COPY_NO_MOVE(claxx) \
    claxx(const claxx&) = delete; \
    claxx& operator = (const claxx&) = delete; \
    claxx(claxx&&) = delete; \
    claxx& operator = (claxx&&) = delete;

namespace auth
{

///config global param
inline constexpr const char *PATH_CONFIG = "./config.ini";
inline constexpr const char *FILE_LOCK_SERVICE = "./auth-web.pin";
inline constexpr const char *CHECK_DB = "SHOW TABLES LIKE 'users';";
inline constexpr const char *API_V1 = "/api/v1/";

///header param
inline constexpr const char *HEADER_CONTENT_TYPE = "application/json";
inline constexpr const char *HEADER_CORS = "Access-Control-Allow-Origin";

///json response param
inline constexpr const char *JSON_STATUS = "status";
inline constexpr const char *JSON_STATUS_OK = "ok";
inline constexpr const char *JSON_STATUS_ERROR = "error";
inline constexpr const char *JSON_DATA = "data";
inline constexpr const char *JSON_HTTP_STATUS = "httpStatus";


///config key param
inline constexpr const char *CONFIG_PATH_LOG = "log.path";
inline constexpr const char *CONFIG_LOG_ROTATION = "log.rotation";
inline constexpr const char *CONFIG_SERVER_PORT = "server.port";
inline constexpr const char *CONFIG_SERVER_MAX_QUEUED = "server.maxQueued";
inline constexpr const char *CONFIG_SERVER_MAX_THREAD = "server.maxThreads";
inline constexpr const char *CONFIG_SERVER_PASSWORD = "server.password";
inline constexpr const char *CONFIG_DB_HOST= "db.host";
inline constexpr const char *CONFIG_DB_PORT= "db.port";
inline constexpr const char *CONFIG_DB_DATABASE = "db.database";
inline constexpr const char *CONFIG_DB_USER = "db.user";
inline constexpr const char *CONFIG_DB_PASSWORD = "db.password";
inline constexpr const char *CONFIG_DB_SCRIPT = "db.script";

}
