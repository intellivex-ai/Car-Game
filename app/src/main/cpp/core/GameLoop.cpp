#include "core/GameLoop.h"
#include <algorithm>

void GameLoop::setCallbacks(UpdateFn updateFn, RenderFn renderFn) {
    m_updateFn = std::move(updateFn);
    m_renderFn = std::move(renderFn);
}

void GameLoop::tick(double realDeltaTime) {
    // Cap the delta to prevent spiral of death on slow frames.
    const double maxDelta = kFixedDt * kMaxStepsPerFrame;
    realDeltaTime = std::min(realDeltaTime, maxDelta);

    m_accumulator += realDeltaTime;

    // Run as many fixed physics steps as the accumulator allows.
    while (m_accumulator >= static_cast<double>(kFixedDt)) {
        if (m_updateFn) {
            m_updateFn(kFixedDt);
        }
        m_accumulator -= static_cast<double>(kFixedDt);
    }

    // Interpolation alpha in [0, 1) — how far we are into the next step.
    const float alpha = static_cast<float>(m_accumulator / static_cast<double>(kFixedDt));

    if (m_renderFn) {
        m_renderFn(alpha);
    }
}

void GameLoop::reset() {
    m_accumulator = 0.0;
}
