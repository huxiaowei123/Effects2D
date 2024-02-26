#include "GLTexture.h"

GLTexture::GLTexture():
    mTextureID(0){

}

GLTexture::~GLTexture() {
    if (mTextureID != 0)
    {
        glDeleteTextures(1, &mTextureID);
        mTextureID = 0;
    }
}

void GLTexture::CreateTexture2D(GLsizei width, GLsizei height, void *data, GLint wrapParam, GLint filterParam,
                                GLenum internalFormat, GLenum format, GLenum type, GLint level) {
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterParam);

}

void GLTexture::BindTexture(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
