#ifndef EFFECTS2D_EFFECT_H
#define EFFECTS2D_EFFECT_H

#include "string"
#include <GLES3/gl32.h>
#include "GLProgram.h"
#include "DateType.h"

class Effect {
public:
    Effect();
    virtual ~Effect() = default;

    virtual bool Init();
    virtual bool Draw(double time);
    virtual bool Draw(TouchInfo info);
    virtual void Destroy();

    void SetViewPort(int left, int bottom, int width, int height);
    EffectType GetEffectType() {return mType;}

private:
    GLProgram mProgram;
    GLuint mQuadVBO;
    GLint mTimeLoc;
    ViewPort mViewPort;

    EffectType mType;
};


#endif //EFFECTS2D_EFFECT_H
