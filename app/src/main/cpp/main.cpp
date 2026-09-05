// main.cpp
// Entry point for the native Android application.
// GameActivity calls android_main() on a dedicated native thread.
//
// All game logic is handled through Application::run().
// This file should remain minimal — no game logic here.

#include "core/Application.h"
#include "core/Log.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>

// Suppress C linkage warning from native_app_glue
extern "C" {

/**
 * android_main
 *
 * Called by GameActivity on the native game thread.
 * Owns the Application object for the game's lifetime.
 * Returns when the user exits or the OS terminates the activity.
 */
void android_main(android_app* app) {
    LOGI("android_main: starting Car Racing Game");

    Application application(app);
    application.run();

    LOGI("android_main: exiting");
}

} // extern "C"
