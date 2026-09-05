#pragma once
#include <android/asset_manager.h>
#include <cstdint>
#include <vector>
#include <string>

/**
 * AssetManager
 *
 * Thin wrapper around Android's AAssetManager.
 * Provides blocking reads from the APK's assets/ directory.
 *
 * The AAssetManager* pointer is owned by the android_app and must remain
 * valid for the lifetime of this object.
 */
class AssetManager {
public:
    explicit AssetManager(AAssetManager* mgr);

    /**
     * Reads the entire contents of an asset file into a byte buffer.
     * Returns an empty vector if the file is not found.
     * @param path  Path relative to assets/ (e.g., "shaders/vertex.glsl")
     */
    std::vector<uint8_t> readBytes(const std::string& path) const;

    /**
     * Reads an asset as a null-terminated string.
     * Useful for GLSL source files.
     */
    std::string readText(const std::string& path) const;

    /** Returns true if the named asset exists. */
    bool exists(const std::string& path) const;

    AAssetManager* getNative() const { return m_mgr; }

private:
    AAssetManager* m_mgr = nullptr;
};
