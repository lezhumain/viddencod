/*
   QTFFmpegWrapper Demo
   Copyright (C) 2009,2010:
         Daniel Roggen, droggen@gmail.com

   All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qvideoapi.h"
#include "cio.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _videoPlayer()
{
    ui->setupUi(this);

    ConsoleInit();

    printf("Starting up\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::image2Pixmap(QImage &img,QPixmap &pixmap)
{
   // Convert the QImage to a QPixmap for display
   pixmap = QPixmap(img.size());
   QPainter painter;
   painter.begin(&pixmap);
   painter.drawImage(0,0,img);
   painter.end();
}

/******************************************************************************
*******************************************************************************
* Decoder demo   Decoder demo   Decoder demo   Decoder demo   Decoder demo
*******************************************************************************
******************************************************************************/

void MainWindow::on_actionLoad_video_triggered()
{
    // Prompt a video to load
   QString fileName = QFileDialog::getOpenFileName(this, "Load Video",QString(),"Video (*.avi *.mp4 *.asf *.mpg *.wmv)");
   if(!fileName.isNull())
   {
      loadVideo(fileName);
   }
}

/**
  Prompts the user for the video to load, and display the first frame
**/
void MainWindow::loadVideo(QString fileName)
{
   _videoPlayer.GetVideoDecoder()->openFile(fileName);
   if(_videoPlayer.GetVideoDecoder()->isOk()==false)
   {
      QMessageBox::critical(this,"Error","Error loading the video");
      return;
   }

   // Get a new frame
   nextFrame();
   // Display a frame
   displayFrame();
printf("Framerate : %d\n", _videoPlayer.GetFrameRate());

}

void MainWindow::errLoadVideo()
{
   QMessageBox::critical(this,"Error","Load a video first");
}
bool MainWindow::checkVideoLoadOk()
{
   if(_videoPlayer.GetVideoDecoder()->isOk()==false)
   {
      errLoadVideo();
      return false;
   }
   return true;
}

/**
  Decode and display a frame
**/
void MainWindow::displayFrame()
{
   // Check we've loaded a video successfully
   if(!checkVideoLoadOk())
      return;

   QImage img;

   // Decode a frame
   int et,en;
   if(!_videoPlayer.GetVideoDecoder()->getFrame(img,&en,&et))
   {
      QMessageBox::critical(this,"Error","Error decoding the frame");
      return;
   }
   // Convert the QImage to a QPixmap for display

   QPixmap p;
   image2Pixmap(img,p);
   // Display the QPixmap
   ui->labelVideoFrame->setPixmap(p);

   // Display the video size
   ui->labelVideoInfo->setText(QString("Size %2 ms. Display: #%3 @ %4 ms.").
                               arg(_videoPlayer.GetVideoDecoder()->getVideoLengthMs()).
                               arg(en).
                               arg(et));

}

QList<QImage> MainWindow::getAllFrames()
{
    short frameRate = 25;
    int lengthMs = _videoPlayer.GetVideoDecoder()->getVideoLengthMs();
    qWarning() << "length" << lengthMs ;

    int maxFrames = lengthMs * frameRate / 1000; // not working
    maxFrames = maxFrames < 0 ? 50000 : maxFrames;
    qWarning() << "maxframes" << maxFrames ;

    QList<QImage> listIm;

    for(int i = 0; i < maxFrames; ++i)
    {
        QImage img;
        int eframeNumbern, frameTime;
        if(!_videoPlayer.GetVideoDecoder()->getFrame(img,&eframeNumbern,&frameTime))
        {
           QMessageBox::critical(this,"Error","Error decoding the frame");
           listIm.clear();
           return listIm;
        }
        listIm.append(img);

        if(!nextFrame() || i == 1000000)
        {
            qWarning() << "Current frame:" << eframeNumbern << "[i =" << i << "]";
            break;
        }
    }

    return listIm;
}

bool MainWindow::nextFrame()
{
   if(!_videoPlayer.GetVideoDecoder()->seekNextFrame())
   {
//      QMessageBox::critical(this,"Error","seekNextFrame failed");
       qWarning() << "seekNextFrame failed";
       return false;
   }

   return true;
}

/**
  Display next frame
**/
void MainWindow::on_pushButtonNextFrame_clicked()
{
   nextFrame();
   displayFrame();
}


void MainWindow::on_pushButtonSeekFrame_clicked()
{
   // Check we've loaded a video successfully
   if(!checkVideoLoadOk())
      return;

   bool ok;

   int frame = ui->lineEditFrame->text().toInt(&ok);
   if(!ok || frame < 0)
   {
      QMessageBox::critical(this,"Error","Invalid frame number");
      return;
   }

   // Seek to the desired frame
   if(!_videoPlayer.GetVideoDecoder()->seekFrame(frame))
   {
      QMessageBox::critical(this,"Error","Seek failed");
      return;
   }
   // Display the frame
   displayFrame();

}


void MainWindow::on_pushButtonSeekMillisecond_clicked()
{
   // Check we've loaded a video successfully
   if(!checkVideoLoadOk())
      return;

   bool ok;

   int ms = ui->lineEditMillisecond->text().toInt(&ok);
   if(!ok || ms < 0)
   {
      QMessageBox::critical(this,"Error","Invalid time");
      return;
   }

   // Seek to the desired frame
   if(!_videoPlayer.GetVideoDecoder()->seekMs(ms))
   {
      QMessageBox::critical(this,"Error","Seek failed");
      return;
   }
   // Display the frame
   displayFrame();
}

/******************************************************************************
*******************************************************************************
* Encoder demo   Encoder demo   Encoder demo   Encoder demo   Encoder demo
*******************************************************************************
******************************************************************************/

/**
  Prompts the user for a file
  Create the file
  Pass the file to the video generation function (alternatively the file name could be passed)
**/
void MainWindow::on_actionSave_synthetic_video_triggered()
{
   QString title("Save a synthetic video");
   QString fileName = QFileDialog::getSaveFileName(this, title,QString(),"Video (*.avi *.asf *.mpg)");
   if(!fileName.isNull())
   {
      GenerateSyntheticVideo(fileName);
   }
}

void MainWindow::on_actionSave_synthetic_variable_frame_rate_video_triggered()
{
   QString title("Save a synthetic video with variable frame rate");
   QString fileName = QFileDialog::getSaveFileName(this, title,QString(),"Video (*.avi *.asf *.mpg)");
   if(!fileName.isNull())
   {
      GenerateSyntheticVideo(fileName,true);
   }
}


void MainWindow::on_actionEncode_video_triggered()
{
    int secElapsed;

    if(!checkVideoLoadOk())
    {
       on_actionLoad_video_triggered();
       if(!checkVideoLoadOk())
           return;
    }


    QTime start = QTime::currentTime();
    start.start();

//    QList<QImage> listImg = getAllFrames();

    short nbFrames = GenerateEncodedVideo("../../../test.avi");

    if(nbFrames == -1)
    {
        printf("An error happened...");
        return;
    }

    secElapsed = start.elapsed() / 1000.;
    QDateTime test = QDateTime::fromTime_t(secElapsed).toUTC();


    QString msg = "Encoded " + QString::number(nbFrames) + " frames at " + QString::number(nbFrames / secElapsed)
            + "f/s (Total Time: "
            + test.toString("hh:mm:ss") + ").";

    printf(msg.toStdString().c_str());
    qWarning() << msg;
}

//void MainWindow::on_actionSave_synthetic_video_triggered()
//{
//   QString title("Save a synthetic video");
//   QString fileName = QFileDialog::getSaveFileName(this, title,QString(),"Video (*.avi *.asf *.mpg)");
//   if(!fileName.isNull())
//   {
//      GenerateSyntheticVideo(fileName);
//   }
//}


void MainWindow::GenerateSyntheticVideo(QString filename, bool vfr)
{
   int width=640;
   int height=480;
   int bitrate=1000000;
   int gop = 20;
   int fps = 25;

   // The image on which we draw the frames
   QImage frame(width,height,QImage::Format_RGB32);     // Only RGB32 is supported

   // A painter to help us draw
   QPainter painter(&frame);
   painter.setBrush(Qt::red);
   painter.setPen(Qt::white);

   // Create the encoder
   QVideoEncoder encoder;
   if(!vfr)
      encoder.createFile(filename,width,height,bitrate,gop,fps);        // Fixed frame rate
   else
      encoder.createFile(filename,width,height,bitrate*1000/fps,gop,1000);  // For variable frame rates: set the time base to e.g. 1ms (1000fps),
                                                                           // and correct the bitrate according to the expected average frame rate (fps)

   QEventLoop evt;      // we use an event loop to allow for paint events to show on-screen the generated video

   // Generate a few hundred frames
   int size=0;
   int maxframe=500;
   unsigned pts=0;
   for(unsigned i=0;i<maxframe;i++)
   {
      // Clear the frame
      painter.fillRect(frame.rect(),Qt::red);   

      // Draw a moving square
      painter.fillRect(width*i/maxframe,height*i/maxframe,30,30,Qt::blue);

      // Frame number
      painter.drawText(frame.rect(),Qt::AlignCenter,QString("Frame %1\nLast frame was %2 bytes").arg(i).arg(size));

      // Display the frame, and processes events to allow for screen redraw
      QPixmap p;


      image2Pixmap(frame,p);
      ui->labelVideoFrame->setPixmap(p);


      evt.processEvents();

      if(!vfr)
         size=encoder.encodeImage(frame);                      // Fixed frame rate
      else
      {
         // Variable frame rate: the pts of the first frame is 0,
         // subsequent frames slow down
         pts += sqrt(i);
         if(i==0)
            size=encoder.encodeImagePts(frame,0);
         else
            size=encoder.encodeImagePts(frame,pts);
      }

      printf("Encoded: %d\n",size);
   }

   encoder.close();

}

int MainWindow::GenerateEncodedVideo(QString filename,bool vfr)
{
    short frameRate = 25;
    int lengthMs = _videoPlayer.GetVideoDecoder()->getVideoLengthMs();
    qWarning() << "length" << lengthMs ;

    int maxFrames = lengthMs * frameRate / 1000; // not working
    maxFrames = maxFrames < 0 ? 50000 : maxFrames;
    qWarning() << "maxframes" << maxFrames ;


    int width, height;
    int bitrate=476000; // 1000000
    int gop = 20;
    int fps = 25;

    // The image on which we draw the frames
    QImage frame;

    // Create the encoder
    QVideoEncoder encoder;

    // Generate a few hundred frames
    int size = 0;
    int maxframe = 3 * 3600 * frameRate ; // TODO: get actual total frame
    unsigned pts = 0;

    int i = 0;
    for(; i < maxFrames; ++i)
    {
        QImage frame;
        int eframeNumbern, frameTime;
        if(!_videoPlayer.GetVideoDecoder()->getFrame(frame,&eframeNumbern,&frameTime))
        {
           QMessageBox::critical(this,"Error","Error decoding the frame");
           return -1;
        }

        if(i == 0)
        {
             width = frame.width();
             height = frame.height();
             if(!vfr)
                encoder.createFile(filename,width,height,bitrate,gop,fps);        // Fixed frame rate
             else
                encoder.createFile(filename,width,height,bitrate*1000/fps,gop,1000);  // For variable frame rates: set the time base to e.g. 1ms (1000fps),
        }                                                                             // and correct the bitrate according to the expected average frame rate (fps)


        // handle
        // Display the frame, and processes events to allow for screen redraw
        QPixmap p;
//        frame = images.at(i);

        frame = frame.convertToFormat(QImage::Format_RGB32);

        image2Pixmap(frame,p);
//        ui->labelVideoFrame->setPixmap(p);


        if(!vfr)
          size=encoder.encodeImage(frame);                      // Fixed frame rate
        else
        {
          // Variable frame rate: the pts of the first frame is 0,
          // subsequent frames slow down
          pts += sqrt(i);
          if(i==0)
             size=encoder.encodeImagePts(frame,0);
          else
             size=encoder.encodeImagePts(frame,pts);
        }

        printf("Encoded: %d\n",size);
        // ------



        if(!nextFrame() || i == 1000000)
        {
            qWarning() << "Current frame:" << eframeNumbern << "[i =" << i << "]";
            break;
        }
    }

    encoder.close();
    printf("Done encoding.");
    return i;
}

///
/// \brief Generates a video encoded with mpeg4 layer 2
/// \param images list of images representing the video's frames
/// \param filename the name of the created video
/// \param vfr ???
///
void MainWindow::GenerateEncodedVideo(QList<QImage> &images, QString filename,bool vfr)
{
    int width = images.at(0).width();
    int height = images.at(0).height();
    int bitrate=476000; // 1000000
    int gop = 20;
    int fps = 25;

    // The image on which we draw the frames
    QImage frame;

    // Create the encoder
    QVideoEncoder encoder;
    if(!vfr)
       encoder.createFile(filename,width,height,bitrate,gop,fps);        // Fixed frame rate
    else
       encoder.createFile(filename,width,height,bitrate*1000/fps,gop,1000);  // For variable frame rates: set the time base to e.g. 1ms (1000fps),
                                                                            // and correct the bitrate according to the expected average frame rate (fps)


    // Generate a few hundred frames
    int size=0;
    int maxframe=images.length();
    unsigned pts=0;
    for(unsigned i=0;i<maxframe;i++)
    {
        // Display the frame, and processes events to allow for screen redraw
        QPixmap p;
        frame = images.at(i);

        frame = frame.convertToFormat(QImage::Format_RGB32);

//       p = QPixmap::fromImage(frame);
        image2Pixmap(frame,p);
        ui->labelVideoFrame->setPixmap(p);



//       if(!p.save("../../../frame" + QString::number((int)i) + ".png"))
//           printf("Image NOT Written");


       
       if(!vfr)
          size=encoder.encodeImage(frame);                      // Fixed frame rate
       else
       {
          // Variable frame rate: the pts of the first frame is 0,
          // subsequent frames slow down
          pts += sqrt(i);
          if(i==0)
             size=encoder.encodeImagePts(frame,0);
          else
             size=encoder.encodeImagePts(frame,pts);
       }

       printf("Encoded: %d\n",size);
    }

    encoder.close();
    printf("Done encoding.");
}



