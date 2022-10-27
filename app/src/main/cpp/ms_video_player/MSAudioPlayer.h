//
// Created by ms on 2022/10/24.
//

#ifndef MSOPENGLES3_MSAUDIOPLAYER_H
#define MSOPENGLES3_MSAUDIOPLAYER_H

#include <mutex>
#include "../ms_video_player/OpenSLInterface.h"

#define OUT_BUFFER_SIZE 8192

class MSAudioPlayer {
public:

    /*单例方法*/
    static MSAudioPlayer* GetInstance();
    /**
     * 开始音频播放
     */
    void StartAudioPlayer();
    /**
     * 结束音频播放
     */
    void StopAudioPlayer();

    /**
     * 得到自由空间
     * @return
     */
    int GetFreeSpace();

    /**
     * 写音频数据
     * @param dataBuff
     * @param size
     * @return
     */
    bool WriteAudioData(const char* dataBuff,int size);

    /**
     *设置音频采样率
     * @param value
     */
    void SetSampleRate(int value){m_sampleRate = value;}
    /**
     * 设置音频采样字节位数
     * @param value
     */
    void SetSampleSize(int value){m_sampleSize = value;}
    /**
     * 设置通道数量
     * @param value
     */
    void Setchannel(int value){m_channel = value;}

private:
    /**
     * 单例 构造方法私有化
     */
    MSAudioPlayer();

    int m_sampleRate = 44100;
    int m_sampleSize = 16;
    int m_channel = 2;

    static MSAudioPlayer*  m_pInstance;
    /**
     * 声明互斥锁
     */
    static pthread_mutex_t m_mutex;

    OPENSL_STREAM*  m_stream;

    class Garbage
    {
    public:
        ~Garbage(){
            if(MSAudioPlayer::m_pInstance){
                delete MSAudioPlayer::m_pInstance;
                MSAudioPlayer::m_pInstance = NULL;
            }
        }
    };
    static Garbage  m_garbage;  //管理单例对象
};


#endif //MSOPENGLES3_MSAUDIOPLAYER_H
