#ifndef AVIDEODECODER_H
#define AVIDEODECODER_H

#include <QIODevice>
#include <QFile>
#include <QImage>

#include <ffmpeg.h>

// NOT USED
class AVideoDecoder
{
   public:
      // Public interface
      AVideoDecoder(){}
      AVideoDecoder(QString file){}
      virtual ~AVideoDecoder();

      virtual bool openFile(QString file) = 0;
      virtual void close() = 0;

      virtual bool getFrame(QImage&img,int *effectiveframenumber=0,int *effectiveframetime=0,int *desiredframenumber=0,int *desiredframetime=0) = 0;
      virtual bool seekNextFrame() = 0;
      virtual bool seekMs(int ts) = 0;
      virtual bool seekFrame(int64_t frame) = 0;
      virtual int getVideoLengthMs() = 0;


      virtual bool isOk() = 0;
};

#endif // AVIDEODECODER_H
