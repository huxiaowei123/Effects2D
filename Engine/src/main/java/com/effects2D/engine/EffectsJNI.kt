package com.effects2D.engine

import android.view.Surface

class EffectsJNI {
    companion object {
        // Used to load the 'engine' library on application startup.
        init {
            System.loadLibrary("engine")
        }

        external fun startRenderThread():Long
        external fun handleTouchEvent(ptr: Long, x: Float, y: Float, type: Int)
        external fun setSurface(ptr: Long, surface: Surface)
        external fun startAnimation(ptr: Long)
        external fun stopAnimation(ptr:Long)
        external fun exitRenderThread(ptr:Long)
    }
}