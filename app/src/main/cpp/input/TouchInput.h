#pragma once
#include "gameplay/CarInput.h"
#include <game-activity/GameActivity.h>
#include <cstdint>

/**
 * TouchInput
 *
 * Reads GameActivity motion events and maps them to virtual button states.
 *
 * Button layout (landscape, normalized 0–1 screen coords):
 *
 *   LEFT  button:   x ∈ [0.02, 0.15], y ∈ [0.40, 0.90]
 *   RIGHT button:   x ∈ [0.17, 0.32], y ∈ [0.40, 0.90]
 *   THROTTLE:       x ∈ [0.76, 0.95], y ∈ [0.05, 0.45]
 *   BRAKE:          x ∈ [0.76, 0.95], y ∈ [0.48, 0.70]
 *   HANDBRAKE:      x ∈ [0.76, 0.95], y ∈ [0.73, 0.90]
 *   PAUSE:          x ∈ [0.44, 0.56], y ∈ [0.00, 0.12]
 *
 * Multi-touch is supported (up to kMaxPointers simultaneous touches).
 * Each frame, each active pointer's position is checked against each button region.
 * A button is "pressed" if any pointer is inside it.
 *
 * Steering output:
 *   - If LEFT only:  -1.0
 *   - If RIGHT only: +1.0
 *   - If both:        0.0
 *   - If neither:     0.0
 *
 * GameActivity delivers touch events via android_app's input buffer.
 * Call processMotionEvents() each frame with the android_app* to drain the buffer.
 */
class TouchInput {
public:
    static constexpr int kMaxPointers = 10;

    TouchInput() = default;

    /**
     * Drains and processes all pending motion events from the GameActivity input buffer.
     * Call once per frame before querying any state.
     */
    void processMotionEvents(android_app* app);

    // --- State queries (valid after processMotionEvents) ---
    float getSteering()  const;   // [-1, 1]
    float getThrottle()  const;   // [0, 1]
    float getBrake()     const;   // [0, 1]
    bool  getHandbrake() const;
    bool  getPause()     const;   // one-shot: returns true once per press

    /** Resets the one-shot pause state. Call after handling the pause press. */
    void consumePause();

private:
    struct Pointer {
        float x = 0.0f;  // normalized [0,1]
        float y = 0.0f;  // normalized [0,1]
        bool  active = false;
    };

    Pointer m_pointers[kMaxPointers] = {};

    bool m_pauseTriggered = false;
    bool m_pauseConsumed  = false;

    struct Rect { float x0, y0, x1, y1; };

    static bool hitTest(const Rect& r, float x, float y);
    bool anyPointerIn(const Rect& r) const;

    // Button regions (defined in .cpp)
    static const Rect kRectLeft;
    static const Rect kRectRight;
    static const Rect kRectThrottle;
    static const Rect kRectBrake;
    static const Rect kRectHandbrake;
    static const Rect kRectPause;
};
