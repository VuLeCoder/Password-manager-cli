#include "./../include/Security.h"

#include <functional>

std::string Sercurity::hash(const std::string& password) {
    return std::to_string(std::hash<std::string>{}(password));
}
