#pragma once

#include "PasswordManagerException.h"

enum class CommandCode{
    Unknown,
    Error,
};

class CommandException : public PasswordManagerException {
private:
    CommandCode code_;

public:
    explicit CommandException(
        CommandCode code, 
        const std::string& message
    ) : PasswordManagerException(message), code_(code) {}

    explicit CommandException(
        CommandCode code,
        const char* message
    ) : PasswordManagerException(message), code_(code) {}

    CommandCode code() const noexcept {
        return code_;
    }
};
