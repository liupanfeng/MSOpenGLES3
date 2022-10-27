
#ifndef MSOPENGLES3_MSAVCODECHANDLER_H
#define MSOPENGLES3_MSAVCODECHANDLER_H

extern "C" {
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
}

#include <mutex>
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include "../MSYUVDataDefine.h"
#include "../MSQueueDef.h"
#include "../android_log_util.h"

#ifndef int64
#define int64 long long
#endif


enum MSMediaType{
    MEDIATYPE_VIDEO = 0,
    MEDIATYPE_MUSIC
};


enum MediaPlayStatus
{
    MEDIAPLAY_STATUS_PLAYING = 0,
    MEDIAPLAY_STATUS_PAUSE,
    MEDIAPLAY_STATUS_SEEK,
    MEDIAPLAY_STATUS_STOP
};

/*定义函数指针*/
typedef void  (*UpdateVideo2GUI_Callback)    (MSYUVData_Frame* yuv,unsigned long userData);
typedef void  (*UpdateCurrentPTS_Callback)   (float pts,unsigned long userData);
typedef void  (*NotifyPrepare) (float onPrepare);
typedef void (*NotifyPlayState) (int state);


class MSAVCodecHandler {
public:
    explicit MSAVCodecHandler();
    virtual ~MSAVCodecHandler();

    void         SetVideoFilePath(std::string& path);
    std::string  GetVideoFilePath();

    int      GetVideoWidth();
    int      GetVideoHeight();

    int      InitVideoCodec();
    void     UnInitVideoCodec();

    void     StartPlayVideo();
    void     StopPlayVideo();

    void     SetMediaStatusPlay();
    void     SetMediaStatusPause();

    void     SeekMedia(float nPos);// mediaType 0 视频 1音乐

    float    GetMediaTotalSeconds();

    int      GetPlayerStatus();

    void     SetupUpdateVideoCallback(UpdateVideo2GUI_Callback callback,unsigned long userData);
    void     SetupUpdateCurrentPTSCallback(UpdateCurrentPTS_Callback callback,unsigned long userData);

    void     notifyPrepare(NotifyPrepare notifyPrepare );
    void     notifyPlayState(NotifyPlayState notifyPlayState );

private:

    void     convertAndRenderVideo(AVFrame* decodedFrame,long long ppts);
    void     convertAndPlayAudio(AVFrame* decodedFrame);
    void     copyDecodedFrame(uint8_t* src, uint8_t* dist,int linesize, int width, int height);
    void     copyDecodedFrame420(uint8_t* src, uint8_t* dist,int linesize, int width, int height);

    void     decodeAndRenderVideo();

    void     tickVideoFrameTimerDelay(int64_t pts);
    void     tickAudioFrameTimerDelay(int64_t pts);

    void     doReadMediaFrameThread();
    void     doAudioDecodePlayThread();
    void     doVideoDecodeShowThread();


    void     readMediaPacket();
    void     freePacket(AVPacket* pkt);

    float    getAudioTimeStampFromPTS(int64 pts);
    float    getVideoTimeStampFromPTS(int64 pts);

    void     startMediaProcessThreads();
    void     waitAllThreadsExit();

    void     stdThreadSleep(int mseconds);

    void     resetAllMediaPlayerParameters();

    std::string getFileSuffix(const char* path);

private:


    int                 m_videoWidth    = 0;
    int                 m_videoHeight   = 0;

    std::string         m_videoPathString = "";

    int                 m_videoStreamIdx    = -1;
    int                 m_audioStreamIdx    = -1;

    bool                m_bReadFileEOF = false;

    int64               m_nSeekingPos =0;

    bool                m_bVideoSeekingOk =false;



    unsigned long               m_userDataVideo=0;
    UpdateVideo2GUI_Callback    m_updateVideoCallback =nullptr;
    NotifyPrepare               m_NotifyPrepare=nullptr;
    NotifyPlayState             m_NotifyPlayState= nullptr;

    unsigned long               m_userDataPts=0;
    UpdateCurrentPTS_Callback   m_updateCurrentPTSCallback =nullptr;

    MediaPlayStatus             m_eMediaPlayStatus;


    AVFormatContext*    m_pFormatCtx    = nullptr;

    AVCodecContext*     m_pVideoCodecCtx     = nullptr;
    AVCodecContext*     m_pAudioCodecCtx     = nullptr;

    AVFrame*             m_pYUVFrame    = nullptr;

    AVFrame*             m_pVideoFrame  = nullptr;
    AVFrame*             m_pAudioFrame  = nullptr;

    SwrContext*          m_pAudioSwrCtx = nullptr;
    SwsContext*          m_pVideoSwsCtx = nullptr;

    int                  m_videoFPS = 0;
    uint8_t*             m_pYUV420Buffer   = nullptr;

    float                m_nCurrAudioTimeStamp = 0.0f;
    float                m_nLastAudioTimeStamp = 0.0f;

    AVRational           m_vStreamTimeRational;
    AVRational           m_aStreamTimeRational;


    int                  m_sampleRate = 48000;
    int                  m_sampleSize = 16;
    int                  m_channel = 2;
    float                m_volumeRatio = 1.00;

    uint8_t*             m_pSwrBuffer = nullptr;
    int                  m_swrBuffSize = 0;

    MSMediaQueue<AVPacket *> m_audioPktQueue;
    MSMediaQueue<AVPacket *> m_videoPktQueue;

    MSMediaType         m_mediaType = MEDIATYPE_VIDEO;
};


#endif //MSOPENGLES3_MSAVCODECHANDLER_H
