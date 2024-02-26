#ifndef EFFECTS2D_RENDERTHREAD_H
#define EFFECTS2D_RENDERTHREAD_H

#include <pthread.h>
#include <android/looper.h>
#include <android/choreographer.h>
#include <android/native_window.h>

#include "DateType.h"
#include "EnumType.h"
#include "EGLEnv.h"
#include "RenderManager.h"
class RenderThread {
public:
    RenderThread();
    virtual ~RenderThread();

    void HandleTouchEvent(int action, float x, float y);
    void SetWindow(ANativeWindow *window);
    void ExitRenderThread();
    void StartAnimation();
    void StopAnimation();

private:
    static void* ThreadLauncher(void* handle);
    static int ALooperCallback(int fb, int event, void* data);
    void ReadMessage(void *buffer, int size);
    bool WriteMessage(unsigned int type, unsigned int size, void* data) const;
    int MessageProcess();
    void ThreadLoop();
    void Wait();
    void Signal();

    void PostFrameCallback();
    static void FrameCallback(long frameTimeNanos, void* data);
    void DrawFrame(double time);

private:
    pthread_t mThreadID;
    pthread_mutex_t mMutex;
    pthread_cond_t mCond;

    bool mThreadExit = false;

    int mReadFd = 0;
    int mWriteFd = 0;

    ALooper* mLooper = nullptr;
    AChoreographer* mChoreographer = nullptr;
    EGLEnv mEGLEnv;
    RenderManager mRenderManager;

    long mFrameStartTimeNanos;
    bool mIsFrameStarted;
    bool mEnableFrameAnimation;

};


#endif //EFFECTS2D_RENDERTHREAD_H
