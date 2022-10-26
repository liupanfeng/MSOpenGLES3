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
 *  这个方法主要是拿到一些视频信息：1.音视频流的index  2.宽高的比率 3.音频采样率 采样位数 通道数量 4.音频视频时间基 5.视频的帧率
 *
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

            int width = m_pVideoCodecCtx->width;
            int height = m_pVideoCodecCtx->height;
        } else if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) { //音频数据
            m_audioStreamIdx = i;
            LOGD("Audio index: %d", m_audioStreamIdx);
            AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
            if (codec == nullptr) {
                LOGE("Audio AVCodec is NULL");
                return -1;
            }
            m_pAudioCodecCtx = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(m_pAudioCodecCtx,
                                          codecParameters); //将参数从codecParameters 拷贝到m_pAudioCodecCtx

            if (avcodec_open2(m_pAudioCodecCtx, codec, nullptr) < 0) {
                LOGE("Could not open codec.");
                return -1;
            }

            m_sampleRate = m_pAudioCodecCtx->sample_rate;  //获取音频的采样率
            LOGD("sample_rate is %d", m_sampleRate);
            m_channel = m_pAudioCodecCtx->channels;
            LOGD("channel number is %d", m_channel);

            switch (m_pAudioCodecCtx->sample_fmt) {
                case AV_SAMPLE_FMT_U8:
                    m_sampleSize = 8;
                    break;
                case AV_SAMPLE_FMT_S16:
                    m_sampleSize = 16;
                    break;
                case AV_SAMPLE_FMT_S32:
                    m_sampleSize = 32;
                    break;
            }
        }

    }

    MSAudioPlayer::GetInstance()->SetSampleRate(m_sampleRate);
    MSAudioPlayer::GetInstance()->SetSampleSize(m_sampleSize);
    MSAudioPlayer::GetInstance()->Setchannel(m_channel);

    if (m_pVideoCodecCtx != nullptr) {
        m_pYUVFrame = av_frame_alloc();
        if (m_pYUVFrame == nullptr) {
            LOGE("YUV frame alloc error.");
            return -1;
        }
        m_pYUV420Buffer = (uint8_t *) av_malloc(av_image_get_buffer_size
                                                        (AV_PIX_FMT_YUV420P,
                                                         m_pVideoCodecCtx->width,
                                                         m_pVideoCodecCtx->height, 1));

        if (m_pYUVFrame == NULL) {
            LOGE("out buffer alloc error.");
            return -1;
        }

        av_image_fill_arrays(m_pYUVFrame->data, m_pYUVFrame->linesize, m_pYUV420Buffer,
                             AV_PIX_FMT_YUV420P, m_pVideoCodecCtx->width, m_pVideoCodecCtx->height,
                             1);

        m_videoWidth = m_pVideoCodecCtx->width;
        m_videoHeight = m_pVideoCodecCtx->height;

        LOGD("Init VideoCodec sucess. Width: %d Height: %d", m_videoWidth, m_videoHeight);
    }

    if (m_videoStreamIdx != -1) {
        AVStream *videoStream = m_pFormatCtx->streams[m_videoStreamIdx];
        m_vStreamTimeRational = videoStream->time_base; //视频流时间基
        m_videoFPS = videoStream->avg_frame_rate.num / videoStream->avg_frame_rate.den;

        LOGD("videoStream->time_base.num: %d  videoStream->time_base.den :%d",
             videoStream->time_base.num, videoStream->time_base.den);
//        videoStream->time_base.num: 1  videoStream->time_base.den :90000  流的时间基 精度比较高  num 是分子  den分母
        LOGD("avg_frame_rate.num: %d  videoStream->avg_frame_rate.den :%d",
             videoStream->avg_frame_rate.num, videoStream->avg_frame_rate.den);
//        avg_frame_rate.num: 30  videoStream->avg_frame_rate.den :1
        LOGD("m_videoFPS: %d ", m_videoFPS);
    }

    if (m_audioStreamIdx != -1) {
        AVStream *audioStream = m_pFormatCtx->streams[m_audioStreamIdx];
        m_aStreamTimeRational = audioStream->time_base;
        LOGD("audioStream->time_base.num: %d  audioStream->time_base.den=  %d",
             audioStream->time_base.num, audioStream->time_base.den);
    }

    return 0;
}

/**
 * ffmpeg 相关指针的释放 必须调用ffmpeg的方法，因为释放的时候会释放内部的数据
 */
void MSAVCodecHandler::UnInitVideoCodec() {
    LOGD("UnInitVideoCodec...");
    if (m_pSwrBuffer != nullptr) {
        free(m_pSwrBuffer);
    }

    if (m_pAudioSwrCtx != nullptr) {
        swr_free(&m_pAudioSwrCtx);//必须调用这个来释放 可以释放内部的数据
    }

    if (m_pVideoSwsCtx != NULL) {
        sws_freeContext(m_pVideoSwsCtx);
    }

    if (m_pAudioFrame != nullptr) {
        av_frame_free(&m_pAudioFrame); //音频视频帧释放  必须用av_frame_free
    }

    if (m_pVideoFrame != nullptr) {
        av_frame_free(&m_pVideoFrame);
    }

    if (m_pYUV420Buffer != nullptr) {
        av_free(m_pYUV420Buffer);
    }

    if (m_pYUVFrame != nullptr) {
        av_frame_free(&m_pYUVFrame);
    }

    if (m_pVideoCodecCtx != NULL) {
        avcodec_close(m_pVideoCodecCtx);
    }


    if (m_pFormatCtx != NULL) {
        avformat_close_input(&m_pFormatCtx);
    }


}

void MSAVCodecHandler::StartPlayVideo() {
    MSAudioPlayer::GetInstance()->StartAudioPlayer();
    //开启线程
    startMediaProcessThreads();
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
    MSAudioPlayer::GetInstance()->StopAudioPlayer();
    while (!m_audioPktQueue.isEmpty()) {
        freePacket(m_audioPktQueue.dequeue());
    }
    while (!m_videoPktQueue.isEmpty()) {
        freePacket(m_videoPktQueue.dequeue());
    }

    waitAllThreadsExit();

    UnInitVideoCodec();

    resetAllMediaPlayerParameters();

}

void MSAVCodecHandler::SetMediaStatusPlay() {
    m_eMediaPlayStatus = MEDIAPLAY_STATUS_PLAYING;
}

void MSAVCodecHandler::SetMediaStatusPause() {
    m_eMediaPlayStatus = MEDIAPLAY_STATUS_PAUSE;
}

void MSAVCodecHandler::SeekMedia(float nPos) {
    if(nPos <0 ){
        return;
    }

    if(m_pFormatCtx == nullptr){
        return;
    }

    m_bThreadRunning = false;
    m_bReadFileEOF = false;

    if (m_mediaType==MEDIATYPE_VIDEO){
        m_nSeekingPos=(long long )nPos/ av_q2d(m_vStreamTimeRational);
        if (m_audioStreamIdx>0&&m_videoStreamIdx>0){
            av_seek_frame(m_pFormatCtx,m_videoStreamIdx,m_nSeekingPos,AVSEEK_FLAG_BACKWARD);
        }
    }

    if(m_mediaType == MEDIATYPE_MUSIC){
        LOGD("SEEK AUDIO");
        m_nSeekingPos=(long long )nPos/ av_q2d(m_aStreamTimeRational);
        if (m_audioStreamIdx>0){
            av_seek_frame(m_pFormatCtx,m_audioStreamIdx,m_nSeekingPos,AVSEEK_FLAG_ANY);
        }
    }

    //释放掉解码队列中的所有视频AVPacket
    while ( !m_videoPktQueue.isEmpty() )
    {
        freePacket( m_videoPktQueue.dequeue() );
    }

    while ( !m_audioPktQueue.isEmpty() )
    {
        freePacket( m_audioPktQueue.dequeue());
    }

    waitAllThreadsExit();

    startMediaProcessThreads();

}

float MSAVCodecHandler::GetMediaTotalSeconds() {
    float totalDuration=m_pFormatCtx->duration/(AV_TIME_BASE*1.000f);
    return totalDuration;
}

int MSAVCodecHandler::GetPlayerStatus() {
    return m_eMediaPlayStatus;
}

/**
 * 设置播放回调
 * @param callback
 * @param userData
 */
void MSAVCodecHandler::SetupUpdateVideoCallback(UpdateVideo2GUI_Callback callback,
                                                unsigned long userData) {
    m_updateVideoCallback = callback;
    m_userDataVideo=userData;
}

void MSAVCodecHandler::SetupUpdateCurrentPTSCallback(UpdateCurrentPTS_Callback callback,
                                                     unsigned long userData) {

}

void MSAVCodecHandler::convertAndRenderVideo(AVFrame *videoFrame, long long int ppts) {
    if (videoFrame == nullptr) {
        return;
    }

    if (m_pVideoSwsCtx == nullptr) {
        //获取sws上下文
        m_pVideoSwsCtx = sws_getContext(m_pVideoCodecCtx->width, m_pVideoCodecCtx->height,
                                        m_pVideoCodecCtx->pix_fmt,
                                        m_pVideoCodecCtx->width, m_pVideoCodecCtx->height,
                                        AV_PIX_FMT_YUV420P,
                                        SWS_BICUBIC, NULL, NULL, NULL);
    }

    sws_scale(m_pVideoSwsCtx, (const uint8_t *const *) videoFrame->data, videoFrame->linesize,
              0,
              m_pVideoCodecCtx->height,
              m_pYUVFrame->data,
              m_pYUVFrame->linesize
    );

    unsigned int lumaLength= m_pVideoCodecCtx->height * (MIN(videoFrame->linesize[0], m_pVideoCodecCtx->width));
    unsigned int chromBLength=((m_pVideoCodecCtx->height)/2)*(MIN(videoFrame->linesize[1], (m_pVideoCodecCtx->width)/2));
    unsigned int chromRLength=((m_pVideoCodecCtx->height)/2)*(MIN(videoFrame->linesize[2], (m_pVideoCodecCtx->width)/2));
    LOGD("  m_pVideoCodecCtx->height=%d   m_pVideoCodecCtx->width=%d "
         " videoFrame->linesize[0]=%d "
         "  videoFrame->linesize[1]=%d"
         " videoFrame->linesize[2]=%d",m_pVideoCodecCtx->height,
         m_pVideoCodecCtx->width,
         videoFrame->linesize[0],
         videoFrame->linesize[1],
         videoFrame->linesize[2]);

    MSYUVData_Frame *updateYUVFrame = new MSYUVData_Frame();

    updateYUVFrame->luma.length=lumaLength;
    updateYUVFrame->chromaB.length =chromBLength;
    updateYUVFrame->chromaR.length =chromRLength;

    updateYUVFrame->luma.dataBuffer=(unsigned char*)malloc(lumaLength);
    updateYUVFrame->chromaB.dataBuffer=(unsigned char*)malloc(chromBLength);
    updateYUVFrame->chromaR.dataBuffer=(unsigned char*)malloc(chromRLength);

    copyDecodedFrame420(m_pYUVFrame->data[0],updateYUVFrame->luma.dataBuffer,m_pYUVFrame->linesize[0],
                        m_pVideoCodecCtx->width,m_pVideoCodecCtx->height);
    copyDecodedFrame420(m_pYUVFrame->data[1], updateYUVFrame->chromaB.dataBuffer,m_pYUVFrame->linesize[1],
                        m_pVideoCodecCtx->width / 2,m_pVideoCodecCtx->height / 2);
    copyDecodedFrame420(m_pYUVFrame->data[2], updateYUVFrame->chromaR.dataBuffer,m_pYUVFrame->linesize[2],
                        m_pVideoCodecCtx->width / 2,m_pVideoCodecCtx->height / 2);

    updateYUVFrame->width=m_pVideoCodecCtx->width;
    updateYUVFrame->height=m_pVideoCodecCtx->height;

    updateYUVFrame->pts = ppts;

    if(m_updateVideoCallback)
    {
        m_updateVideoCallback(updateYUVFrame,m_userDataVideo);
    }

    if(updateYUVFrame->luma.dataBuffer){
        free(updateYUVFrame->luma.dataBuffer);
        updateYUVFrame->luma.dataBuffer=NULL;
    }

    if(updateYUVFrame->chromaB.dataBuffer){
        free(updateYUVFrame->chromaB.dataBuffer);
        updateYUVFrame->chromaB.dataBuffer=NULL;
    }

    if(updateYUVFrame->chromaR.dataBuffer){
        free(updateYUVFrame->chromaR.dataBuffer);
        updateYUVFrame->chromaR.dataBuffer=NULL;
    }

    if(updateYUVFrame){
        delete updateYUVFrame;
        updateYUVFrame = NULL;
    }


}

/**
 *
 * @param decodedFrame
 */
void MSAVCodecHandler::convertAndPlayAudio(AVFrame *decodedFrame) {
    if (!m_pFormatCtx || !m_pAudioFrame || decodedFrame == nullptr) {
        return;
    }

    if (m_pAudioSwrCtx == nullptr) {
        m_pAudioSwrCtx = swr_alloc();
        swr_alloc_set_opts(m_pAudioSwrCtx, av_get_default_channel_layout(m_channel),

                           AV_SAMPLE_FMT_S16,
                           m_sampleRate,
                           av_get_default_channel_layout(m_pAudioCodecCtx->channels),
                           m_pAudioCodecCtx->sample_fmt,
                           m_pAudioCodecCtx->sample_rate,
                           0, NULL
        );

        if (m_pAudioSwrCtx != nullptr) {
            int ret = swr_init(m_pAudioSwrCtx);
            LOGD("swr_init RetValue:", ret);
        }
    }

    int bufSize = av_samples_get_buffer_size(NULL, m_channel, decodedFrame->nb_samples,
                                             AV_SAMPLE_FMT_S16, 0);

    if (!m_pSwrBuffer || m_swrBuffSize < bufSize) {
        m_swrBuffSize = bufSize;
        m_pSwrBuffer = (uint8_t *) realloc(m_pSwrBuffer, m_swrBuffSize);
    }

    uint8_t *outbuf[2] = {m_pSwrBuffer, 0};
    //音频重采样
    int len = swr_convert(m_pAudioSwrCtx, outbuf, decodedFrame->nb_samples,
                          (const uint8_t **) decodedFrame->data, decodedFrame->nb_samples);
    if (len <= 0) {
        return;
    }

    if (!m_bThreadRunning) {
        return;
    }

    MSAudioPlayer::GetInstance()->WriteAudioData((char *) m_pSwrBuffer, bufSize);
}

void MSAVCodecHandler::copyDecodedFrame(uint8_t *src, uint8_t *dist, int linesize, int width,
                                        int height) {
    width = MIN(linesize, width);

    for (int i = 0; i < height; ++i) {
        memcpy(dist, src, width);
        dist += width;
        src += linesize;
    }
}
/**
 * 这个方法需要继续揣摩一下
 * @param src
 * @param dist
 * @param linesize
 * @param width
 * @param height
 */
void MSAVCodecHandler::copyDecodedFrame420(uint8_t *src, uint8_t *dist, int linesize, int width,
                                           int height) {
    width = MIN(linesize, width);
    for (int i = 0; i < height; ++i) {
        memcpy(dist, src, width);
        dist += width;
        src += linesize;
    }
}

void MSAVCodecHandler::decodeAndRenderVideo() {

}

void MSAVCodecHandler::tickVideoFrameTimerDelay(int64_t pts) {

}

void MSAVCodecHandler::tickAudioFrameTimerDelay(int64_t pts) {

}

void MSAVCodecHandler::doReadMediaFrameThread() {
    while (m_bThreadRunning) {
        m_bFileThreadRunning = true;
        if (m_eMediaPlayStatus == MEDIAPLAY_STATUS_PAUSE) {
            stdThreadSleep(10);
        }

        if (m_pVideoCodecCtx != nullptr && m_pAudioCodecCtx != nullptr) { //有视频,有音频
            if (m_videoPktQueue.size() > MAX_VIDEO_FRAME_IN_QUEUE
                && m_audioPktQueue.size() > MAX_AUDIO_FRAME_IN_QUEUE) {  //这样会导致音频或者视频会大于定义的最大值
                stdThreadSleep(10);
                continue;
            }
        } else if (m_pVideoCodecCtx != NULL && m_pAudioCodecCtx == NULL) { //只有视频,没有音频
            float sleepTime = 1000.0 / (float) m_videoFPS;
            stdThreadSleep(sleepTime);
        } else {  //只有音频
            if (m_audioPktQueue.size() > MAX_AUDIO_FRAME_IN_QUEUE) {
                stdThreadSleep(10);
                continue;
            }
        }

        if (m_bReadFileEOF == false) {
            readMediaPacket();
        } else {
            stdThreadSleep(10);
        }
        LOGD("read file thread exit...");
        m_bFileThreadRunning = false;
    }
    return;
}

/**
 * 音频数据解码  AVPacket->AVFrame
 */
void MSAVCodecHandler::doAudioDecodePlayThread() {
    if (m_pFormatCtx == nullptr) {
        return;
    }

    if (m_pAudioFrame == nullptr) {
        m_pAudioFrame = av_frame_alloc();
    }

    while (m_bThreadRunning) {
        m_bAudioThreadRunning = true;
        if (m_eMediaPlayStatus == MEDIAPLAY_STATUS_PAUSE) {
            stdThreadSleep(10);
            continue;
        }

        if (m_audioPktQueue.isEmpty()) {
            stdThreadSleep(1);
            continue;
        }

        AVPacket *packet = (AVPacket *) m_audioPktQueue.dequeue();
        if (packet == nullptr) {
            break;
        }
        if (!m_bThreadRunning) {
            freePacket(packet);
            break;
        }

        tickAudioFrameTimerDelay(packet->pts);  //这个是关键  音视频同步策略

        int retValue = avcodec_send_packet(m_pAudioCodecCtx, packet);
        if (retValue != 0) {
            freePacket(packet);
            continue;
        }

        int decodeRet = avcodec_receive_frame(m_pAudioCodecCtx, m_pAudioFrame);

        if (decodeRet == 0) {
            convertAndPlayAudio(m_pAudioFrame);  //解码出来的音频帧进行播放
        }
        freePacket(packet);

    }
    LOGD("audio decode show  thread exit...");
    m_bAudioThreadRunning = false;
    return;
}

void MSAVCodecHandler::doVideoDecodeShowThread() {
    if (m_pFormatCtx == nullptr) {
        return;
    }
    if (m_pVideoFrame == nullptr) {
        m_pVideoFrame == av_frame_alloc();
    }
    while (m_bThreadRunning) {
        m_bVideoThreadRunning = true;  //这个标记线程
        if (m_eMediaPlayStatus == MEDIAPLAY_STATUS_PAUSE) {
            stdThreadSleep(10);
            continue;
        }

        if (m_videoPktQueue.isEmpty()) {
            stdThreadSleep(1);
            continue;
        }

        AVPacket *packet = (AVPacket *) m_videoPktQueue.dequeue();

        if (packet == nullptr) {
            break;
        }
        if (!m_bThreadRunning) {
            freePacket(packet);
            break;
        }

        tickVideoFrameTimerDelay(packet->pts);

        int retValue = avcodec_send_packet(m_pVideoCodecCtx, packet);

        if (retValue != 0) {
            freePacket(packet);
            continue;
        }

        int decodeRet = avcodec_receive_frame(m_pVideoCodecCtx, m_pVideoFrame);
        if (decodeRet == 0) {
            convertAndRenderVideo(m_pVideoFrame, packet->pts);
        }

        freePacket(packet);  //解码 播放之后需要将packet 释放了
    }

    LOGD("video decode show  thread exit...");
    m_bVideoThreadRunning = false;

    return;
}

/**
 * 读视音频包
 */
void MSAVCodecHandler::readMediaPacket() {

    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket));
    if (!packet) {
        return;
    }

    av_init_packet(packet);
    m_eMediaPlayStatus = MEDIAPLAY_STATUS_PLAYING;
    int retValue = av_read_frame(m_pFormatCtx, packet);
    if (retValue == 0) {
        if (packet->stream_index == m_videoStreamIdx) {//Video frame
            //安全起见，如果用户希望自由地使用一个FFMPEG内部创建的AVPacket结构，
            // 最好调用av_dup_packet进行缓冲区的克隆，将其转化为缓冲区能够被释放的 AVPacket，
            // 以免对缓冲区的不当占用造成异常错误
            if (!av_dup_packet(packet)) {
                m_videoPktQueue.enqueue(packet);
            } else {
                freePacket(packet);
            }
        }
        if (packet->stream_index == m_audioStreamIdx) {//audio frame
            if (!av_dup_packet(packet)) {
                m_audioPktQueue.enqueue(packet);
            } else {
                freePacket(packet);
            }
        }

    } else if (retValue < 0) {
        if ((m_bReadFileEOF == false) && (retValue == AVERROR_EOF)) {
            m_bReadFileEOF = true;
        }
        return;
    }
}

void MSAVCodecHandler::freePacket(AVPacket *pkt) {
    if(pkt == nullptr ){
        return;
    }

    av_free_packet( pkt );

    free( pkt );
}

float MSAVCodecHandler::getAudioTimeStampFromPTS(long long int pts) {
    float aTimeStamp=pts* av_q2d(m_aStreamTimeRational);
    return aTimeStamp;
}

float MSAVCodecHandler::getVideoTimeStampFromPTS(long long int pts) {
    float vTimeStamp = pts* av_q2d(m_vStreamTimeRational);
    return vTimeStamp;
}

void MSAVCodecHandler::startMediaProcessThreads() {
    m_bThreadRunning = true; //运行状态

    std::thread readThread(&MSAVCodecHandler::doReadMediaFrameThread, this);
    readThread.detach();

    std::thread audioThread(&MSAVCodecHandler::doAudioDecodePlayThread, this);
    audioThread.detach();

    std::thread videoThread(&MSAVCodecHandler::doVideoDecodeShowThread, this);
    videoThread.detach();

}

void MSAVCodecHandler::waitAllThreadsExit() {
    while (m_bFileThreadRunning) {
        stdThreadSleep(10);
        continue;
    }

    while (m_bAudioThreadRunning) {
        stdThreadSleep(10);
        continue;
    }

    while (m_bVideoThreadRunning) {
        stdThreadSleep(10);
        continue;
    }
}

void MSAVCodecHandler::stdThreadSleep(int mseconds) {
    std::chrono::milliseconds sleepTime(mseconds);
    std::this_thread::sleep_for(sleepTime); //当前线程等待
}

void MSAVCodecHandler::resetAllMediaPlayerParameters() {
    m_pFormatCtx = nullptr;
    m_pVideoCodecCtx = nullptr;
    m_pAudioCodecCtx = nullptr;
    m_pYUVFrame = nullptr;
    m_pVideoFrame = nullptr;
    m_pAudioFrame = nullptr;
    m_pAudioSwrCtx = nullptr;
    m_pVideoSwsCtx = nullptr;
    m_pYUV420Buffer = nullptr;
    m_pSwrBuffer = nullptr;

    m_videoWidth = 0;
    m_videoHeight = 0;

    m_videoPathString = "";

    m_videoStreamIdx = -1;
    m_audioStreamIdx = -1;

    m_bReadFileEOF = false;

    m_nSeekingPos = 0;


    m_nCurrAudioTimeStamp = 0.0f;
    m_nLastAudioTimeStamp = 0.0f;

    m_sampleRate = 48000;
    m_sampleSize = 16;
    m_channel = 2;

    m_volumeRatio = 1.0f;
    m_swrBuffSize = 0;

    m_vStreamTimeRational = av_make_q(0, 0);
    m_aStreamTimeRational = av_make_q(0, 0);

    m_mediaType = MEDIATYPE_VIDEO;
    m_eMediaPlayStatus = MEDIAPLAY_STATUS_STOP;
}

/**
 * 解析后缀名字
 * @param path
 * @return
 */
std::string MSAVCodecHandler::getFileSuffix(const char *path) {

    const char* pos = strrchr(path,'.');
    if (pos){
        std::string str(pos+1);
        std::transform(str.begin(),str.end(),str.begin(),::tolower);
        return str;
    }
    return std::string();
}
