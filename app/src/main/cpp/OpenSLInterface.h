//
// Created by ms on 2022/10/24.
//

#ifndef MSOPENGLES3_OPENSLINTERFACE_H
#define MSOPENGLES3_OPENSLINTERFACE_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct threadLock_ {
    pthread_mutex_t m;
    pthread_cond_t  c;
    unsigned char   s;
} threadLock;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct opensl_stream{

    // engine interfaces  引擎接口
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    //output mix interfaces 输出混合接口
    SLObjectItf outputMixObject;

    // buffer queue player interface  缓冲队列播放器界面
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    SLEffectSendItf bqPlayerEffectSend;


    // recorder interfaces  记录仪接口
    SLObjectItf recorderObject;
    SLRecordItf recorderRecord;
    SLAndroidSimpleBufferQueueItf recorderBufferQueue;

    // buffer indexes  缓冲区索引
    int currentInputIndex;
    int currentOutputIndex;


    // current buffer half (0, 1)  //当前缓冲区一半
    int currentOutputBuffer;
    int currentInputBuffer;

    // buffers
    short *outputBuffer[2];
    short *inputBuffer[2];

    // size of buffers
    int outBufSamples;
    int inBufSamples;

    // locks
    void*  inlock;
    void*  outlock;

    double time;
    int inchannels;
    int outchannels;
    int sr;
} OPENSL_STREAM;

OPENSL_STREAM* android_OpenAudioDevice(int sr, int inchannels, int outchannels, int bufferframes);

void android_CloseAudioDevice(OPENSL_STREAM *p);

int android_AudioIn(OPENSL_STREAM *p, short *buffer,int size);

int android_AudioOut(OPENSL_STREAM *p, short *buffer,int size);

double android_GetTimestamp(OPENSL_STREAM *p);

#ifdef __cplusplus
};
#endif



#endif //MSOPENGLES3_OPENSLINTERFACE_H
