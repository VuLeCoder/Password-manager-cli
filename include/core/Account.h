#pragma once
#include <string/SecureString.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Account {
private:
    SecureString service;
    SecureString username;
    SecureString password;
    SecureString category;
    SecureString note;

public:
    Account() = default;

    Account(
        const SecureString& service,
        const SecureString& username,
        const SecureString& password,
        const SecureString& category,
        const SecureString& note
    );

    Account(const Account& other);

    const SecureString& getService() const;
    const SecureString& getUsername() const;
    const SecureString& getPassword() const;
    const SecureString& getCategory() const;
    const SecureString& getNote() const;

    void setService(const SecureString& service);
    void setUsername(const SecureString& username);
    void setPassword(const SecureString& password);
    void setCategory(const SecureString& category);
    void setNote(const SecureString& note);
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

