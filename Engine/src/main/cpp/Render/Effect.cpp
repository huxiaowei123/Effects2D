#include "Effect.h"
#include "Log.h"

const std::string vertexShaderCode = R"(
layout (location = 0) in vec4 a_vertex;
out vec2 uv;
void main() {
    uv = a_vertex.zw;
    gl_Position = vec4(a_vertex.xy, 0.0, 1.0);
}
)";

const std::string fragmentShaderCode = R"(
layout (location = 0) out vec4 outColor;
in vec2 uv;
uniform float u_time;
void main() {
    outColor = vec4(uv.x, uv.y, abs(sin(u_time)), 1.0);
}
)";
Effect::Effect():
    mProgram(),
    mQuadVBO(0),
    mTimeLoc(0),
    mViewPort(),
    mType(DEFAULT_EFFECT)
{

}

bool Effect::Init() {
    mProgram.CreateProgram(vertexShaderCode, fragmentShaderCode);
    if (mProgram.GetProgramID() == GL_NONE) {
        LOGE("program create failed");
        return false;
    }
    mTimeLoc = glGetUniformLocation(mProgram.GetProgramID(),"u_time");
    glUniform1f(mTimeLoc, 0.0f);
    float quadVertex[] = {
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
    };

    if (mQuadVBO != 0) {
        glDeleteBuffers(1, &mQuadVBO);
        mQuadVBO = 0;
    }
    glGenBuffers(1, &mQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertex), quadVertex, GL_STATIC_DRAW);
    LOGD("init program(%d)",mProgram.GetProgramID());
    return (mQuadVBO != 0);
}

void Effect::Destroy() {
    mProgram.DestroyProgram();
    if (mQuadVBO != 0) {
        glDeleteBuffers(1, &mQuadVBO);
        mQuadVBO = 0;
    }
}

bool Effect::Draw(double time) {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(mViewPort.left, mViewPort.bottom, mViewPort.width, mViewPort.height);
    mProgram.Use();
    glUniform1f(mTimeLoc, time);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    return true;
}

void Effect::SetViewPort(int left, int bottom, int width, int height) {
    mViewPort.left = left;
    mViewPort.bottom = bottom;
    mViewPort.width = width;
    mViewPort.height = height;
}

bool Effect::Draw(TouchInfo info) {
    return false;
}
