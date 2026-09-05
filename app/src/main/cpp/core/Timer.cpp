#include "core/Timer.h"
#include <time.h>

Timer::Timer() {
    reset();
}

void Timer::reset() {
    m_startNs = nowNs();
}

double Timer::getElapsedSeconds() const {
    return static_cast<double>(getElapsedNanos()) / 1.0e9;
}

int64_t Timer::getElapsedNanos() const {
    return nowNs() - m_startNs;
}

// static
int64_t Timer::nowNs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<int64_t>(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
}
