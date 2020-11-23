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


#include <sstream>
#include <iostream>
#include <cstring>

#include <openssl/err.h>

#include <Poco/Exception.h>
using Poco::Exception;

#include "aes.h"


using namespace auth::crypto;


using FuncEncrType = const EVP_CIPHER *(*)();

static string getOpenSSLError()
{
    BIO *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf;
    size_t len = BIO_get_mem_data(bio, &buf);
    string ret(buf, len);
    BIO_free(bio);
    return ret;
}


AES::AES(const ByteArray &key, const ByteArray &iv,  Type type, KeySize keySize, char padding, bool accurateKeySize) : type(type), keySize(keySize), iv(iv)
{
    if (key.empty())
    {
        throw Exception("key empty");
    }
    if (this->type == Type::CBC && iv.empty())
    {
        throw Exception("iv empty");
    }
    if (this->type == Type::CBC && iv.size() != AES_BLOCK_SIZE)
    {
        throw Exception("iv wrong size");
    }
    
    
    const uint8_t KEY_SIZE = static_cast<uint16_t>(this->keySize);
    
    ByteArray byteKey;
    if (accurateKeySize)
    {
        if (key.size() != KEY_SIZE)
        {

            throw Exception("wrong key size");
        }
    }
    else
    {
        uint8_t i = 0;
        for (; i < key.size() && i < KEY_SIZE; i++)
        {
            byteKey.push_back(key[i]);
        }
        for (; i < KEY_SIZE; i++)
        {
            byteKey.push_back(padding);
        }
    }
    
    this->key = byteKey;
    
}

AES::~AES() noexcept
{
    fill(iv.begin(), iv.end(), 0x0);
    fill(key.begin(), key.end(), 0x0);
    iv.clear();
    key.clear();
}

string AES::toString() const
{
    ostringstream oss;
    oss << (void*)this;
    return "GroupDao@" + string(oss.str());
}

ByteArray AES::encryptFunc(const ByteArray &plain, FuncEncrType func, bool enableIv) const
{
    
    int keyLength = static_cast<int>(keySize);
    int plainTextLen = static_cast<int>(plain.size());
    
    
    /* generate a key with a given length */
    uint8_t key[keyLength];
    memset(key, 0x0, keyLength);
    memcpy(key, &this->key.data()[0], keyLength);
    
    uint8_t *iv;
    if (enableIv)
    {
        iv = new uint8_t[AES_BLOCK_SIZE];
        memset(iv, 0x0, AES_BLOCK_SIZE);
        memcpy(iv, &this->iv.data()[0], AES_BLOCK_SIZE);
    }
    else
    {
        iv = nullptr;
    }

    
    /* generate input with a given length */
    uint8_t plainText[plainTextLen];
    memset(plainText, 0x0, plainTextLen);
    memcpy(plainText, &plain[0], plainTextLen);
    
    uint8_t cipherText[((plainTextLen + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE];


    EVP_CIPHER_CTX *ctx;

    int len;
    
    int cipherTextLen;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        throw Exception(getOpenSSLError());
    }
    
    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, func(), nullptr, key, iv))
    {
        getOpenSSLError();
        throw Exception(getOpenSSLError());
    }
    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, cipherText, &len, plainText, plainTextLen))
    {
        throw Exception(getOpenSSLError());
    }
    cipherTextLen = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, cipherText + len, &len))
    {
        throw Exception(getOpenSSLError());
    }
    cipherTextLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    delete [] iv;

    return ByteArray(const_cast<const uint8_t *>(cipherText), cipherTextLen);
}

ByteArray AES::decryptFunc(const ByteArray &cipher, FuncEncrType func, bool enableIv) const
{
    
    int keyLength = static_cast<int>(keySize);
    int cipherTextLen = static_cast<int>(cipher.size());

    /* generate a key with a given length */
    uint8_t key[keyLength];
    memset(key, 0x0, keyLength);
    memcpy(key, &this->key.data()[0], keyLength);

    uint8_t *iv;
    if (enableIv)
    {
        iv = new uint8_t[AES_BLOCK_SIZE];
        memset(iv, 0x0, AES_BLOCK_SIZE);
        memcpy(iv, &this->iv.data()[0], AES_BLOCK_SIZE);
    }
    else
    {
        iv = nullptr;
    }
    
    /* generate input with a given length */
    uint8_t cipherText[cipherTextLen];
    memset(cipherText, 0x0, cipherTextLen);
    memcpy(cipherText, &cipher[0], cipherTextLen);

    uint8_t plainText[cipherTextLen];

    EVP_CIPHER_CTX *ctx;

    int len;

    int plainTextLen;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        throw Exception(getOpenSSLError());
    }

    /*
    * Initialise the decryption operation. IMPORTANT - ensure you use a key
    * and IV size appropriate for your cipher
    * In this example we are using 256 bit AES (i.e. a 256 bit key). The
    * IV size for *most* modes is the same as the block size. For AES this
    * is 128 bits
    */
    if(1 != EVP_DecryptInit_ex(ctx, func(), nullptr, key, iv))
    {
        throw Exception(getOpenSSLError());
    }
    
    /*
    * Provide the message to be decrypted, and obtain the plaintext output.
    * EVP_DecryptUpdate can be called multiple times if necessary.
    */
    if(1 != EVP_DecryptUpdate(ctx, plainText, &len, cipherText, cipherTextLen))
    {
        throw Exception(getOpenSSLError());
    }
    plainTextLen = len;

    /*
    * Finalise the decryption. Further plaintext bytes may be written at
    * this stage.
    */
    if(1 != EVP_DecryptFinal_ex(ctx, plainText + len, &len))
    {
        throw Exception(getOpenSSLError());
    }
    plainTextLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    delete [] iv;
    
    return ByteArray(const_cast<const uint8_t *>(plainText), plainTextLen);
}

ByteArray AES::encrypt(const ByteArray &bytes) const
{
    if (bytes.empty())
    {
        throw Exception("bytes empty");
    }
    
    switch (type)
    {
        case Type::CBC:
            return encryptCBCMode(bytes);
        case Type::CTR:
            return encryptCTRMode(bytes);
        case Type::ECB:
            return encryptECBMode(bytes);
        case Type::OFB:
            return encryptOFBMode(bytes);
    }
}

ByteArray AES::decrypt(const ByteArray &bytes) const
{
    if (bytes.empty())
    {
        throw Exception("bytes empty");
    }
    
    switch (type)
    {
        case Type::CBC:
            return dencryptCBCMode(bytes);
        case Type::CTR:
            return dencryptCTRMode(bytes);
        case Type::ECB:
             return dencryptECBMode(bytes);
        case Type::OFB:
             return dencryptOFBMode(bytes);
    }
}



ByteArray AES::encryptCBCMode(const ByteArray &plain) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_cbc;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_cbc;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_cbc;
            break;
    }
    
    return encryptFunc(plain, func, true);
}

ByteArray AES::dencryptCBCMode(const ByteArray &cipher) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_cbc;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_cbc;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_cbc;
            break;
    }
    
    return decryptFunc(cipher, func, true);
}

ByteArray AES::encryptECBMode(const ByteArray &plain) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ecb;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ecb;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_cbc;
            break;
    }
    
    return encryptFunc(plain, func, false);
}

ByteArray AES::dencryptECBMode(const ByteArray &cipher) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ecb;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ecb;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_cbc;
            break;
    }
    
    return decryptFunc(cipher, func, false);
}


ByteArray AES::encryptCTRMode(const ByteArray &plain) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ctr;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ctr;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_ctr;
            break;
    }
    
    return encryptFunc(plain, func, false);
}

ByteArray AES::dencryptCTRMode(const ByteArray &cipher) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ctr;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ctr;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_ctr;
            break;
    }
    
    return decryptFunc(cipher, func, false);
}

ByteArray AES::encryptOFBMode(const ByteArray &plain) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ofb;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ofb;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_ofb;
            break;
    }
    
    return encryptFunc(plain, func, false);
}

ByteArray AES::dencryptOFBMode(const ByteArray &cipher) const
{
    FuncEncrType func;
    switch (this->keySize)
    {
        case KeySize::K128:
            func = &EVP_aes_128_ctr;
            break;
        case KeySize::K192:
            func = &EVP_aes_192_ctr;
            break;
        case KeySize::K256:
            func = &EVP_aes_256_ctr;
            break;
    }
    
    return decryptFunc(cipher, func, false);
}


//#pragma clang diagnostic pop
