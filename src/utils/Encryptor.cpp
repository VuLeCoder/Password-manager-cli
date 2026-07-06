#include "exception/EncryptException.h"
#include "utils/Encryptor.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

EncryptResult Encryptor::encrypt(
    const SecureBuffer& plaintext,
    const uint8_t* key
) {
    EncryptResult result;
    generateRandomNonce(result.nonce);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx) {
        throw EncryptException(EncryptCode::ContextError, "Failed to create cipher context.");
    }

    result.ciphertext.resize(plaintext.size());

    int len = 0;
    int ciphertextLen = 0;

    try {
        if(EVP_EncryptInit_ex(
                ctx,
                EVP_aes_256_gcm(),
                nullptr,
                nullptr,
                nullptr) != 1
        ) 
            throw EncryptException(EncryptCode::EncryptError, "EncryptInit failed.");

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_IVLEN,
                result.nonce.size(),
                nullptr) != 1
        )
            throw EncryptException(EncryptCode::EncryptError, "Failed to set nonce length.");

        if(EVP_EncryptInit_ex(
                ctx,
                nullptr,
                nullptr,
                key,
                result.nonce.data()) != 1
        )
            throw EncryptException(EncryptCode::EncryptError, "Failed to initialize key.");

        if(EVP_EncryptUpdate(
                ctx,
                result.ciphertext.data(),
                &len,
                plaintext.data(),
                static_cast<int>(plaintext.size())) != 1
        )
            throw EncryptException(EncryptCode::EncryptError, "EncryptUpdate failed.");

        ciphertextLen = len;

        if(EVP_EncryptFinal_ex(
                ctx,
                result.ciphertext.data() + len,
                &len) != 1
        )
            throw EncryptException(EncryptCode::EncryptError, "EncryptFinal failed.");

        ciphertextLen += len;

        result.ciphertext.resize(ciphertextLen);

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_GET_TAG,
                result.tag.size(),
                result.tag.data()) != 1
        )
            throw EncryptException(EncryptCode::EncryptError, "Failed to get authentication tag.");
    }
    catch (...) {
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }

    EVP_CIPHER_CTX_free(ctx);

    return result;
}

SecureBuffer Encryptor::decrypt(
    const SecureBuffer& ciphertext,
    const uint8_t* key,
    const std::array<uint8_t,12>& nonce,
    const std::array<uint8_t,16>& tag
) {
    SecureBuffer plaintext(ciphertext.size());

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx) {
        throw EncryptException(EncryptCode::ContextError, "Failed to create cipher context.");
    }

    int len = 0;
    int plaintextLen = 0;

    try {
        if(EVP_DecryptInit_ex(
                ctx,
                EVP_aes_256_gcm(),
                nullptr,
                nullptr,
                nullptr) != 1)
            throw EncryptException(EncryptCode::DecryptError, "DecryptInit failed.");

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_IVLEN,
                nonce.size(),
                nullptr) != 1)
            throw EncryptException(EncryptCode::DecryptError, "Failed to set nonce length.");

        if(EVP_DecryptInit_ex(
                ctx,
                nullptr,
                nullptr,
                key,
                nonce.data()) != 1)
            throw EncryptException(EncryptCode::DecryptError, "Failed to initialize key.");

        if(EVP_DecryptUpdate(
                ctx,
                plaintext.data(),
                &len,
                ciphertext.data(),
                static_cast<int>(ciphertext.size())) != 1)
            throw EncryptException(EncryptCode::DecryptError, "DecryptUpdate failed.");

        plaintextLen = len;

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_TAG,
                tag.size(),
                const_cast<uint8_t*>(tag.data())) != 1)
            throw EncryptException(EncryptCode::DecryptError, "Failed to set authentication tag.");

        if(EVP_DecryptFinal_ex(
                ctx,
                plaintext.data() + len,
                &len) != 1)
            throw EncryptException(EncryptCode::DecryptError, "Authentication failed.");

        plaintextLen += len;

        plaintext.resize(plaintextLen);
    }
    catch (...) {
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }

    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

void Encryptor::generateRandomNonce(std::array<uint8_t,12>& nonce) {
    if(RAND_bytes(nonce.data(), static_cast<int>(nonce.size())) != 1){
        throw EncryptException(
            EncryptCode::RandomGenerationFailed,
            "Failed to generate random nonce."
        );
    }
}
