#ifndef AACTRANSCODER_H
#define AACTRANSCODER_H

class AVFormatContext;
class AVCodecContext;
class SwrContext;
class AVAudioFifo;

class AACTranscoder
{
public:
    AACTranscoder();

private:
    AVFormatContext *input_format_context, *output_format_context;
    AVCodecContext *input_codec_context, *output_codec_context;
    SwrContext *resample_context;
    AVAudioFifo *fifo;
    int ret;
};

#endif // AACTRANSCODER_H
