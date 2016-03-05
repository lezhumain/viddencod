#ifndef QAUDIODECODER_H
#define QAUDIODECODER_H

#include <QIODevice>
#include <QFile>
#include <QImage>

#include "ffmpeg.h"

class QAudioDecoder
{
protected:
    // Basic FFmpeg stuff
    ffmpeg::AVFormatContext *pFormatCtx;
    int audioStream;
    ffmpeg::AVCodecContext  *pCodecCtx;
    ffmpeg::AVCodec         *pCodec;
    ffmpeg::AVFrame         *pFrame;
    int16_t                  *pAudioFrame;
    ffmpeg::AVFrame         *pFrameRGB;
    ffmpeg::AVPacket        packet;
    ffmpeg::SwsContext      *img_convert_ctx;
    uint8_t                 *buffer;
    int                     numBytes;

    // State infos for the wrapper
    bool ok;
    QImage LastFrame;
    int16_t LastAudioFrame;
    int LastFrameTime,LastLastFrameTime,LastLastFrameNumber,LastFrameNumber;
    int DesiredFrameTime,DesiredFrameNumber;
    bool LastFrameOk;                // Set upon start or after a seek we don't have a frame yet

    // Initialization functions
    virtual bool initCodec();
    virtual void InitVars();
    QList<int16_t> getAllAudioFrames();

//    // Helpers
    virtual void dumpFormat(ffmpeg::AVFormatContext *ic,int index,const char *url,int is_output);
//    virtual void saveFramePPM(ffmpeg::AVFrame *pFrame, int width, int height, int iFrame);

//    // Seek
    virtual bool decodeSeekFrame(int after);

public:
    // Public interface
    QAudioDecoder();
//    QAudioDecoder(QString file);
//    virtual ~QAudioDecoder();

    virtual bool openFile(QString file);
    virtual void close();
//    virtual void flush();

    virtual bool getAudioFrame(int16_t &audioSample, int *effectiveframenumber=0, int *effectiveframetime=0, int *desiredframenumber=0, int *desiredframetime=0);
    virtual bool seekNextAudioFrame();
//    virtual bool seekMs(int ts);
//    virtual bool seekFrame(int64_t frame);
    bool isOk();
    virtual int getVideoLengthMs();


//    virtual bool isOk();
};

#endif // QAUDIODECODER_H
