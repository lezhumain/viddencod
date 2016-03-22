#ifndef QVIDEODECODERTEST_H
#define QVIDEODECODERTEST_H
#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"

class QVideoEncoderTest : public QVideoEncoder
{
public:
    QVideoEncoderTest();
    ffmpeg::AVCodec* GetCodec(){return pCodec;}
    ffmpeg::AVCodecContext* GetCodecCTX(){return pCodecCtx;}
};

#endif // QVIDEODECODERTEST_H
