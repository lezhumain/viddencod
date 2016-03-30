#ifndef QVIDEOENCODERTEST_HPP
#define QVIDEOENCODERTEST_HPP

#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"

class QVideoEncoderTest : public QVideoEncoder
{
public:
    QVideoEncoderTest();
    ffmpeg::AVCodec* GetCodec(){return pCodec;}
    ffmpeg::AVCodecContext* GetCodecCTX(){return pCodecCtx;}
};

#endif // QVIDEOENCODERTEST_HPP
