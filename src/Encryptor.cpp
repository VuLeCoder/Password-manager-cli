#include "./../include/Encryptor.h"
#include "./../include/Security.h"

std::string Encryptor::encrypt(
    const std::string& data,
    const std::string& key
) {
    std::string res = data;
    for(size_t i=0; i<data.length(); ++i) {
        res[i] = (data[i] ^ key[i % key.length()]);
    }
    return Sercurity::toHex(res);
}

std::string Encryptor::decrypt(
    const std::string& data,
    const std::string& key
) {
    std::string hex = Sercurity::hexToStr(data);
    for(size_t i=0; i<hex.length(); ++i) {
        hex[i] = (hex[i] ^ key[i % key.length()]);
    }
    return hex;
}
