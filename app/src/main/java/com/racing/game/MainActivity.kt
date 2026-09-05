package com.racing.game

import androidx.games.activity.GameActivity
import android.os.Bundle
import android.view.View
import android.view.WindowManager

/**
 * MainActivity — minimal host for the C++ game engine.
 *
 * All game logic lives in native C++ via GameActivity / android_main().
 * This Kotlin class only handles Android-layer concerns:
 *   - Full-screen immersive mode
 *   - Keeping the screen awake during gameplay
 *
 * Do NOT add game logic here.
 */
class MainActivity : GameActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Keep screen on while the game is running
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)

        // Enter full-screen immersive mode immediately
        applyImmersiveMode()
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            applyImmersiveMode()
        }
    }

    /**
     * Hides the system status bar and navigation bar.
     * Uses sticky immersive mode so bars stay hidden even after accidental swipes.
     */
    @Suppress("DEPRECATION")
    private fun applyImmersiveMode() {
        window.decorView.systemUiVisibility = (
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            or View.SYSTEM_UI_FLAG_FULLSCREEN
        )
    }
}
