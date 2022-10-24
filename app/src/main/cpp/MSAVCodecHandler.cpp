//
// Created by ms on 2022/10/24.
//

#include "MSAVCodecHandler.h"

MSAVCodecHandler::MSAVCodecHandler() {

}

MSAVCodecHandler::~MSAVCodecHandler() {

}

void MSAVCodecHandler::SetVideoFilePath(string &path) {

}

std::string MSAVCodecHandler::GetVideoFilePath() {
    return std::string();
}

int MSAVCodecHandler::GetVideoWidth() {
    return 0;
}

int MSAVCodecHandler::GetVideoHeight() {
    return 0;
}

int MSAVCodecHandler::InitVideoCodec() {
    return 0;
}

void MSAVCodecHandler::UnInitVideoCodec() {

}

void MSAVCodecHandler::StartPlayVideo() {

}

void MSAVCodecHandler::StopPlayVideo() {

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

std::string MSAVCodecHandler::getFileSuffix(const char *path) {
    return std::string();
}
