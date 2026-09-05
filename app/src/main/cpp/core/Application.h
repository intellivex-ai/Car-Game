#pragma once
#include <game-activity/native_app_glue/android_native_app_glue.h>

/**
 * Application
 *
 * Wraps the android_app* lifecycle. Owns the GameLoop, Renderer, and all
 * top-level systems. Responds to GameActivity lifecycle commands (window
 * created/destroyed, pause/resume, focus gained/lost).
 *
 * Created once in android_main(); destroyed when android_main() returns.
 */
class Application {
public:
    explicit Application(android_app* app);
    ~Application();

    // Non-copyable
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
     * Main entry — runs until the android_app requests termination.
     * Internally polls lifecycle events and drives the game loop.
     */
    void run();

    /** Called by android_app_glue when the native window is ready to draw on. */
    void onWindowCreated(ANativeWindow* window);

    /** Called when the native window is being destroyed. */
    void onWindowDestroyed();

    /** Called when the app loses/regains focus (e.g., notification shade pulled down). */
    void onFocusChanged(bool focused);

    /** Called when the app is paused (background). */
    void onPause();

    /** Called when the app resumes from the background. */
    void onResume();

private:
    android_app* m_app = nullptr;
    bool         m_running = false;
    bool         m_windowReady = false;
    bool         m_focused = false;

    void pollEvents();
    static void handleCmd(android_app* app, int32_t cmd);
};
