#ifndef QVIDEOAPI_H
#define QVIDEOAPI_H

#include <QIODevice>
#include <QFile>
#include <QImage>

#include "QVideoDecoder.h"
#include "QVideoEncoder.h"

class QVideoAPI
{
    private :
        QVideoEncoder QVideoEnc;
        QVideoDecoder QVideoDec;

    public:
        QVideoAPI();
        int GetFrameRate();
        QVideoEncoder GetVideoEncoder();
        QVideoDecoder GetVideoDecoder();
        void SetVideoDec(QVideoDecoder videoDec);
        void SetVideoEnc(QVideoEncoder videoEnc);
};

#endif // QVIDEOAPI_H
