#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Account {
private:
    std::string service;
    std::string username;
    std::string password;
    std::string category;
    std::string note;

public:
    Account() = default;

    Account(
        const std::string& service,
        const std::string& username,
        const std::string& password,
        const std::string& category,
        const std::string& note
    );

    Account(const Account& other);

    const std::string& getService() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
    const std::string& getCategory() const;
    const std::string& getNote() const;

    void setService(const std::string& service);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setCategory(const std::string& category);
    void setNote(const std::string& note);
};

inline void to_json(json& j, const Account& a) {
    j = {
        {"service", a.getService()},
        {"username", a.getUsername()},
        {"password", a.getPassword()},
        {"category", a.getCategory()},
        {"note", a.getNote()}
    };
}

inline void from_json(const json& j, Account& a) {
    a = Account(
        j.value("service", ""),
        j.value("username", ""),
        j.value("password", ""),
        j.value("category", ""),
        j.value("note", "")
    );
}

