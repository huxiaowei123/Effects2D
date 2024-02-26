#include "GLProgram.h"
#include "Log.h"

GLProgram::GLProgram():
    mProgramID(GL_NONE) {

}

GLProgram::~GLProgram() {
    DestroyProgram();
}

void GLProgram::CreateProgram(std::string vertShaderCode, std::string fragShaderCode) {
    //create program
    mProgramID = glCreateProgram();
    if (GL_NONE == mProgramID) {
        LOGE("create program failed");
        return;
    }

    //attach vertex shader
    GLuint vertexShader = LoadShader(vertShaderCode,GL_VERTEX_SHADER);
    if (0 == vertexShader) {
        LOGE("load vertex shader failed");
        return;
    }
    glAttachShader(mProgramID, vertexShader);

    //attach fragment shader
    GLuint fragmentShader = LoadShader(fragShaderCode, GL_FRAGMENT_SHADER);
    if (0 == fragmentShader) {
        LOGE("load fragment shader failed");
        return;
    }
    glAttachShader(mProgramID, fragmentShader);

    //link program
    glLinkProgram(mProgramID);

    //check Link status
    GLint Linked;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &Linked);

    if (!Linked) {
        GLint infoLength = 0;
        glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLength);
            glGetProgramInfoLog(mProgramID, infoLength, NULL, infoLog);
            LOGE("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(mProgramID);
        return;
    }

    glUseProgram(mProgramID);
}

GLuint GLProgram::LoadShader(std::string code, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (0 == shader) {
        LOGE("create %d shader failed",type);
        return 0;
    }

    const GLchar *codes[2];
    //default shader head
    const char *headCode = "#version 320 es\nprecision highp float;\n";
    codes[0] = headCode;
    codes[1] = code.c_str();
    //load shader source
    glShaderSource(shader, 2, codes, NULL);
    glCompileShader(shader);

    //check compile status
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint infoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLength);
            glGetShaderInfoLog(shader, infoLength, NULL, infoLog);
            LOGE("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    LOGD("create shader success,type:%d id:%d",type, shader);
    return shader;
}

void GLProgram::Use() {
    glUseProgram(mProgramID);
}

void GLProgram::Disable() {
    glUseProgram(0);
}

void GLProgram::SetInt(std::string symbol, int value) {
    GLint loc = glGetUniformLocation(mProgramID, symbol.c_str());
    glUseProgram(mProgramID);
    glUniform1i(loc, value);
}

void GLProgram::SetFloat(std::string symbol, float value) {
    GLint loc = glGetUniformLocation(mProgramID, symbol.c_str());
    glUseProgram(mProgramID);
    glUniform1f(loc, value);
}

void GLProgram::DestroyProgram() {
    if (mProgramID != GL_NONE) {
        glDeleteProgram(mProgramID);
        mProgramID = GL_NONE;
    }
}

