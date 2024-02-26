#ifndef EFFECTS2D_GLTEXTURE_H
#define EFFECTS2D_GLTEXTURE_H
#include <GLES3/gl32.h>

class GLTexture {
public:
    GLTexture();
    ~GLTexture();

    void CreateTexture2D(GLsizei width, GLsizei height, void* data, GLint wrapParam = GL_CLAMP_TO_EDGE,
                         GLint filterParam = GL_NEAREST,GLenum internalFormat = GL_RGBA,
                         GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLint level = 0);

    void BindTexture(int index);

private:
    GLuint mTextureID;
};


#endif //EFFECTS2D_GLTEXTURE_H
