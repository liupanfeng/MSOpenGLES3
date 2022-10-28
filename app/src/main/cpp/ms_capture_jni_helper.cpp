#include <jni.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/check.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <trackerexport.h>


#include "android_log_util.h"
#include "MSYUVDataDefine.h"
#include "MSGLVideoRender.h"

MSGLVideoRender *msGlVideoRender;


extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_capture_MSOpenCVRender_nativeInitGL(JNIEnv *env, jobject thiz,
                                                                jobject asset_manager,jstring file_path) {
    AAssetManager *aAssetManager= AAssetManager_fromJava(env,asset_manager);
    msGlVideoRender = new MSGLVideoRender();
    if (aAssetManager!= nullptr){
        const char* dirPath=(char*)env->GetStringUTFChars(file_path,JNI_OK);
        std::string dirPathString=std::string (dirPath);
        msGlVideoRender->SetupAssetManager(aAssetManager,dirPathString);

        std::string modelPath = dirPathString + "/roboman-landmark-model.bin";
        std::string classiPath = dirPathString + "/haar_roboman_ff_alt2.xml";

        bool retValue=FACETRACKER_API_init_facetracker_resources(modelPath.c_str(),classiPath.c_str());
        if(!retValue){
            LOGE("Init facetracker failed.....");
        }else{
            LOGD("Init facetracker success!!!");
        }
        env->ReleaseStringUTFChars(file_path,dirPath);
    }

    msGlVideoRender->InitGL();

}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_capture_MSOpenCVRender_nativeDrawGL(JNIEnv *env, jobject thiz) {
    msGlVideoRender->PaintGL();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_capture_MSOpenCVRender_nativeResize(JNIEnv *env, jobject thiz,
                                                                jint width, jint height) {
    msGlVideoRender->ResizeGL(width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_meishe_msopengles3_capture_MSOpenCVRender_nativeUpdateCameraFrame(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jbyteArray yuv_frame,
                                                                           jint width,
                                                                           jint height) {
//    LOGD("YUV FRAME: %d %d",width,height);
    jbyte * yuvBuffer=(jbyte *)env->GetByteArrayElements(yuv_frame,JNI_OK);
    cv::Mat image(height+height/2,width,CV_8UC1,(unsigned char *)yuvBuffer);

    cv::Mat bgrCVFrame;
    cv::Mat yuvCVFrame;
    cv::cvtColor(image,bgrCVFrame,cv::COLOR_YUV2BGR_NV21);  //nv21 yuv 转成 bgr格式
    cv::rotate(bgrCVFrame,bgrCVFrame,cv::ROTATE_90_COUNTERCLOCKWISE);
    cv::flip(bgrCVFrame,bgrCVFrame,1);  //镜像旋转

    //bgrCVFrame 可以在opencv里面去做很多事情.
    FACETRACKER_API_facetracker_obj_track(bgrCVFrame);
    ofVec2f posVec2f=FACETRACKER_API_getPosition(bgrCVFrame);

    glm::vec2 posValue = glm::vec2(posVec2f.x,posVec2f.y);
    float   scaleValue=FACETRACKER_API_getScale(bgrCVFrame);
    LOGD("TRACKER: %f %f",posVec2f.x,posVec2f.y);

    cv::cvtColor(bgrCVFrame,yuvCVFrame,cv::COLOR_BGR2YUV_I420);  //将数据从BGR格式转成I420

    //opengles渲染YUV
    int lumaSize = width* height;
    int uv_stride = width / 2;
    int chromaSize = uv_stride *(height / 2);

    uint8_t *Y_data_Dst = yuvCVFrame.data;
    uint8_t *U_data_Dst = yuvCVFrame.data + lumaSize;   //  width* height
    uint8_t *V_data_Dst = yuvCVFrame.data + lumaSize + chromaSize;   // width* height+width* height/4

    //yuvCVFrame  -  MSYUVData_Frame  数据转换

    MSYUVData_Frame  yuvFrame;
    memset(&yuvFrame,0,sizeof (yuvFrame));  //yuvFrame初始化  将数据全部初始化为0

    yuvFrame.luma.dataBuffer = Y_data_Dst;
    yuvFrame.chromaB.dataBuffer = U_data_Dst;
    yuvFrame.chromaR.dataBuffer = V_data_Dst;

    yuvFrame.luma.length = lumaSize;
    yuvFrame.chromaB.length = chromaSize;
    yuvFrame.chromaR.length = chromaSize;


    yuvFrame.width =   height;
    yuvFrame.height =  width;

    msGlVideoRender->RendVideo(&yuvFrame);

    if(thiz != nullptr){
        jclass renderClass=env->GetObjectClass(thiz);
        jmethodID renderMethodID=env->GetMethodID(renderClass,"UpdateRequestGLRender","()V");
        env->CallVoidMethod(thiz,renderMethodID);
        env->DeleteLocalRef(renderClass);
    }

    env->ReleaseByteArrayElements(yuv_frame,yuvBuffer,JNI_OK);
}