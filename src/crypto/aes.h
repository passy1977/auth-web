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

#include <openssl/aes.h>
#include <openssl/evp.h>

#include "bytearray.h"

namespace auth::crypto
{

class AES final
{
public:
    enum class Type
    {
        CBC, ECB, CTR, OFB
    };
    enum class KeySize : uint8_t
    {
        K128 = 16 , K192 = 24, K256 = 32
    };
    
private:
    const Type type;
    const KeySize keySize;
    
    ByteArray iv;
    ByteArray key;
    
public:
    static constexpr const char PADDING = '$';
    
    AES(const ByteArray &, const ByteArray &, Type = Type::CBC, KeySize = KeySize::K128, char = PADDING, bool = false);
    inline explicit AES(const ByteArray &key, Type type = Type::CBC, KeySize keySize = KeySize::K128, char padding = PADDING, bool accurateKeySize = false) : AES(key, {}, type, keySize, padding, accurateKeySize) {}
    inline explicit AES(const ByteArray &&key, Type type = Type::CBC, KeySize keySize = KeySize::K128, char padding = PADDING, bool accurateKeySize = false) : AES(key, {}, type, keySize, padding, accurateKeySize) {}
    inline AES(const ByteArray &&key, const ByteArray &&iv, Type type = Type::CBC, KeySize keySize = KeySize::K128, char padding = PADDING, bool accurateKeySize = false) : AES(key, iv, type, keySize, padding, accurateKeySize) {}
    AES(const AES&) = delete;
    AES operator = (const AES&) = delete;
    AES(AES&&) = delete;
    AES operator = (AES&&) = delete;
    ~AES() noexcept ;
    
    ByteArray encrypt(const ByteArray &) const;
    inline ByteArray encrypt(const ByteArray &&bytes) const {
        return encrypt(bytes);
    }

    ByteArray decrypt(const ByteArray &) const;
    inline ByteArray decrypt(const ByteArray &&bytes) const {
        return decrypt(bytes);
    }

    inline Type getType() const noexcept  {
        return type;
    }
    
    inline KeySize getKeySyze() const noexcept
    {
        return keySize;
    }
    
    string toString() const;
    
private:
    
    ByteArray encryptFunc(const ByteArray &, const EVP_CIPHER *(*)(), bool) const;
    ByteArray decryptFunc(const ByteArray &, const EVP_CIPHER *(*)(), bool) const;
    
    ByteArray encryptCBCMode(const ByteArray &) const;
    ByteArray dencryptCBCMode(const ByteArray &) const;
    
    ByteArray encryptECBMode(const ByteArray &) const;
    ByteArray dencryptECBMode(const ByteArray &) const;
    
    ByteArray encryptCTRMode(const ByteArray &) const;
    ByteArray dencryptCTRMode(const ByteArray &) const;
    
    ByteArray encryptOFBMode(const ByteArray &) const;
    ByteArray dencryptOFBMode(const ByteArray &) const;
};

}

