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

    saveDraft(vaultData, tempPath);
    replaceVault(tempPath, path);
    fs::remove(tempPath);
}

VaultData BinaryStorage::load(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw StorageException(
            StorageCode::CannotOpenFile,
            "Cannot open temporary vault file: " + path.string()
        );
    }

    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        file.close();
        return VaultData{};
    }
    file.seekg(0, std::ios::beg);
    
    SecureBuffer buffer(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    BinaryReader reader(buffer);
    return reader.readVault();
}
