#include <jni.h>
#include <string>

#include "ms_opengles/MSNDKGLESRender.h"

MSNDKGLESRender* m_ndkGLESRender;

extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSOpenGLJNIRender_jniInitGL(JNIEnv *env, jobject thiz,
                                                        jobject asset_manager,jint type) {
    AAssetManager *aAssetManager= AAssetManager_fromJava(env,asset_manager);
    m_ndkGLESRender =new MSNDKGLESRender(type);
    if (aAssetManager!= nullptr){
        m_ndkGLESRender->SetupAssetManager(aAssetManager);
    }

    m_ndkGLESRender->InitGL();


}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSOpenGLJNIRender_jniPaintGL(JNIEnv *env, jobject thiz) {
    m_ndkGLESRender->PaintGL();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSOpenGLJNIRender_jniResizeGL(JNIEnv *env, jobject thiz, jint width,
                                                          jint height) {
    m_ndkGLESRender->ResizeGL(width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSOpenGLJNIRender_jniOnDestroy(JNIEnv *env, jobject thiz) {
    if (m_ndkGLESRender!= nullptr){
        delete m_ndkGLESRender;
        m_ndkGLESRender= nullptr;
    }
}