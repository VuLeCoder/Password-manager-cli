#include "./../include/Security.h"

#include <functional>
#include <sstream>
#include <iomanip>

std::string Sercurity::hash(const std::string& password) {
    return std::to_string(std::hash<std::string>{}(password));
}

std::string Sercurity::toHex(const std::string& data) {
    std::stringstream ss;

    for(unsigned char c : data) {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(c);
    }

    return ss.str();
}

std::string Sercurity::hexToStr(const std::string& hex) {
    std::string res;

    if(hex.length() % 2 != 0) {
        return res;
    }

    for(size_t i=0; i<hex.length(); i+=2) {
        std::string byteString =
            hex.substr(i, 2);

        char byte = static_cast<char>(
            std::stoi(byteString, nullptr, 16)
        );

        res.push_back(byte);
    }

    return res;
}
