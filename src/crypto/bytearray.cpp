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

#include "bytearray.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <Poco/Exception.h>
using Poco::Exception;

using namespace auth::crypto;

extern string hexToString(const uint8_t *bytes, size_t size, bool upperCase) noexcept;

ByteArray::ByteArray(const uint8_t * bytes, size_t size)
{
    if (!bytes)
    {
        throw Exception("nullptr bytes");
    }

    const uint8_t * p = bytes;
    for (size_type i = 0; i < size; i++)
    {
        push_back(*p);
        p++;
    }
}


ByteArray::ByteArray(const char *c)
{
    if (!c)
    {
        throw Exception("nullptr");
    }

    const char * p = c;
    for (size_type i = 0; i < strlen(c); i++)
    {
        push_back(*p);
        p++;
    }
}

ByteArray::ByteArray(const string &buffer) noexcept
{
    const char * p = buffer.c_str();
    for (size_type i = 0; i < buffer.size(); i++)
    {
        push_back(*p);
        p++;
    }
}

string ByteArray::toString() const
{
    string ret;
    for(auto b : *this)
    {
        ret += b;
    }
    return ret;
}

void ByteArray::add(const ByteArray &bytes)
{
    insert(end(), bytes.begin(), bytes.end());
}

string ByteArray::toBase64() const
{
    return ByteArray::toBase64(*this);
}

string ByteArray::toBase64(const ByteArray &bytes)
{
    const uint8_t* stringToEncode = bytes.toBytes();
    size_t length = bytes.size();

    if (!stringToEncode || length < 1)
    {
       throw Exception("bytesToEncode null or size 0");
    }
    BIO *buff, *b64f;
    BUF_MEM *ptr;

    b64f = BIO_new(BIO_f_base64());
    buff = BIO_new(BIO_s_mem());
    buff = BIO_push(b64f, buff);

    BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
    BIO_set_close(buff, BIO_CLOSE);
    BIO_write(buff, stringToEncode, length);
    BIO_flush(buff);

    BIO_get_mem_ptr(buff, &ptr);
    BIO_set_close(buff, BIO_NOCLOSE);

    string ret(ptr->data, ptr->length);

    BIO_free_all(buff);

    return ret;
}

ByteArray ByteArray::fromBase64(const string &encodedString)
{
    if (encodedString.empty()) {
        throw Exception("encodedString null");
    }
    const unsigned char *input = reinterpret_cast<const unsigned char *>(encodedString.data());
    int length = static_cast<int>(encodedString.size());

    BIO *b64, *bmem;

    uint8_t *buffer = new uint8_t[length];
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);

    BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
    BIO_set_close(bmem, BIO_CLOSE);
    int bufferLenght = BIO_read(bmem, buffer, length);

    BIO_free_all(bmem);

    ByteArray ret(buffer, bufferLenght);

    delete [] buffer;

    return ret;
}

string ByteArray::toHex(bool upperCase) const noexcept
{
    return hexToString(toBytes(), size(), upperCase);
}

bool operator==(const ByteArray&a, const ByteArray&b) noexcept
{
    if (a.size() != b.size())
    {
        return false;
    }
    return memcmp(a.data(), b.data(), sizeof(*a.data())) == 0;
}
bool operator==(const ByteArray&a, const char *b) noexcept
{
    return a.toString() == b;
}

bool operator==(const ByteArray&a, const string &b) noexcept
{
    return a.toString() == b;
}

ByteArray operator+(const ByteArray&a, const ByteArray&b)
{
    auto ret = a;
    ret.add(b);
    return ret;
}

string operator+(const string&a, const ByteArray &b)
{
    return a + b.toString();
}

string operator+(const ByteArray &a, const string &b)
{
    return a.toString() + b;
}

ostream& operator<<(ostream &os, const ByteArray &ba) noexcept
{
    os << const_cast<ByteArray &>(ba).toString();
    return os;
}
