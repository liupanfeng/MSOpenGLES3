#include <jni.h>

#include "MSGLVideoRender.h"
#include "MSAVCodecHandler.h"
#include "ms_opengles/MSGLCommonDef.h"


JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

MSAVCodecHandler m_MSAVCodecHandler;
MSGLVideoRender m_MSGLVideoRender;



void updateVideoData(MSYUVData_Frame* yuvFrame,unsigned long userData);


extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkInitVideoPlayer(JNIEnv *env, jobject thiz) {
    m_MSAVCodecHandler.SetupUpdateVideoCallback(updateVideoData,NULL);//设置回调
    env->GetJavaVM(&g_jvm);  //获取jvm
    g_obj=env->NewGlobalRef(thiz); //获取java的引用
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkStartPlayerWithFile(JNIEnv *env, jobject thiz,
                                                            jstring file_name) {
    std::string fileName=env->GetStringUTFChars(file_name,JNI_OK);
    LOGD("file name is %s",fileName.c_str());
    m_MSAVCodecHandler.StopPlayVideo();  //播放之前 先停止
    m_MSAVCodecHandler.SetVideoFilePath(fileName);  //设置文件路径
    m_MSAVCodecHandler.InitVideoCodec(); //初始化解码器
    m_MSAVCodecHandler.StartPlayVideo(); //开始播放
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkPauseVideoPlay(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkStopVideoPlayer(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkGetVideoSizeRatio(JNIEnv *env, jobject thiz) {
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkGetVideoTotalSeconds(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkSeekMedia(JNIEnv *env, jobject thiz, jfloat n_pos) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkInitGL(JNIEnv *env, jobject thiz, jobject asset_manager) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkPaintGL(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkResizeGL(JNIEnv *env, jobject thiz, jint width,
                                                 jint height) {
}

void updateVideoData(MSYUVData_Frame* yuvFrame,unsigned long userData){

}