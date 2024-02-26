#include <unistd.h>
#include <cerrno>
#include <cstring>


#include "Log.h"
#include "RenderThread.h"
#include <android/native_window.h>
RenderThread::RenderThread() :
    mThreadID(0),
    mMutex(),
    mCond(),
    mThreadExit(false),
    mEGLEnv(),
    mFrameStartTimeNanos(0),
    mIsFrameStarted(false),
    mEnableFrameAnimation(false),
    mRenderManager(mEGLEnv)
{
    //初始化线程同步的锁和信号量
    pthread_mutex_init(&mMutex, nullptr);
    pthread_cond_init(&mCond, nullptr);

    //线程间消息读写文件描述符初始化
    int MessagePipe[2];
    if(pipe(MessagePipe) != 0){
        LOGE("create pipe failed:%s",strerror(errno));
        return;
    }
    mReadFd = MessagePipe[0];
    mWriteFd = MessagePipe[1];

    //创建渲染线程
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&mThreadID, &attr,ThreadLauncher, this);
    pthread_setname_np(mThreadID, "RenderThread");

    //等待子线程创建成功
    LOGD("Render subThread is start launching...");
    Wait();
    LOGD("Render subThread is has launched.");
}

void *RenderThread::ThreadLauncher(void *handle) {
    auto* renderThread = static_cast<RenderThread*>(handle);
    renderThread->ThreadLoop();
    return nullptr;
}

void RenderThread::ThreadLoop() {
    mLooper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(mLooper, mReadFd, 0, ALOOPER_EVENT_INPUT, ALooperCallback, this);

    //初始化egl环境
    mEGLEnv.InitContext();
    //标记线程已启动
    Signal();

    while (true) {
        int result = ALooper_pollOnce(-1, nullptr, nullptr, nullptr);
        if (result == ALOOPER_POLL_ERROR || mThreadExit) {
            LOGE("render thread exit:%d,result:%d",mThreadExit, result);
            break;
        }
    }
    ALooper_removeFd(mLooper, mReadFd);

    delete this;
}

void RenderThread::Wait() {
    pthread_mutex_lock(&mMutex);
    pthread_cond_wait(&mCond, &mMutex);
    pthread_mutex_unlock(&mMutex);

}

void RenderThread::Signal() {
    pthread_mutex_lock(&mMutex);
    pthread_cond_signal(&mCond);
    pthread_mutex_unlock(&mMutex);
}

int RenderThread::ALooperCallback(int fb, int event, void *data) {
    return (static_cast<RenderThread*>(data))->MessageProcess();
}

bool RenderThread::WriteMessage(unsigned int type, unsigned int size, void *data) const {
    if (mWriteFd>0) {
        write(mWriteFd, &type, sizeof(type));
        if (size != 0 && data != nullptr) {
            write(mWriteFd, data, size);
        }
        return true;
    }
    return false;
}

void RenderThread::ReadMessage(void *buffer, int size) {
    read(mReadFd, buffer, size);
}

int RenderThread::MessageProcess() {
    MessageType messageType = MessageType::NONE;
    ReadMessage(&messageType, sizeof(MessageType));
    LOGD("read message:%d",messageType);

    switch (messageType) {
        case MessageType::EXIT: {
            mThreadExit = true;
            mEGLEnv.DestroyContext();
            mRenderManager.Destroy();
            break;
        }
        case MessageType::SET_WINDOW: {
            ANativeWindow *window = nullptr;
            ReadMessage(&window, sizeof(ANativeWindow*));
            mEGLEnv.SetWindow(window);
            Signal();
            mRenderManager.Init();
            break;
        }
        case MessageType::TOUCH_EVENT: {
            TouchInfo info{};
            ReadMessage(&info, sizeof(TouchInfo));
            mRenderManager.DrawByTouchEvent(info);
            break;
        }
        case MessageType::DO_ANIMATION: {
            mEnableFrameAnimation = true;
            PostFrameCallback();
            break;
        }
        case MessageType::STOP_ANIMATION: {
            mEnableFrameAnimation = false;
            break;
        }

        default:{

        }
    }

    //返回1会继续读取消息，否则停止。
    return 1;
}

RenderThread::~RenderThread() {
    int mutRes = pthread_mutex_destroy(&mMutex);
    int condRes = pthread_cond_destroy(&mCond);
    if (condRes == EBUSY) {
        LOGE("pthread_cond_destroy busy");
    }
    close(mReadFd);
    close(mWriteFd);
}

void RenderThread::PostFrameCallback() {
    if (nullptr == mChoreographer) {
        mChoreographer = AChoreographer_getInstance();
    }

    if (mChoreographer != nullptr && !mThreadExit) {
        AChoreographer_postFrameCallback(mChoreographer, RenderThread::FrameCallback, this);
        mIsFrameStarted = true;
        LOGD("post frame callback success");
    } else {
        LOGD("post frame failed: mChoreographer = %p  threadExit = %d", mChoreographer, mThreadExit);
    }
}

void RenderThread::FrameCallback(long frameTimeNanos, void *data) {
    auto renderThread = static_cast<RenderThread*>(data);
    if (renderThread->mIsFrameStarted) {
        renderThread->mFrameStartTimeNanos = frameTimeNanos;
        renderThread->mIsFrameStarted = false;
    }
    double time = 0.000000001*(frameTimeNanos - renderThread->mFrameStartTimeNanos);
    renderThread->DrawFrame(time);
    if (renderThread->mEnableFrameAnimation && !renderThread->mThreadExit) {
        AChoreographer_postFrameCallback(renderThread->mChoreographer, RenderThread::FrameCallback, renderThread);
    } else {
        LOGD("frame pause case enable(%d) threadExit(%d)", renderThread->mEnableFrameAnimation, renderThread->mThreadExit);
    }
}

void RenderThread::DrawFrame(double time) {
    mRenderManager.DrawByFrameCallback(time);
}

void RenderThread::HandleTouchEvent(int action, float x, float y) {
    TouchInfo info = TouchInfo();
    info.type = static_cast<TouchActionType>(action);
    info.x = x;
    info.y = y;
    WriteMessage(MessageType::TOUCH_EVENT, sizeof(TouchInfo), &info);
}

void RenderThread::SetWindow(ANativeWindow *window) {
    WriteMessage(MessageType::SET_WINDOW, sizeof(ANativeWindow *), &window);
    Wait();
}

void RenderThread::ExitRenderThread() {
    WriteMessage(MessageType::EXIT, 0, nullptr);
}

void RenderThread::StartAnimation() {
    WriteMessage(MessageType::DO_ANIMATION, 0, nullptr);
}

void RenderThread::StopAnimation() {
    WriteMessage(MessageType::STOP_ANIMATION, 0, nullptr);
}



