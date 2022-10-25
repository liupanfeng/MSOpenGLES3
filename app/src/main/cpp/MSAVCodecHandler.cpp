//
// Created by ms on 2022/10/24.
//

#include "MSAVCodecHandler.h"
#include "MSAudioPlayer.h"

#if !defined(MIN)
#define MIN(A, B)    ((A) < (B) ? (A) : (B))
#endif

std::atomic<bool> m_bFileThreadRunning(false);
std::atomic<bool> m_bAudioThreadRunning(false);
std::atomic<bool> m_bVideoThreadRunning(false);
std::atomic<bool> m_bThreadRunning(false);


MSAVCodecHandler::MSAVCodecHandler() {
    av_register_all();
    /*重置所有的播放参数*/
    resetAllMediaPlayerParameters();
}

MSAVCodecHandler::~MSAVCodecHandler() {

}

void MSAVCodecHandler::SetVideoFilePath(string &path) {
    m_videoPathString = path;
    std::string fileSuffix = getFileSuffix(m_videoPathString.c_str());
    LOGD("File suffix %s", fileSuffix.c_str());
    if (fileSuffix == "mp3") { //通过后缀名称来确定是音频还是视频
        m_mediaType = MEDIATYPE_MUSIC;
    } else {
        m_mediaType = MEDIATYPE_VIDEO;
    }


}

std::string MSAVCodecHandler::GetVideoFilePath() {
    return m_videoPathString;
}

int MSAVCodecHandler::GetVideoWidth() {
    return m_videoWidth;
}

int MSAVCodecHandler::GetVideoHeight() {
    return m_videoHeight;
}

/**
 *  1 有视频，有音频 m_pVideoCodecCtx != NULL m_pAudioCodecCtx != NULL 用音频同步视频
    2 有视频，没音频 m_pVideoCodecCtx != NULL m_pAudioCodecCtx == NULL 按照视频FPS自己播放
    3 有封面的mp3 同1 但只有一张封面图片，当作视频，相当于只有一帧视频.
    4 没有封面的mp3 盗版音乐 m_pVideoCodecCtx == NULL m_pAudioCodecCtx != NULL
 * @return
 */
int MSAVCodecHandler::InitVideoCodec() {
    if (m_videoPathString == "") {
        LOGE("m_videoPathString is null");
        return -1;
    }

    const char *filePath = m_videoPathString.c_str();

    if (avformat_open_input(&m_pFormatCtx, filePath, nullptr, nullptr)) {
        LOGE("open input stream failed .");
        return -1;
    }

    if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0) {
        LOGE("avformat_find_stream_info failed .");
        return -1;
    }

    av_dump_format(m_pFormatCtx, 0, filePath, 0);

    m_videoStreamIdx = -1;
    m_audioStreamIdx = -1;

    for (int i = 0; i < m_pFormatCtx->nb_streams; ++i) { //遍历format Context 中的nb_streams 这个里边保存的是流信息
        AVCodecParameters *codecParameters = m_pFormatCtx->streams[i]->codecpar;  //从流信息中拿到参数
        if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {  //判断参数类型 如果是视频类型
            m_videoStreamIdx = i;
            LOGD("video index: %d", m_videoStreamIdx);
            AVCodec *codec = avcodec_find_decoder(
                    codecParameters->codec_id);  //通过参数中的codec_id 来得到解码器
            if (codec == nullptr) {
                LOGE("Video AVCodec is NULL");
                return -1;
            }

            m_pVideoCodecCtx = avcodec_alloc_context3(codec);  //得到解码上下文  AVCodecContext *  得到解码器上下文
            avcodec_parameters_to_context(m_pVideoCodecCtx, codecParameters); //从参数中将数据拷贝到codec中

            if (avcodec_open2(m_pVideoCodecCtx, codec, nullptr) < 0) { //打开视频信息
                LOGE("Could not open codec.");
                return -1;
            }

        } else if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) { //音频数据
            m_audioStreamIdx = i;
            LOGD("Audio index: %d",m_audioStreamIdx);
            AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
            if (codec == nullptr) {
                LOGE("Audio AVCodec is NULL");
                return -1;
            }
            m_pAudioCodecCtx=avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(m_pAudioCodecCtx,codecParameters); //将参数从codecParameters 拷贝到m_pAudioCodecCtx

            if (avcodec_open2(m_pAudioCodecCtx,codec, nullptr)<0){
                LOGE("Could not open codec.");
                return -1;
            }

            m_sampleRate=m_pAudioCodecCtx->sample_rate;  //获取音频的采样率
            LOGE("sample_rate is %d",m_sampleRate);
            m_channel=m_pAudioCodecCtx->channels;
            LOGE("channel number is %d",m_channel);

            switch (m_pAudioCodecCtx->sample_fmt) {
                case AV_SAMPLE_FMT_U8:
                    m_sampleSize=8;
                    break;
                case AV_SAMPLE_FMT_S16:
                    m_sampleSize=16;
                    break;
                case AV_SAMPLE_FMT_S32:
                    m_sampleSize=32;
                    break;
            }
        }

    }

    MSAudioPlayer::GetInstance()->SetSampleRate(m_sampleRate);
    MSAudioPlayer::GetInstance()->SetSampleSize(m_sampleSize);
    MSAudioPlayer::GetInstance()->Setchannel(m_channel);

    if(m_pVideoCodecCtx != nullptr){
        m_pYUVFrame=av_frame_alloc();
        if (m_pYUVFrame == nullptr){
            LOGE("YUV frame alloc error.");
            return -1;
        }

    }

    return 0;
}

void MSAVCodecHandler::UnInitVideoCodec() {

}

void MSAVCodecHandler::StartPlayVideo() {

}

void MSAVCodecHandler::StopPlayVideo() {
    m_bThreadRunning = false;
    m_bReadFileEOF = false;

    m_nCurrAudioTimeStamp = 0.0f; //当前的音频时间戳
    m_nLastAudioTimeStamp = 0.0f; //最后的时间戳

    /*重置时间基*/
    m_vStreamTimeRational = av_make_q(0, 0);
    m_aStreamTimeRational = av_make_q(0, 0);

    m_eMediaPlayStatus = MEDIAPLAY_STATUS_STOP;

//    JCAudioPlayer::GetInstance()->StopAudioPlayer();

}

void MSAVCodecHandler::SetMediaStatusPlay() {

}

void MSAVCodecHandler::SetMediaStatusPause() {

}

void MSAVCodecHandler::SeekMedia(float nPos) {

}

float MSAVCodecHandler::GetMediaTotalSeconds() {
    return 0;
}

int MSAVCodecHandler::GetPlayerStatus() {
    return 0;
}

/**
 * 设置播放回调
 * @param callback
 * @param userData
 */
void MSAVCodecHandler::SetupUpdateVideoCallback(UpdateVideo2GUI_Callback callback,
                                                unsigned long userData) {

}

void MSAVCodecHandler::SetupUpdateCurrentPTSCallback(UpdateCurrentPTS_Callback callback,
                                                     unsigned long userData) {

}

void MSAVCodecHandler::convertAndRenderVideo(AVFrame *decodedFrame, long long int ppts) {

}

void MSAVCodecHandler::convertAndPlayAudio(AVFrame *decodedFrame) {

}

void MSAVCodecHandler::copyDecodedFrame(uint8_t *src, uint8_t *dist, int linesize, int width,
                                        int height) {

}

void MSAVCodecHandler::copyDecodedFrame420(uint8_t *src, uint8_t *dist, int linesize, int width,
                                           int height) {

}

void MSAVCodecHandler::decodeAndRenderVideo() {

}

void MSAVCodecHandler::tickVideoFrameTimerDelay(int64_t pts) {

}

void MSAVCodecHandler::tickAudioFrameTimerDelay(int64_t pts) {

}

void MSAVCodecHandler::doReadMediaFrameThread() {

}

void MSAVCodecHandler::doAudioDecodePlayThread() {

}

void MSAVCodecHandler::doVideoDecodeShowThread() {

}

void MSAVCodecHandler::readMediaPacket() {

}

void MSAVCodecHandler::freePacket(AVPacket *pkt) {

}

float MSAVCodecHandler::getAudioTimeStampFromPTS(long long int pts) {
    return 0;
}

float MSAVCodecHandler::getVideoTimeStampFromPTS(long long int pts) {
    return 0;
}

void MSAVCodecHandler::startMediaProcessThreads() {

}

void MSAVCodecHandler::waitAllThreadsExit() {

}

void MSAVCodecHandler::stdThreadSleep(int mseconds) {

}

void MSAVCodecHandler::resetAllMediaPlayerParameters() {

}

/**
 * 解析后缀名字
 * @param path
 * @return
 */
std::string MSAVCodecHandler::getFileSuffix(const char *path) {
    return std::string();
}
