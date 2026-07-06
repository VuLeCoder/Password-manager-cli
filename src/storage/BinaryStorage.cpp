#include "./../../include/storage/BinaryStorage.h"
#include "./../../include/storage/BinaryWriter.h"
#include "./../../include/storage/BinaryReader.h"
#include "./../../include/exception/StorageException.h"
#include "storage/VaultData.h"

#include <fstream>

// === === === === ===
// private:
// === === === === ===
void BinaryStorage::replaceVault(
    const fs::path& tempPath,
    const fs::path& finalPath
) {
    try {
        fs::rename(tempPath, finalPath);
    } catch (const fs::filesystem_error& e) {
        throw StorageException(
            StorageCode::CannotRenameFile,
            std::string("Cannot rename file: ") + e.what()
        );
    }
}

void BinaryStorage::saveDraft(
    const VaultData& vaultData,
    const fs::path& tempPath
) {
    std::ofstream file(tempPath, std::ios::binary);
    if(!file.is_open()) {
        throw StorageException(
            StorageCode::CannotOpenFile,
            "Cannot open temporary vault file: " + tempPath.string()
        );
    }

    BinaryWriter writer;
    writer.writeVault(vaultData);

    SecureBuffer buffer = writer.takeBuffer();

    file.write(
        reinterpret_cast<const char*>(buffer.data()),
        static_cast<std::streamsize>(buffer.size())
    );

    if (!file) {
        throw StorageException(
            StorageCode::CannotWriteFile,
            "Failed to write vault file: " + tempPath.string()
        );
    }
}

// === === === === ===
// public:
// === === === === ===
void BinaryStorage::save(
    const VaultData& vaultData, 
    const fs::path& path
) {
    fs::path tempPath = path;
    tempPath += ".tmp";

    if(path.has_parent_path()) {
        fs::create_directory(path.parent_path());
    }

    try {
        saveDraft(vaultData, tempPath);
        replaceVault(tempPath, path);
    } catch (...) {
        std::error_code ec;
        fs::remove(tempPath, ec);
        throw;
    }
}

VaultData BinaryStorage::load(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw StorageException(
            StorageCode::CannotOpenFile,
            "Cannot open vault file: " + path.string()
        );
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if (size == 0) {
        file.close();
        return VaultData{};
    }
    file.seekg(0, std::ios::beg);
    
    SecureBuffer buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    BinaryReader reader(buffer);
    return reader.readVault();
}
