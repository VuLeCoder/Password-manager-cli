#include "./../../include/utils/Encryptor.h"
#include "./../../include/utils/Security.h"

#include <openssl/evp.h>

bool Encryptor::encrypt(
    const std::vector<uint8_t>& plaintext,
    const uint8_t* key,
    const uint8_t* nonce,
    std::vector<uint8_t>& ciphertext,
    std::array<uint8_t, 16>& tag
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx) return false;

    int len = 0;
    ciphertext.resize(plaintext.size());

    bool success = false;

    do {
        if(EVP_EncryptInit_ex(
                ctx,
                EVP_aes_256_gcm(),
                nullptr,
                nullptr,
                nullptr) != 1)
            break;

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_IVLEN,
                12,
                nullptr) != 1)
            break;

        if(EVP_EncryptInit_ex(
                ctx,
                nullptr,
                nullptr,
                key,
                nonce) != 1)
            break;

        if(EVP_EncryptUpdate(
                ctx,
                ciphertext.data(),
                &len,
                plaintext.data(),
                static_cast<int>(plaintext.size())) != 1)
            break;

        int ciphertext_len = len;

        if (EVP_EncryptFinal_ex(
                ctx,
                ciphertext.data() + len,
                &len) != 1)
            break;

        ciphertext_len += len;

        ciphertext.resize(ciphertext_len);

        if (EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_GET_TAG,
                16,
                tag.data()) != 1)
            break;

        success = true;

    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return success;
}

bool Encryptor::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const uint8_t* key,
    const uint8_t* nonce,
    const uint8_t* tag,
    std::vector<uint8_t>& plaintext
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx) return false;

    int len = 0;
    plaintext.resize(ciphertext.size());
    bool success = false;

    do {
        if(EVP_DecryptInit_ex(
                ctx,
                EVP_aes_256_gcm(),
                nullptr,
                nullptr,
                nullptr) != 1)
            break;

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_IVLEN,
                12,
                nullptr) != 1)
            break;

        if(EVP_DecryptInit_ex(
                ctx,
                nullptr,
                nullptr,
                key,
                nonce) != 1)
            break;

        if(EVP_DecryptUpdate(
                ctx,
                plaintext.data(),
                &len,
                ciphertext.data(),
                static_cast<int>(ciphertext.size())) != 1)
            break;

        int plaintext_len = len;

        if(EVP_CIPHER_CTX_ctrl(
                ctx,
                EVP_CTRL_GCM_SET_TAG,
                16,
                const_cast<uint8_t*>(tag)) != 1)
            break;

        int ret = EVP_DecryptFinal_ex(
            ctx,
            plaintext.data() + len,
            &len
        );

        if(ret <= 0) break;

        plaintext_len += len;
        plaintext.resize(plaintext_len);
        success = true;

    } while (false);

    EVP_CIPHER_CTX_free(ctx);
    return success;
}
