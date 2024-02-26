#include <GLES3/gl3.h>
#include <EGL//egl.h>
#include <EGL/eglext.h>

#include "EGLEnv.h"
#include "Log.h"

EGLEnv::EGLEnv() :
    mWindow(nullptr),
    mConfig(nullptr),
    mDisplay(EGL_NO_DISPLAY),
    mContext(EGL_NO_CONTEXT),
    mSurface(EGL_NO_SURFACE),
    mWidth(0),
    mHeight(0) {

}

bool EGLEnv::InitContext() {
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == mDisplay) {
        LOGE("get display failed: %d", eglGetError());
        return false;
    }

    EGLint majorVersion = 0;
    EGLint minorVersion = 0;
    if (!eglInitialize(mDisplay, &majorVersion, &minorVersion)) {
        LOGE("eglInitialize failed: %d", eglGetError());
        return false;
    } else {
        LOGD("egl initialized success, version:%d.%d",minorVersion,minorVersion);
    }

    //egl属性配置，后续设置为可配置
    EGLint numConfigs = 0;
    EGLint attribList[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_NONE
    };
    if (!eglChooseConfig(mDisplay, attribList, &mConfig, 1, &numConfigs)) {
        LOGE("eglChooseConfig failed: %d", eglGetError());
        return false;
    }
    if (numConfigs < 1) {
        LOGE("eglChooseConfig failed:numConfigs is %d", numConfigs);
        return false;
    }

    EGLint contextAttrs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    mContext = eglCreateContext(mDisplay, mConfig, EGL_NO_CONTEXT, contextAttrs);
    if (EGL_NO_CONTEXT == mContext) {
        LOGE("eglCreateContext failed: %d",eglGetError());
        return false;
    }
    LOGD("init context success");
    return true;
}

bool EGLEnv::SetWindow(ANativeWindow *window) {
    if (nullptr == window) {
        LOGE("window in nullptr");
        return false;
    }
    if (EGL_NO_CONTEXT == mContext || EGL_NO_DISPLAY == mDisplay || nullptr == mConfig) {
        LOGE("EGLContext is not exist");
        return false;
    }

    //销毁旧的surface和window
    DestroySurface();

    mWindow = window;
    mWidth = ANativeWindow_getWidth(mWindow);
    mHeight = ANativeWindow_getHeight(mWindow);

    mSurface = eglCreateWindowSurface(mDisplay, mConfig, mWindow, nullptr);
    if (EGL_NO_SURFACE == mSurface) {
        LOGE("eglCreateWindowSurface failed: %d", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)) {
        LOGE("eglMakeCurrent failed: %d", eglGetError());
        return false;
    }

    LOGD("eglCreateWindowSurface success");
    return true;
}

void EGLEnv::SwapBuffer() {
    if (EGL_NO_DISPLAY == mDisplay || EGL_NO_SURFACE == mSurface) {
        LOGE("display or surface is not exist");
        return;
    }
    eglSwapBuffers(mDisplay, mSurface);
    EGLenum code = eglGetError();
    if (code != EGL_SUCCESS) {
        LOGE("gl swapBuffer error: %d",code);
    }
}

void EGLEnv::DestroyContext() {
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (mSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mDisplay, mSurface);
        mSurface = EGL_NO_SURFACE;
    }
    eglDestroyContext(mDisplay, mContext);
    mContext = EGL_NO_CONTEXT;
    eglTerminate(mDisplay);

    if (mWindow != nullptr) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
}

void EGLEnv::DestroySurface() {
    if (mSurface != nullptr) {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, mContext);
        eglDestroySurface(mDisplay, mSurface);
        mSurface = EGL_NO_SURFACE;
        LOGD("destroy history EGL surface");
    }

    if (mWindow != nullptr) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
        LOGD("release history native window");
    }
}
