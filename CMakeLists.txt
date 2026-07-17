cmake_minimum_required(VERSION 3.16)

project(
    PasswordManager
    VERSION 1.0.0
    LANGUAGES CXX
)

# =========================================================
# Build type
# =========================================================

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# =========================================================
# Dependencies
# =========================================================

add_subdirectory(external/Secure-string)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(ARGON2 REQUIRED libargon2)
pkg_check_modules(OPENSSL REQUIRED openssl)

# =========================================================
# Sources
# =========================================================

set(SOURCES
    src/main.cpp

    src/cli/CommandDispatcher.cpp
    src/cli/CommandParser.cpp
    src/cli/Console.cpp

    src/cli/modules/HelpPrinter.cpp

    src/core/Account.cpp
    src/core/AuthGuard.cpp
    src/core/PasswordManager.cpp
    src/core/SessionContext.cpp
    src/core/Vault.cpp

    src/storage/BinaryStorage.cpp
    src/storage/BinaryReader.cpp
    src/storage/BinaryWriter.cpp

    src/utils/Encryptor.cpp
    src/utils/Security.cpp
)

set(HEADERS
    include/cli/Command.h
    include/cli/CommandDispatcher.h
    include/cli/CommandParser.h
    include/cli/Console.h

    include/cli/modules/HelpPrinter.h

    include/core/Account.h
    include/core/AuthGuard.h
    include/core/PasswordManager.h
    include/core/SessionContext.h
    include/core/Vault.h

    include/storage/BinaryStorage.h

    include/utils/Constants.h
    include/utils/Encryptor.h
    include/utils/Security.h
)

# =========================================================
# Executable
# =========================================================

add_executable(password-manager
    ${SOURCES}
    ${HEADERS}
)

target_compile_features(password-manager PRIVATE cxx_std_17)

# =========================================================
# Include directories
# =========================================================

target_include_directories(password-manager
    PRIVATE
        include
        ${ARGON2_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIRS}
)

target_link_directories(password-manager
    PRIVATE
        ${ARGON2_LIBRARY_DIRS}
        ${OPENSSL_LIBRARY_DIRS}
)

# =========================================================
# Libraries
# =========================================================

target_link_libraries(password-manager
    PRIVATE
        SecureString
        Threads::Threads
        ${ARGON2_LIBRARIES}
        ${OPENSSL_LIBRARIES}
)

if(WIN32)

    target_link_libraries(password-manager PRIVATE
        ws2_32
        crypt32
        bcrypt
    )

elseif(UNIX)

    target_link_libraries(password-manager PRIVATE
        pthread
    )

endif()

# =========================================================
# Warnings
# =========================================================

if(MSVC)

    target_compile_options(password-manager PRIVATE
        /W4
    )

else()

    target_compile_options(password-manager PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()

# =========================================================
# Output
# =========================================================

set_target_properties(password-manager PROPERTIES
    OUTPUT_NAME "lptv"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# =========================================================
# Static runtime (MinGW)
# =========================================================

if(MINGW)

    target_link_options(password-manager PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )

endif()

# =========================================================
# Install
# =========================================================

install(
    TARGETS password-manager
    RUNTIME DESTINATION bin
)

# =========================================================
# Packaging
# =========================================================

set(CPACK_PACKAGE_NAME "LPTV Password Manager")
set(CPACK_PACKAGE_VENDOR "LPTV")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
