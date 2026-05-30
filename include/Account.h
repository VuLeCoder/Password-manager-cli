#pragma once
#include <string>

class Account{
private:
    std::string service;
    std::string username;
    std::string password;
    std::string note;

public:
    Account() = default;

    Account(
        const std::string& service,
        const std::string& username,
        const std::string& password,
        const std::string& note
    );

    std::string getService();
    std::string getUsername();
    std::string getPassword();
    std::string getNote();

    void setService(const std::string& service);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setNote(const std::string& note);
};
