package com.effects2D.engine

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.TextureView
import android.view.TextureView.SurfaceTextureListener

class EffectsTextureView(context:Context?, attrs:AttributeSet?, defStyleAttr:Int, defStylesRes:Int):
    TextureView(context!!, attrs, defStyleAttr, defStylesRes), SurfaceTextureListener {
    companion object {
        const val TAG = "EffectsTextureView"
    }

    private var mEngine:EffectsEngine = EffectsEngine()  //native 渲染引擎的kotlin端实例
    private var mSurface:Surface? = null
    private var mRenderMode = 0           //0:vsync驱动  1:touch事件驱动

    private var mSurfaceWidth = 0
    private var mSurfaceHeight = 0

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?,defStyleAttr: Int) : this(
        context,
        attrs,
        defStyleAttr,
        0
    )

    init {
        surfaceTextureListener = this
        isOpaque = false
        mEngine.startRenderThread()
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        if (1 != mRenderMode) {
            return false
        }
        mEngine.handleTouchEvent(event)
        return true
    }


    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        mSurface = Surface(surface)
        mSurfaceWidth = width
        mSurfaceHeight = height
        Log.d(TAG, "onSurfaceTextureAvailable: ($width,$height)")
        mSurface?.let {
            mEngine.setSurface(it)
            mEngine.startAnimation()
        }

    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
        Log.d(TAG, "onSurfaceTextureSizeChanged")
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        return false
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {

    }

}