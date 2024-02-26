#ifndef EFFECTS2D_EGLENV_H
#define EFFECTS2D_EGLENV_H

#include <EGL/egl.h>
#include <android/native_window.h>


class EGLEnv {
public:
    EGLEnv();
    virtual ~EGLEnv() = default;

    bool InitContext();
    bool SetWindow(ANativeWindow* window);
    void SwapBuffer();

    void DestroyContext();
    void DestroySurface();

    int GetWidth() {return mWidth;}
    int GetHeight(){return mHeight;}

private:
    ANativeWindow* mWindow;
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
    EGLConfig mConfig;

    int mWidth;
    int mHeight;


};


#endif //EFFECTS2D_EGLENV_H
