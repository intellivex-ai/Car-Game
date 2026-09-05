#pragma once
#include "save/GameSettings.h"
#include <string>
#include <unordered_map>
#include <cstdint>

/**
 * SaveSystem
 *
 * Persists game data to app-private internal storage.
 * No JNI required — the save path is passed in at init() after being
 * retrieved in Application from android_app->activity->internalDataPath.
 *
 * File format (save.bin):
 *   [4 bytes] magic "RGSV"
 *   [2 bytes] version (uint16, currently 1)
 *   [4 bytes] record count (uint32)
 *   Per record:
 *     [32 bytes] key (null-padded ASCII)
 *     [8 bytes]  value (double, reinterpreted as needed)
 *
 * Separate per-track best lap times use keys like "best_lap_0", "best_lap_1".
 *
 * Usage:
 *   SaveSystem save;
 *   save.init("/data/user/0/com.racing.game/files");
 *   save.loadSettings(settings);
 *   // ... play game ...
 *   settings.gyroSensitivity = 1.5f;
 *   save.saveSettings(settings);
 *   save.setBestLap(0, 81.3f);
 */
class SaveSystem {
public:
    static constexpr int kMaxTracks = 16;

    SaveSystem() = default;

    /**
     * Set the directory path where save.bin is stored.
     * Typically android_app->activity->internalDataPath.
     */
    void init(const char* internalDataPath);

    /** Load settings from disk. Returns default settings if file not found. */
    bool loadSettings(GameSettings& out) const;

    /** Persist settings to disk. */
    bool saveSettings(const GameSettings& settings) const;

    /** Get best lap time for a track. Returns infinity if none recorded. */
    float getBestLap(int trackIndex) const;

    /** Update best lap time for a track. Only writes if newTime < current best. */
    bool setBestLap(int trackIndex, float newTime);

private:
    std::string m_savePath;

    // In-memory KV store (loaded on init, flushed on write)
    mutable std::unordered_map<std::string, double> m_kv;

    bool loadFromDisk() const;
    bool saveToDisk()   const;

    static constexpr uint32_t kMagic   = 0x52475356; // "RGSV"
    static constexpr uint16_t kVersion = 1;
};
