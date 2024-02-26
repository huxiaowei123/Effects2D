#ifndef EFFECTS2D_LOG_H
#define EFFECTS2D_LOG_H

#include <android/log.h>

#define LOG_TAG "EffectsEngine"

#define LOGE(format,...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s:%d:%s: " format "\n",__FILE__, __LINE__, __func__, ##__VA_ARGS__))
#define LOGD(format,...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s:%d:%s: " format "\n",__FILE__, __LINE__, __func__, ##__VA_ARGS__))
#define LOGI(format,...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s:%d:%s: " format "\n",__FILE__, __LINE__, __func__, ##__VA_ARGS__))

#define CHECK_GL_ERROR_DEBUG() \
    do {                       \
    GLenum __error = glGetError\
    if (__error) {             \
            __android_log_print(ANDROID_LOG_ERROR,    \
                "GLError",                            \
                "0x%04X in %s function:%s line:%d",   \
                __error,__FILE__, __LINE__, __func__);\
        }                                             \
    } while (false)



#endif //EFFECTS2D_LOG_H
