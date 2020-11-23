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

#include <string>
#include <tuple>
using std::string;
using std::tuple;


/**
 * @brief endWith check il the string end with suffix
 * @param str to check
 * @param suffix to end
 * @return true if str end whit suffix
 */
bool endWith(const string &str, const string &suffix) noexcept;

/**
 * @brief endWith check il the string end with suffix
 * @param str to check
 * @param suffix to end
 * @return true if str end whit suffix
 */
bool endWith(const string &str, const string &&suffix) noexcept
{
    return endWith(str, suffix);
}

/**
 * @brief hexToString convert hex to string
 * @param bytes
 * @param size
 * @param upperCase
 * @return
 */
inline tuple<uint8_t *, size_t> stringToBytes(string str) noexcept
{
    return tuple<uint8_t *, size_t>{reinterpret_cast<uint8_t *>(&str[0]), str.size()};
}
string bytesToString(const uint8_t *, size_t ) noexcept;

/**
 * @brief hexToString convert hex to string
 * @param bytes
 * @param size
 * @param upperCase
 * @return
 */
inline string bytesToString(const tuple<uint8_t *, size_t> &bytes) noexcept
{
    auto &&[first, second] = bytes;
    return bytesToString(first, second);
}

/**
 * @brief hexToString convert hex to string
 * @param bytes
 * @param size
 * @param upperCase
 * @return
 */
inline string bytesToString(const tuple<uint8_t *, size_t> &&bytes) noexcept
{
    auto &&[first, second] = bytes;
    return bytesToString(first, second);
}

/**
 * @brief hexToString convert hex to string
 * @param bytes
 * @param size
 * @param upperCase
 * @return
 */
string hexToString(const uint8_t *bytes, size_t size, bool upperCase = true) noexcept;
