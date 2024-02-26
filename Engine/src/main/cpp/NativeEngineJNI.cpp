#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#include "RenderThread.h"
#include "Log.h"

#define CHECK(handle) \
    if (0L == handle) { \
        LOGE("No native thread exist!"); \
        return;       \
    }                 \
    auto* renderThread = (RenderThread*)(handle);
extern "C"
JNIEXPORT jlong JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_startRenderThread(JNIEnv *env, jobject thiz) {
    auto *renderThread = new RenderThread();
    return (jlong)renderThread;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_handleTouchEvent(JNIEnv *env, jobject thiz,
                                                                     jlong ptr, jfloat x, jfloat y,
                                                                     jint type) {
    CHECK(ptr)
    renderThread->HandleTouchEvent(type, x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_setSurface(JNIEnv *env, jobject thiz, jlong ptr,
                                                               jobject surface) {
    CHECK(ptr)
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nullptr == nativeWindow) {
        LOGE("Get native window failed");
        return;
    }
    renderThread->SetWindow(nativeWindow);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_startAnimation(JNIEnv *env, jobject thiz,
                                                                   jlong ptr) {
    CHECK(ptr)
    renderThread->StartAnimation();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_stopAnimation(JNIEnv *env, jobject thiz,
                                                                  jlong ptr) {
    CHECK(ptr)
    renderThread->StopAnimation();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_effects2D_engine_EffectsJNI_00024Companion_exitRenderThread(JNIEnv *env, jobject thiz,
                                                                     jlong ptr) {
    CHECK(ptr)
    renderThread->ExitRenderThread();
}