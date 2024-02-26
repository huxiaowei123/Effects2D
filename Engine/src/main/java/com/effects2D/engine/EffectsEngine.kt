package com.effects2D.engine

import android.util.Log
import android.view.MotionEvent
import android.view.Surface


class EffectsEngine {

    private var mHandle = 0L   //native端线程句柄

    fun startRenderThread() {
        mHandle = EffectsJNI.startRenderThread()
    }

    fun handleTouchEvent(event: MotionEvent?) {
        val x = event?.x
        val y = event?.y
        val type = event?.action
        EffectsJNI.handleTouchEvent(mHandle, x!!, y!!, type!!)
    }

    fun setSurface(mSurface: Surface) {
        EffectsJNI.setSurface(mHandle, mSurface)
    }

    fun startAnimation() {
        EffectsJNI.startAnimation(mHandle);
    }
}