#ifndef QVIDEODECODERTEST_HPP
#define QVIDEODECODERTEST_HPP

#include "../../QTFFmpegWrapper/headers/QVideoDecoder.h"

class QVideoDecoderTest : public QVideoDecoder
{
public:
    QVideoDecoderTest();
    ffmpeg::AVCodec* GetCodec(){return pCodec;}
    ffmpeg::AVCodecContext* GetCodecCTX(){return pCodecCtx;}
};

#endif // QVIDEODECODERTEST_HPP
