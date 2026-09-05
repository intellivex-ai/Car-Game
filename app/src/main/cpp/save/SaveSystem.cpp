#include "save/SaveSystem.h"
#include "core/Log.h"
#include <fstream>

SaveSystem::SaveSystem() = default;

void SaveSystem::init(const std::string& internalStoragePath) {
    m_saveFilePath = internalStoragePath + "/game_save.bin";
    load();
}

bool SaveSystem::save() {
    std::ofstream file(m_saveFilePath, std::ios::binary);
    if (!file.is_open()) {
        LOGE("SaveSystem: Failed to open save file for writing: %s", m_saveFilePath.c_str());
        return false;
    }

    file.write(reinterpret_cast<const char*>(&m_settings.gyroscopeEnabled), sizeof(bool));
    file.write(reinterpret_cast<const char*>(&m_settings.musicVolume), sizeof(float));
    file.write(reinterpret_cast<const char*>(&m_settings.sfxVolume), sizeof(float));

    file.close();
    LOGI("SaveSystem: Settings saved successfully to %s", m_saveFilePath.c_str());
    return true;
}

bool SaveSystem::load() {
    std::ifstream file(m_saveFilePath, std::ios::binary);
    if (!file.is_open()) {
        LOGI("SaveSystem: No existing save file found, using defaults");
        return false;
    }

    file.read(reinterpret_cast<char*>(&m_settings.gyroscopeEnabled), sizeof(bool));
    file.read(reinterpret_cast<char*>(&m_settings.musicVolume), sizeof(float));
    file.read(reinterpret_cast<char*>(&m_settings.sfxVolume), sizeof(float));

    file.close();
    LOGI("SaveSystem: Settings loaded successfully");
    return true;
}
