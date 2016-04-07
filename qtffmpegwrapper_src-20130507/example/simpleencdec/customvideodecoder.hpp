#ifndef CUSTOMVIDEODECODER_H
#define CUSTOMVIDEODECODER_H

//#include "ffmpeg.h" // NOT WORKING

#include <math.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
//#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
class QFile;

class CustomVideoDecoder
{
public:
    CustomVideoDecoder();
    ~CustomVideoDecoder();
    Main();

protected:
    static const short INBUF_SIZE = 4096,
                    AUDIO_INBUF_SIZE = 20480,
                    AUDIO_REFILL_THRESH = 4096;

    AVCodec *codec;
    AVCodecContext *c;
    AVFrame *frame;
    AVPacket avpkt;
    QFile *f;
    int frame_count;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];

};

#endif // CUSTOMVIDEODECODER_H
