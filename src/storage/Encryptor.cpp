#include "./../../include/storage/Encryptor.h"
#include "./../../include/utils/Security.h"

std::string Encryptor::encrypt(
    const std::string& data,
    const std::string& key
) {
    std::string res = data;
    for(size_t i=0; i<data.length(); ++i) {
        res[i] = (data[i] ^ key[i % key.length()]);
    }
    return res;
}

std::string Encryptor::decrypt(
    const std::string& data, 
    const std::string& key
) {
    std::string hex = data;
    for(size_t i=0; i<hex.length(); ++i) {
        hex[i] = (hex[i] ^ key[i % key.length()]);
    }
    return hex;
}
