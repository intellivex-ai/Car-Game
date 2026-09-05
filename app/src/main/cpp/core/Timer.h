#pragma once
#include <cstdint>

/**
 * Timer
 *
 * High-resolution wall-clock timer using CLOCK_MONOTONIC.
 * Provides elapsed time in seconds (double) and nanoseconds (int64).
 *
 * Usage:
 *   Timer t;
 *   t.reset();
 *   // ... do work ...
 *   double dt = t.getElapsedSeconds();  // time since last reset()
 *   t.reset();
 */
class Timer {
public:
    Timer();

    /** Reset the start point to now. */
    void reset();

    /** Seconds elapsed since last reset(). Returns a double for precision. */
    double getElapsedSeconds() const;

    /** Nanoseconds elapsed since last reset(). */
    int64_t getElapsedNanos() const;

private:
    int64_t m_startNs = 0;

    static int64_t nowNs();
};
