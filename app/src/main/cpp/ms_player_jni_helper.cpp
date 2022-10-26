#include <jni.h>

#include "MSGLVideoRender.h"
#include "MSAVCodecHandler.h"
#include "ms_opengles/MSGLCommonDef.h"


JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

MSAVCodecHandler m_MSAVCodecHandler;
MSGLVideoRender m_MSGLVideoRender;


/**
 * 更新视频数据
 * @param yuvFrame
 * @param userData
 */
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
    m_MSAVCodecHandler.StopPlayVideo();
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkGetVideoSizeRatio(JNIEnv *env, jobject thiz) {

    int width=m_MSAVCodecHandler.GetVideoWidth();
    int height=m_MSAVCodecHandler.GetVideoHeight();
    jfloat ratio=(jfloat)width/(jfloat)height;
    LOGD("VIDEO SIZE RATIO: %f",ratio);
    return ratio;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkGetVideoTotalSeconds(JNIEnv *env, jobject thiz) {
    jfloat  totalSeconds=m_MSAVCodecHandler.GetMediaTotalSeconds();
    return totalSeconds;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkSeekMedia(JNIEnv *env, jobject thiz, jfloat n_pos) {
    m_MSAVCodecHandler.SeekMedia(n_pos);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkInitGL(JNIEnv *env, jobject thiz, jobject asset_manager) {
     AAssetManager* assetManager= AAssetManager_fromJava(env,asset_manager);
    if (assetManager!= nullptr){
        m_MSGLVideoRender.SetupAssetManager(assetManager);
    }
     m_MSGLVideoRender.InitGL();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkPaintGL(JNIEnv *env, jobject thiz) {
    m_MSGLVideoRender.PaintGL();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_MSPlayer_ndkResizeGL(JNIEnv *env, jobject thiz, jint width,
                                                 jint height) {
    m_MSGLVideoRender.ResizeGL(width,height);
}


void updateVideoData(MSYUVData_Frame* yuvFrame,unsigned long userData){
    if (yuvFrame== nullptr){
        return;
    }

    m_MSGLVideoRender.RendVideo(yuvFrame);
    LOGD("UPDATE VIDEO DATA: %d %d",yuvFrame->width,yuvFrame->height);

   JNIEnv* jniEnv;

    jmethodID  m_methodID;
    jclass  m_class;

    //Attach主线程
    if (g_jvm->AttachCurrentThread(&jniEnv, nullptr)!=JNI_OK){
        LOGE("JVM: AttachCurrentThread failed");
        return ;
    }
    jclass m_JClass=jniEnv->GetObjectClass(g_obj);

    if (m_JClass== nullptr){
        LOGD("JVM: FindClass error.....");
        if(g_jvm->DetachCurrentThread() != JNI_OK)
        {
            LOGE("JVM: DetachCurrentThread failed");
        }
        return ;
    }

    jmethodID m_method=jniEnv->GetMethodID(m_JClass,"OnVideoRenderCallback", "()V");
    if (m_methodID == NULL)
    {
        LOGE("JVM: GetMethodID error.....");
        //Detach主线程
        if(g_jvm->DetachCurrentThread() != JNI_OK)
        {
            LOGE("JVM: DetachCurrentThread failed");
        }
    }

    jniEnv->CallVoidMethod(g_obj,m_method);
    jniEnv->DeleteLocalRef(m_JClass);
}