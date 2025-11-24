#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

enum class StorageType {
    LocalDisk,
    AmazonS3
};

class IFileStorage {
public:
    virtual ~IFileStorage() = default;
    virtual bool uploadFile(const std::string& localPath, const std::string& remotePath) = 0;
    virtual bool downloadFile(const std::string& remotePath, const std::string& localPath) = 0;
    virtual bool deleteFile(const std::string& remotePath) = 0;
    virtual std::string getStorageInfo() const = 0;
};

class UserStorageConfig {
private:
    UserStorageConfig() = default;
    UserStorageConfig(const UserStorageConfig&) = delete;
    UserStorageConfig& operator=(const UserStorageConfig&) = delete;
    UserStorageConfig(UserStorageConfig&&) = delete;
    UserStorageConfig& operator=(UserStorageConfig&&) = delete;

    StorageType currentStorage = StorageType::LocalDisk;
    std::unordered_map<std::string, std::string> configParams;

public:
    static UserStorageConfig& getInstance() {
        static UserStorageConfig instance;
        return instance;
    }

    void setStorageType(StorageType type) {
        currentStorage = type;
    }

    StorageType getStorageType() const {
        return currentStorage;
    }

    void setConfigParam(const std::string& key, const std::string& value) {
        configParams[key] = value;
    }

    std::string getConfigParam(const std::string& key) const {
        auto it = configParams.find(key);
        return (it != configParams.end()) ? it->second : "";
    }

    void clear() {
        currentStorage = StorageType::LocalDisk;
        configParams.clear();
    }
};

class LocalDiskStorage : public IFileStorage {
public:
    bool uploadFile(const std::string& localPath, const std::string& remotePath) override {
        return true;
    }

    bool downloadFile(const std::string& remotePath, const std::string& localPath) override {
        return true;
    }

    bool deleteFile(const std::string& remotePath) override {
        return true;
    }

    std::string getStorageInfo() const override {
        return "Local Disk: " +
            UserStorageConfig::getInstance().getConfigParam("local_root_path");
    }
};

class AmazonS3Storage : public IFileStorage {
public:
    bool uploadFile(const std::string& localPath, const std::string& remotePath) override {
        return true;
    }

    bool downloadFile(const std::string& remotePath, const std::string& localPath) override {
        return true;
    }

    bool deleteFile(const std::string& remotePath) override {
        return true;
    }

    std::string getStorageInfo() const override {
        return "Amazon S3 bucket: " +
            UserStorageConfig::getInstance().getConfigParam("s3_bucket");
    }
};

class StorageFactory {
public:
    static std::unique_ptr<IFileStorage> createStorage() {
        auto& config = UserStorageConfig::getInstance();
        switch (config.getStorageType()) {
        case StorageType::LocalDisk:
            return std::make_unique<LocalDiskStorage>();
        case StorageType::AmazonS3:
            return std::make_unique<AmazonS3Storage>();
        default:
            return std::make_unique<LocalDiskStorage>();
        }
    }
};

int main() {
    auto& config = UserStorageConfig::getInstance();
    config.setStorageType(StorageType::LocalDisk);
    config.setConfigParam("local_root_path", "/home/user/myfiles/");

    auto storage1 = StorageFactory::createStorage();
    std::cout << storage1->getStorageInfo() << std::endl;

    config.setStorageType(StorageType::AmazonS3);
    config.setConfigParam("s3_bucket", "my-company-backups");
    config.setConfigParam("aws_region", "eu-central-1");

    auto storage2 = StorageFactory::createStorage();
    std::cout << storage2->getStorageInfo() << std::endl;

    auto& config2 = UserStorageConfig::getInstance();
    std::cout << "Same instance? " << (&config == &config2 ? "YES" : "NO") << std::endl;

    return 0;
}