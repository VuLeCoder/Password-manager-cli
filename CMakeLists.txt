cmake_minimum_required(VERSION 3.10)

project(PasswordManager)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ARGON2 REQUIRED libargon2)
pkg_check_modules(OPENSSL REQUIRED openssl)
find_package(nlohmann_json REQUIRED)

# Source files
set(SOURCES
    src/main.cpp

    src/cli/CommandDispatcher.cpp
    src/cli/CommandParser.cpp
    src/cli/Console.cpp

    src/core/Account.cpp
    src/core/AuthGuard.cpp
    src/core/PasswordManager.cpp
    src/core/SessionContext.cpp
    src/core/Vault.cpp

    src/storage/Storage.cpp
    
    src/utils/Encryptor.cpp
    src/utils/Security.cpp
)

# Header files (optional, but helpful for IDEs)
set(HEADERS
    include/cli/Command.h
    include/cli/CommandDispatcher.h
    include/cli/CommandParser.h
    include/cli/Console.h

    include/core/Account.h
    include/core/AuthGuard.h
    include/core/PasswordManager.h
    include/core/SessionContext.h
    include/core/Vault.h

    include/storage/Storage.h
    
    include/utils/Constants.h
    include/utils/Encryptor.h
    include/utils/Security.h
)

# Create executable
add_executable(password-manager ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(
    password-manager 
    PRIVATE 
    include
    ${ARGON2_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
)

target_link_directories(
    password-manager
    PRIVATE
    ${ARGON2_LIBRARY_DIRS}
    ${OPENSSL_LIBRARY_DIRS}
)

target_link_libraries(
    password-manager
    PRIVATE
    ${ARGON2_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    nlohmann_json::nlohmann_json
    crypt32
    ws2_32
    bcrypt
)

set_target_properties(password-manager PROPERTIES
    OUTPUT_NAME "lptv"
)

if(MINGW)
    target_link_options(password-manager PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()
