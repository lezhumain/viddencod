#ifndef QVIDEOENCODERTEST_HPP
#define QVIDEOENCODERTEST_HPP

#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"

class QVideoEncoderTest : public QVideoEncoder
{
public:
    QVideoEncoderTest();
    ffmpeg::AVCodec* GetCodec(){return pCodecVideo;}
    ffmpeg::AVCodecContext* GetCodecCTX(){return pCodecCtxVideo;}
};

#endif // QVIDEOENCODERTEST_HPP
