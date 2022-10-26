//
// Created by ms on 2022/10/24.
//

#include "MSAudioPlayer.h"
#include "android_log_util.h"

MSAudioPlayer::MSAudioPlayer() {
    m_stream = NULL;
}

MSAudioPlayer *MSAudioPlayer::GetInstance() {
    if (m_pInstance == nullptr) {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_mutex_lock(&m_mutex);
        m_pInstance = new MSAudioPlayer();
        pthread_mutex_unlock(&m_mutex);
    }
    return m_pInstance;
}

void MSAudioPlayer::StartAudioPlayer() {
    pthread_mutex_lock(&m_mutex);
    StopAudioPlayer();
    /*打开android音频设备*/
    m_stream = android_OpenAudioDevice(m_sampleRate, m_channel, m_channel, OUT_BUFFER_SIZE);

    pthread_mutex_unlock(&m_mutex);

}

void MSAudioPlayer::StopAudioPlayer() {
    if (m_stream != nullptr) {
        pthread_mutex_lock(&m_mutex);
        android_CloseAudioDevice(m_stream);
        m_stream = nullptr;
        pthread_mutex_unlock(&m_mutex);
    }
}

int MSAudioPlayer::GetFreeSpace() {
    pthread_mutex_lock(&m_mutex);
    int retValue = m_stream->outBufSamples;
    pthread_mutex_unlock(&m_mutex);
    return retValue;
}

bool MSAudioPlayer::WriteAudioData(const char *dataBuff, int size) {
    if((dataBuff == NULL) || (size <= 0)){
        return false;
    }

    pthread_mutex_lock(&m_mutex);
    int writeSamples=android_AudioOut(m_stream,(short*)dataBuff,size/2);  //这里为啥size/2  有什么说法吗？
    pthread_mutex_unlock(&m_mutex);
    LOGD("WRITE SAMPLES:%d %d",size,writeSamples);
    return true;
}
