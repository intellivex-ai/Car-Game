#include "core/AssetManager.h"
#include "core/Log.h"
#include <android/asset_manager.h>

AssetManager::AssetManager(AAssetManager* mgr)
    : m_mgr(mgr) {}

std::vector<uint8_t> AssetManager::readBytes(const std::string& path) const {
    AAsset* asset = AAssetManager_open(m_mgr, path.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        LOGW("AssetManager: file not found: %s", path.c_str());
        return {};
    }

    size_t size = static_cast<size_t>(AAsset_getLength(asset));
    std::vector<uint8_t> buffer(size);
    AAsset_read(asset, buffer.data(), size);
    AAsset_close(asset);
    return buffer;
}

std::string AssetManager::readText(const std::string& path) const {
    auto bytes = readBytes(path);
    return std::string(bytes.begin(), bytes.end());
}

bool AssetManager::exists(const std::string& path) const {
    AAsset* asset = AAssetManager_open(m_mgr, path.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) return false;
    AAsset_close(asset);
    return true;
}
