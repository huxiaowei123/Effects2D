#include "RenderManager.h"

RenderManager::RenderManager(EGLEnv& egl):
    mEGLEnv(egl),
    mCurrentEffect(){
    mCurrentEffect = Effect();
}

RenderManager::~RenderManager() {

}

void RenderManager::Init() {
    LOGD("Init renderManager");
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    mCurrentEffect.Init();
    mCurrentEffect.SetViewPort(0, 0, mEGLEnv.GetWidth(), mEGLEnv. GetHeight());
}

void RenderManager::Destroy() {
    mCurrentEffect.Destroy();
}

bool RenderManager::DrawByTouchEvent(TouchInfo info) {
    return mCurrentEffect.Draw(info);
}

bool RenderManager::DrawByFrameCallback(double time) {
    mCurrentEffect.Draw(time);
    mEGLEnv.SwapBuffer();
    return false;
}

void RenderManager::SetEffect(EffectType type) {
    if (type == mCurrentEffect.GetEffectType())
    {
        LOGD("effect %d id already set", type);
        return;
    }

    mCurrentEffect.Destroy();
    switch (type) {
        case DEFAULT_EFFECT:
        {
            mCurrentEffect = Effect();
            mCurrentEffect.Init();
            //是否要设置viewport
            break;
        }
    }
}
