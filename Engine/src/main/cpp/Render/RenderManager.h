#ifndef EFFECTS2D_RENDERMANAGER_H
#define EFFECTS2D_RENDERMANAGER_H
#include "DateType.h"
#include "Log.h"
#include "Effect.h"
#include "EGLEnv.h"

class RenderManager {
public:
    RenderManager(EGLEnv& egl);
    ~RenderManager();

    void Init();
    void Destroy();

    bool DrawByTouchEvent(TouchInfo info);
    bool DrawByFrameCallback(double time);

    void SetEffect(EffectType type);
private:
    Effect mCurrentEffect;
    EGLEnv& mEGLEnv;
};


#endif //EFFECTS2D_RENDERMANAGER_H
