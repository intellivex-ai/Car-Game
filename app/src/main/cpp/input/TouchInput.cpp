#include "input/TouchInput.h"
#include "core/Log.h"

TouchInput::TouchInput() = default;

void TouchInput::processGameActivityMotionEvents(android_app* app) {
    if (!app || !app->inputBuffer) return;

    // Drain GameActivity MotionEvent buffer
    android_input_buffer* inputBuffer = app->inputBuffer;
    uint64_t count = GameActivityInputBuffer_getMotionEventCount(inputBuffer);
    if (count == 0) return;

    // Clear active touch points for fresh sample
    m_touchPoints.clear();

    for (uint64_t i = 0; i < count; ++i) {
        GameActivityMotionEvent* motionEvent = &inputBuffer->motionEvents[i];
        int32_t action = motionEvent->action & AMOTION_EVENT_ACTION_MASK;

        for (uint32_t p = 0; p < motionEvent->pointerCount; ++p) {
            float x = GameActivityPointerAxes_getX(&motionEvent->pointers[p]);
            float y = GameActivityPointerAxes_getY(&motionEvent->pointers[p]);

            if (action == AMOTION_EVENT_ACTION_DOWN ||
                action == AMOTION_EVENT_ACTION_POINTER_DOWN ||
                action == AMOTION_EVENT_ACTION_MOVE) {
                m_touchPoints.push_back({ x, y, motionEvent->pointers[p].id });
            }
        }
    }

    GameActivityInputBuffer_clear(inputBuffer);
}

CarInput TouchInput::toCarInput(int screenWidth, int screenHeight) const {
    CarInput input;
    if (screenWidth <= 0 || screenHeight <= 0) return input;

    float w = static_cast<float>(screenWidth);
    float h = static_cast<float>(screenHeight);

    bool steerLeft = false;
    bool steerRight = false;

    for (const auto& tp : m_touchPoints) {
        float normX = tp.x / w;
        float normY = tp.y / h;

        // Steering buttons (Bottom Left)
        if (normY > 0.6f && normX < 0.2f) {
            steerLeft = true;
        } else if (normY > 0.6f && normX >= 0.2f && normX < 0.4f) {
            steerRight = true;
        }

        // Throttle (Bottom Right)
        if (normY > 0.6f && normX > 0.8f) {
            input.throttle = 1.0f;
        }

        // Brake (Bottom Center-Right)
        if (normY > 0.6f && normX > 0.65f && normX <= 0.8f) {
            input.brake = 1.0f;
        }

        // Handbrake (Middle Right)
        if (normY > 0.4f && normY <= 0.6f && normX > 0.85f) {
            input.handbrake = true;
        }
    }

    if (steerLeft && !steerRight) input.steering = -1.0f;
    else if (steerRight && !steerLeft) input.steering = 1.0f;
    else input.steering = 0.0f;

    return input;
}
