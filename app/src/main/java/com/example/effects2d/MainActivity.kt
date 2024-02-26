package com.example.effects2d

import android.annotation.SuppressLint
import android.app.Activity
import android.os.Bundle
import android.widget.TextView
import com.effects2D.engine.EffectsTextureView

class MainActivity : Activity() {
    private var mTexture:EffectsTextureView? = null
    @SuppressLint("MissingInflatedId")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mTexture = findViewById(R.id.texture)
    }
}

