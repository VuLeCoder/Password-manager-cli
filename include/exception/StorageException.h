#pragma once

#include "./PasswordManagerException.h"

enum class StorageCode{
    Unknown,
    FileNotFound,
    CannotOpenFile,
    InvalidFormat,
    UnsupportedVersion
};

class StorageException : public PasswordManagerException {
private:
    StorageCode code_;

public:
    explicit StorageException(
        StorageCode code, 
        const std::string& message
    ) : PasswordManagerException(message), code_(code) {}

    explicit StorageException(
        StorageCode code,
        const char* message
    ) : PasswordManagerException(message), code_(code) {}

    StorageCode code() const noexcept {
        return code_;
    }
};
