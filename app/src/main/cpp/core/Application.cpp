#include "core/Application.h"
#include "core/Log.h"

// TODO Phase 3: include renderer, scene manager, and other systems here.
// For now this is a minimal stub that keeps the app alive without crashing.

Application::Application(android_app* app)
    : m_app(app) {
    // Register the lifecycle command callback with android_app_glue.
    app->userData = this;
    app->onAppCmd = handleCmd;
}

Application::~Application() = default;

void Application::run() {
    LOGI("Application::run started");

    m_running = true;

    while (m_running) {
        pollEvents();

        // Only render when the window is ready and app is focused.
        // Rendering implementation added in Phase 3.
        if (m_windowReady && m_focused) {
            // TODO: game loop tick + render
        }
    }

    LOGI("Application::run exiting");
}

void Application::pollEvents() {
    int events;
    android_poll_source* source;

    // Poll for events without blocking (timeout = 0).
    // If the app has no window yet or is not focused, block (timeout = -1)
    // to avoid busy-waiting and wasting battery.
    int timeoutMs = (m_windowReady && m_focused && m_running) ? 0 : -1;

    if (ALooper_pollOnce(timeoutMs, nullptr, &events,
                         reinterpret_cast<void**>(&source)) >= 0) {
        if (source != nullptr) {
            source->process(m_app, source);
        }
    }
}

void Application::onWindowCreated(ANativeWindow* /*window*/) {
    LOGI("Application::onWindowCreated");
    m_windowReady = true;
    // TODO Phase 3: initialize renderer surface here
}

void Application::onWindowDestroyed() {
    LOGI("Application::onWindowDestroyed");
    m_windowReady = false;
    // TODO Phase 3: destroy renderer surface here
}

void Application::onFocusChanged(bool focused) {
    LOGI("Application::onFocusChanged: %s", focused ? "gained" : "lost");
    m_focused = focused;
}

void Application::onPause() {
    LOGI("Application::onPause");
    // TODO Phase 12: pause audio
}

void Application::onResume() {
    LOGI("Application::onResume");
    // TODO Phase 12: resume audio
}

// static
void Application::handleCmd(android_app* app, int32_t cmd) {
    auto* self = reinterpret_cast<Application*>(app->userData);
    if (!self) return;

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window) {
                self->onWindowCreated(app->window);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            self->onWindowDestroyed();
            break;

        case APP_CMD_GAINED_FOCUS:
            self->onFocusChanged(true);
            break;

        case APP_CMD_LOST_FOCUS:
            self->onFocusChanged(false);
            break;

        case APP_CMD_PAUSE:
            self->onPause();
            break;

        case APP_CMD_RESUME:
            self->onResume();
            break;

        case APP_CMD_DESTROY:
            self->m_running = false;
            break;

        default:
            break;
    }
}
