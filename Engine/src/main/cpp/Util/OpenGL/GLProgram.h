#ifndef EFFECTS2D_GLPROGRAM_H
#define EFFECTS2D_GLPROGRAM_H

#include <GLES3/gl32.h>
#include <string>
#include <list>

class GLProgram {
public:
    GLProgram();
    ~GLProgram();

    void CreateProgram(std::string vertShaderCode, std::string fragShaderCode);
    void Use();
    void Disable();

    void SetInt(std::string symbol, int value);
    void SetFloat(std::string symbol, float value);

    GLuint GetProgramID(){return mProgramID;}
    void DestroyProgram();

private:
    GLuint LoadShader(std::string code, GLenum type);


private:
    GLuint mProgramID;
};


#endif //EFFECTS2D_GLPROGRAM_H
