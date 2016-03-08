#include "aactranscoder.hpp"
#include "ffmpeg.h"


#include <stdio.h>
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"

AACTranscoder::AACTranscoder() :
    input_format_context(NULL),
    output_format_context(NULL),
    input_codec_context(NULL),
    output_codec_context(NULL),
    resample_context(NULL),
    fifo(NULL),
    ret(AVERROR_EXIT)
{
}
