#pragma once

#include "PasswordManagerException.h"

enum class EncryptCode{
    Unknown,
    RandomGenerationFailed,
    ContextError,
    EncryptError,
    DecryptError,
};

class EncryptException : public PasswordManagerException {
private:
    EncryptCode code_;

public:
    explicit EncryptException(
        EncryptCode code, 
        const std::string& message
    ) : PasswordManagerException(message), code_(code) {}

    explicit EncryptException(
        EncryptCode code,
        const char* message
    ) : PasswordManagerException(message), code_(code) {}

    EncryptCode code() const noexcept {
        return code_;
    }
};
