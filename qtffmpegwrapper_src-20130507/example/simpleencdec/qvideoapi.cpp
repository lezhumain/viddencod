#include "qvideoapi.h"
#include "ffmpeg.h"

QVideoAPI::QVideoAPI()
{
    QVideoDec = new QVideoDecoder;
    QVideoEnc = new QVideoEncoder;
}

QVideoAPI::~QVideoAPI()
{
    delete(QVideoDec);
    delete(QVideoEnc);
}

int QVideoAPI::GetFrameRate()
{
  ffmpeg::AVFormatContext *tmp =  QVideoDec->getAVFormatContext();
  ffmpeg::AVStream *st = av_new_stream(tmp,0);
  ffmpeg::AVRational fr = st->r_frame_rate;
  printf("\t\tframerate = %d\n", fr);
    return 1;
}

QVideoEncoder* QVideoAPI::GetVideoEncoder()
{
    return QVideoEnc;
}

QVideoDecoder* QVideoAPI::GetVideoDecoder()
{
    return QVideoDec;
}

void QVideoAPI::SetVideoDec(QVideoDecoder* videoDec)
{
    QVideoDec = videoDec;
}

void QVideoAPI::SetVideoEnc(QVideoEncoder* videoEnc)
{
    QVideoEnc = videoEnc;
}
