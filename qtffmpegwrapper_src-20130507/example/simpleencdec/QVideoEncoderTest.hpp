#ifndef QVIDEOENCODERTEST_HPP
#define QVIDEOENCODERTEST_HPP

#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"

class QVideoEncoderTest : public QVideoEncoder
{
public:
    QVideoEncoderTest();
    ffmpeg::AVCodec* GetCodec(){return pCodecVideoEncoder;}
    ffmpeg::AVCodecContext* GetCodecCTX(){return pCodecCtxVideoEncoder;}
};

#endif // QVIDEOENCODERTEST_HPP
