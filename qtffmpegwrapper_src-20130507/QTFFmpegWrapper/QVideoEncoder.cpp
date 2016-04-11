/*
   QTFFmpegWrapper - QT FFmpeg Wrapper Class
   Copyright (C) 2009-2012:
         Daniel Roggen, droggen@gmail.com

   All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QPainter>
#include "headers/QVideoEncoder.h"
#include <ffmpeg.h>
#include <QDebug>
#include <QTimer>
#include <QTime>


#include <QDebug>
#include <QTime>

/******************************************************************************
*******************************************************************************
* QVideoEncoder   QVideoEncoder   QVideoEncoder   QVideoEncoder   QVideoEncoder
*******************************************************************************
******************************************************************************/


/******************************************************************************
* PUBLIC   PUBLIC   PUBLIC   PUBLIC   PUBLIC   PUBLIC   PUBLIC   PUBLIC   PUBLIC
******************************************************************************/

QVideoEncoder::QVideoEncoder()
{
   initVars();
   initCodec();
}

QVideoEncoder::~QVideoEncoder()
{
   close();
}

//ffmpeg::AVFormatContext* QVideoEncoder::GetContext(void)
//{
//    return pFormatCtxVideo;
//}

void QVideoEncoder::SetFramerate()
{
    //  Recopie du framerate dans le paramètre du framerate de l'output
    pFormatCtxVideo->streams[0]->time_base = Frame_Rate;
}

void QVideoEncoder::GetFramerate(ffmpeg::AVRational *FramRat)
{
    FramRat->num = Frame_Rate.num;
    FramRat->den = Frame_Rate.den;
}

void QVideoEncoder::SaveTmpFrameRate(ffmpeg::AVRational *FramRat)
{
    Frame_Rate.num = FramRat->num;
    Frame_Rate.den = FramRat->den;
}

bool QVideoEncoder::createFile(QString fileName,unsigned width,unsigned height,unsigned bitrate,unsigned gop,unsigned fps)
{
   // If we had an open video, close it.
   close();

   Width    = width;
   Height   = height;
   Gop      = gop;
   Bitrate  = bitrate;

   if(!isSizeValid())
   {
      printf("Invalid size\n");
      return false;
   }

   pOutputFormatVideo = ffmpeg::av_guess_format(NULL, fileName.toStdString().c_str(), NULL);
   if (!pOutputFormatVideo)
   {
      printf("Could not deduce output format from file extension: using MPEG.\n");
      pOutputFormatVideo = ffmpeg::av_guess_format("mpeg", NULL, NULL);
   }

   pFormatCtxVideo = ffmpeg::avformat_alloc_context();
   if(!pFormatCtxVideo)
   {
      printf("Error allocating format context\n");
      return false;
   }
   pFormatCtxVideo->oformat = pOutputFormatVideo;
   snprintf(pFormatCtxVideo->filename, sizeof(pFormatCtxVideo->filename), "%s", fileName.toStdString().c_str());

   // Add the video stream
   pStream = avformat_new_stream(pFormatCtxVideo,0);
   if(!pStream )
   {
      printf("Could not allocate stream\n");
      return false;
   }
   printf("2. Frame_Rate.num = %d", Frame_Rate.num);
   printf("2. Frame_Rate.den = %d", Frame_Rate.den);

   SetFramerate();

   pCodecCtxVideo                        = pStream->codec;
   pCodecCtxVideo->codec_id              = pOutputFormatVideo->video_codec;
   pCodecCtxVideo->codec_type            = ffmpeg::AVMEDIA_TYPE_VIDEO;
   pCodecCtxVideo->bit_rate              = 30/*GetFramerate()*/;
   pCodecCtxVideo->width                 = getWidth();
   pCodecCtxVideo->height                = getHeight();

//   pCodecCtxVideo->time_base.den         = fps;
//   pCodecCtxVideo->time_base.num         = 1;
   pCodecCtxVideo->gop_size              = Gop;
   pCodecCtxVideo->pix_fmt               = ffmpeg::AV_PIX_FMT_YUV420P;
   pCodecCtxVideo->thread_count          = 1;
   pCodecCtxVideo->bit_rate              = 64000;
   pCodecCtxVideo->sample_fmt            = ffmpeg::AV_SAMPLE_FMT_S16;
   pCodecCtxVideo->sample_rate           = 44100;
   pCodecCtxVideo->channels              = 2;
   // some formats want stream headers to be separate
   if(pFormatCtxVideo->oformat->flags & AVFMT_GLOBALHEADER)
      pCodecCtxVideo->flags |= CODEC_FLAG_GLOBAL_HEADER;

   ffmpeg::av_dump_format(pFormatCtxVideo, 0, fileName.toStdString().c_str(), 1);

   if (pCodecCtxVideo->codec_id == ffmpeg::AV_CODEC_ID_H264)
       ffmpeg::av_opt_set(pCodecCtxVideo->priv_data, "preset", "veryslow", 0);

   // open_video

   // find the video encoder
   pCodecVideo = avcodec_find_encoder(pCodecCtxVideo->codec_id);
   if (!pCodecVideo)
   {
      printf("codec not found\n");
      return false;
   }

   // open the codec
   int iret = avcodec_open2(pCodecCtxVideo, pCodecVideo, NULL);
   if ( iret < 0)
   {
      printf("could not open codec\n");
      return false;
   }

   // Allocate memory for output
   if(!initOutputBuf())
   {
      printf("Can't allocate memory for output bitstream\n");
      return false;
   }

   // Allocate the YUV frame
   if(!initFrame())
   {
      printf("Can't init frame\n");
      return false;
   }

#define URL_WRONLY 1
   int iRet = avio_open(&pFormatCtxVideo->pb,
                        fileName.toStdString().c_str(),
                        AVIO_FLAG_WRITE);
   if(iRet < 0)
   {
      fprintf(stderr, "Could not open '%s': error n°%d\n", fileName.toStdString().c_str(), iRet);
      printf( "Could not open '%s'\n", fileName.toStdString().c_str());
      return false;
   }

   if(!avformat_write_header(pFormatCtxVideo, NULL))
    ok = true;

   return ok;
}

/**
   \brief Completes writing the stream, closes it, release resources.
**/
bool QVideoEncoder::close()
{
   if(!isOk())
      return false;

   av_write_trailer(pFormatCtxVideo);

   // close_video
   avcodec_close(pStream->codec);

   // Free the picture to be encoded
   freeFrame();

   // Free the output Buffer
   freeOutputBuf();

   /* free the streams */
   for(int i = 0; i < (int)pFormatCtxVideo->nb_streams; i++)
   {
      av_freep(&pFormatCtxVideo->streams[i]->codec);
      av_freep(&pFormatCtxVideo->streams[i]);
   }

   // Close file
   avio_close(pFormatCtxVideo->pb);

   // Free the stream
   av_free(pFormatCtxVideo);

   //   Set variable with default values
   initVars();
   return true;
}

/**
   \brief Encode one frame

   The frame must be of the same size as specified in the createFile call.

   This is the standard method to encode videos with fixed frame rates.
   Each call to encodeImage adds a frame, which will be played back at the frame rate
   specified in the createFile call.
**/
int QVideoEncoder::encodeImage(const QImage &img)
{
   return encodeImage_p(img);
}
/**
   \brief Encode one frame

   The frame must be of the same size as specified in the createFile call.

   This mehtod allows to specify the presentation time stamp (pts) of the frame.
   pts is specified in multiples of 1/framerate, where framerate was specified in the createFile call
   E.g. to encode frames with a 1ms resolution: set the frame rate to 1000, and pts is the presentation
   time in milliseconds.
   pts must be monotonously increasing.
   The first frame ought to have a pts of 0 to be immediately displayed.
**/
int QVideoEncoder::encodeImagePts(const QImage &img,unsigned pts)
{
   return encodeImage_p(img,true,pts);
}

/******************************************************************************
* INTERNAL   INTERNAL   INTERNAL   INTERNAL   INTERNAL   INTERNAL   INTERNAL
******************************************************************************/

void QVideoEncoder::initVars()
{
   ok                 = false;
   pFormatCtxVideo    = 0;
   pOutputFormatVideo = 0;
   pCodecCtxVideo     = 0;
   pStream            = 0;
   pCodecVideo        = 0;
   ppictureVideo      = NULL;
   outbuf             = 0;
   picture_buf        = 0;
   img_convert_ctx    = 0;
}

/**
   \brief Register the codecs
**/
bool QVideoEncoder::initCodec()
{
   ffmpeg::av_register_all();

   //printf("License: %s\n",ffmpeg::avformat_license());
   //printf("AVCodec version %d\n",ffmpeg::avformat_version());
   //printf("AVFormat configuration: %s\n",ffmpeg::avformat_configuration());

   return true;
}

/**
   \brief Encode one frame - internal function
   custompts: true if a custom presentation time stamp  is used
   pts: presentation time stamp in milliseconds
**/
int QVideoEncoder::encodeImage_p(const QImage &img,
                                 bool custompts,
                                 unsigned pts)
{
   if(!isOk())
      return -1;


   //QTimer timer;
   QTime *timer;
   timer->start();
   //convertImage(img);       // Custom conversion routine
   convertImage_sws(img);     // SWS conversion
   int ms = timer->elapsed();
   double s = ms / 1000.;
   //ms -= s;
   delete(timer);
   qWarning() << "Conversion took" << QString::number(ms) + "ms";





   if(custompts)                             // Handle custom pts
     pCodecCtxVideo->coded_frame->pts = pts;      // Set the time stamp

   //   Clear the output AVPacket pkt
   av_init_packet(&pkt);

   // packet data will be allocated by the encoder
   // as we don't know the size needed for this frame
   pkt.data = NULL;
   pkt.size = 0;

   ppictureVideo->height = img.height();
   ppictureVideo->width = img.width();
   ppictureVideo->format = img.format();

   int isEncodedFrameNotEmpty = 0;

   QTime *t = new QTime();
   t->start();
   ffmpeg::avcodec_encode_video2(pCodecCtxVideo,
                                 &pkt,
                                 ppictureVideo,
                                 &isEncodedFrameNotEmpty);
    ms = t->elapsed();
    delete(t);
    qWarning() << "\tConversion took " + QString::number(ms) + "ms";

   if(custompts)                        // Handle custom pts (must set it again for the rest of the processing)
     pCodecCtxVideo->coded_frame->pts = pts; // Set the time stamp

   if (isEncodedFrameNotEmpty)
   {
      if (pCodecCtxVideo->coded_frame->pts != AV_NOPTS_VALUE)
         pkt.pts= av_rescale_q(pCodecCtxVideo->coded_frame->pts,
                               pCodecCtxVideo->time_base,
                               pStream->time_base);

      if(pCodecCtxVideo->coded_frame->key_frame)
         pkt.flags |= AV_PKT_FLAG_KEY;

      pkt.stream_index= pStream->index;


      qWarning() << "\tSize of context: " + QString::number(sizeof(*pCodecCtxVideo));
      qWarning() << "\tSize of pkt: " + QString::number(sizeof(pkt));
      int ret = av_interleaved_write_frame(pFormatCtxVideo,
                                           &pkt);

      if(ret<0)
         return -1;
   }
   return pCodecCtxVideo->frame_bits;
}

/**
  Ensures sizes are some reasonable multiples
**/
bool QVideoEncoder::isSizeValid()
{
    if(getWidth()%8)
       return false;
    if(getHeight()%8)
       return false;
    return true;
}

unsigned QVideoEncoder::getWidth()
{
   return Width;
}

unsigned QVideoEncoder::getHeight()
{
   return Height;
}

bool QVideoEncoder::isOk()
{
   return ok;
}

/**
  Allocate memory for the compressed bitstream
**/
bool QVideoEncoder::initOutputBuf()
{
   outbuf_size = getWidth()*getHeight()*  3;        // Some extremely generous memory allocation for the encoded frame.
   outbuf = new uint8_t[outbuf_size];

   if(outbuf == 0)
      return false;

   return true;
}
/**
  Free memory for the compressed bitstream
**/
void QVideoEncoder::freeOutputBuf()
{
   if(outbuf)
   {
      delete[] outbuf;
      outbuf=0;
   }
}

bool QVideoEncoder::initFrame()
{
   ppictureVideo = ffmpeg::av_frame_alloc();
   if(ppictureVideo == 0)
      return false;

   int size = avpicture_get_size(pCodecCtxVideo->pix_fmt,
                                 pCodecCtxVideo->width,
                                 pCodecCtxVideo->height);

   picture_buf = new uint8_t[size];
   if(picture_buf == 0)
   {
      av_free(ppictureVideo);
      ppictureVideo = 0;
      return false;
   }

   // Setup the planes
   avpicture_fill((ffmpeg::AVPicture *)ppictureVideo,
                  picture_buf,
                  pCodecCtxVideo->pix_fmt,
                  pCodecCtxVideo->width,
                  pCodecCtxVideo->height);

   return true;
}

void QVideoEncoder::freeFrame()
{
   if(picture_buf)
   {
      delete[] picture_buf;
      picture_buf = 0;
   }

   if(ppictureVideo)
   {
      av_free(ppictureVideo);
      ppictureVideo = NULL;
   }
}

/**
  \brief Convert the QImage to the internal YUV format

  Custom conversion - not very optimized.

**/

bool QVideoEncoder::convertImage(const QImage &img)
{
   // Check if the image matches the size
   if(img.width()!= (int)getWidth() || img.height()!= (int)getHeight())
   {
      printf("Wrong image size!\n");
      return false;
   }

   if(img.format()!=QImage::Format_RGB32	&& img.format() != QImage::Format_ARGB32)
   {
      printf("Wrong image format 1\n");
      return false;
   }

   // RGB32 to YUV420
   int size = getWidth()* getHeight();

   // Y
   for(unsigned y = 0; y < getHeight(); y++)
   {
      unsigned char *s = (unsigned char*)img.scanLine(y);
      unsigned char *d = (unsigned char*)&picture_buf[y * getWidth()];
      printf("Line %d. d: %p. picture_buf: %p\n",y,d,picture_buf);

      for(unsigned x = 0; x < getWidth(); x++)
      {
         unsigned int r = s[2];
         unsigned int g = s[1];
         unsigned int b = s[0];

         unsigned Y = (r * 2104 + g * 4130 + b * 802 + 4096 + 131072) >> 13;
         if(Y > 235) Y = 235;

         *d = Y;
         d += 1;
         s += 4;
      }
   }

   // U,V
   for(unsigned y = 0; y < getHeight(); y += 2)
   {
      unsigned char *s = (unsigned char*)img.scanLine(y);
      unsigned int ss = img.bytesPerLine();
      unsigned char *d = (unsigned char*)&picture_buf[size+y/2*getWidth()/2];

      printf("Line %d. d: %p. picture_buf: %p\n",y,d,picture_buf);

      for(unsigned x=0;x<getWidth();x+=2)
      {
         // Cr = 128 + 1/256 * ( 112.439 * R'd -  94.154 * G'd -  18.285 * B'd)
         // Cb = 128 + 1/256 * (- 37.945 * R'd -  74.494 * G'd + 112.439 * B'd)

         // Get the average RGB in a 2x2 block
         int r = (s[2] + s[6] + s[ss+2] + s[ss+6] + 2) >> 2;
         int g = (s[1] + s[5] + s[ss+1] + s[ss+5] + 2) >> 2;
         int b = (s[0] + s[4] + s[ss+0] + s[ss+4] + 2) >> 2;

         int Cb = (-1214 * r - 2384 * g + 3598 * b + 4096 + 1048576) >> 13;
         if(Cb < 16)              Cb = 16;
         if(Cb > 240)             Cb = 240;

         int Cr = (3598 * r - 3013 * g - 585 * b + 4096 + 1048576) >> 13;
         if(Cr < 16)              Cr = 16;
         if(Cr > 240)             Cr = 240;

         *d = Cb;
         *(d + size/4) = Cr;

         d += 1;
         s += 8;
      }
   }
   return true;
}

/**
  \brief Convert the QImage to the internal YUV format

  SWS conversion

   Caution: the QImage is allocated by QT without guarantee about the alignment and bytes per lines.
   It *should* be okay as we make sure the image is a multiple of many bytes (8 or 16)...
   ... however it is not guaranteed that sws_scale won't at some point require more bytes per line.
   We keep the custom conversion for that case.

**/

bool QVideoEncoder::convertImage_sws(const QImage &img)
{
   QImage::Format format;
   // Check if the image matches the size
   if(img.width()!= (int)getWidth() || img.height()!= (int)getHeight())
   {
      printf("Wrong image size!\n");
      return false;
   }

   if((format = img.format()) != QImage::Format_RGB32
      && ((format = img.format()) != QImage::Format_ARGB32))
   {
        printf("Wrong image format 2\n");
        return false;
   }

   img_convert_ctx = ffmpeg::sws_getCachedContext(img_convert_ctx,
                                                  getWidth(),
                                                  getHeight(),
                                                  ffmpeg::AV_PIX_FMT_BGRA,
																									getWidth(),
                                                  getHeight(),
                                                  ffmpeg::AV_PIX_FMT_YUV420P,
                                                  SWS_FAST_BILINEAR,
                                                  NULL,
                                                  NULL,
                                                  NULL);

   if (img_convert_ctx == NULL)
   {
      printf("Cannot initialize the conversion context\n");
      return false;
   }

   uint8_t *srcplanes[3];
   srcplanes[0] = (uint8_t*)img.bits();
   srcplanes[1] = 0;
   srcplanes[2] = 0;

   int srcstride[3];
   srcstride[0] = img.bytesPerLine();
   srcstride[1] = 0;
   srcstride[2] = 0;

   ffmpeg::sws_scale(img_convert_ctx,
                     srcplanes,
                     srcstride,
                     0,
                     getHeight(),
                     ppictureVideo->data,
                     ppictureVideo->linesize);

   return true;
}
