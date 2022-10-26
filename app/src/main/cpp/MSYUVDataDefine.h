//
// Created by ms on 2022/10/24.
//

#ifndef MSOPENGLES3_MSYUVDATADEFINE_H
#define MSOPENGLES3_MSYUVDATADEFINE_H

#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)   //一个字节对齐

#define MAX_AUDIO_FRAME_IN_QUEUE    1200
#define MAX_VIDEO_FRAME_IN_QUEUE    600


typedef struct YUVChannelDef
{
    unsigned int    length;
    unsigned char*  dataBuffer;

}MSYUVChannel;


typedef struct  YUVFrameDef
{
    unsigned int    width;
    unsigned int    height;
    MSYUVChannel      luma;
    MSYUVChannel      chromaB;
    MSYUVChannel      chromaR;
    long long       pts;

}MSYUVData_Frame;

typedef struct DecodedAudiodataDef
{
    unsigned char*  dataBuff;
    unsigned int    dataLength;
}MSDecodedAudioData;

#pragma pack(pop)

#endif //MSOPENGLES3_MSYUVDATADEFINE_H
