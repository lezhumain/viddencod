#include "ordonnanceur.hpp"
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include "logmanager.hpp"
#include <math.h>

#include "fifo.h"


Ordonnanceur* Ordonnanceur::_instance = NULL;

Ordonnanceur::Ordonnanceur(const short nbThread, const QString& filename) :
    QObject(),
    _nbThread(nbThread),
    _filename(filename),
    m_NbFramesDecodedVideo(-1)
{
//    CreateThread();
    m_FrameRateDecodedVideo.num = -1;
    m_FrameRateDecodedVideo.den = -1;
    qWarning() << "Ordonnanceur created.";
}

Ordonnanceur::~Ordonnanceur()
{
    if(!_lstAgent.empty())
    {
        int len = _lstAgent.length();
        for(int i = 0; i < len; ++i)
            _lstAgent.removeFirst();
    }

    qWarning() << "Ordonnanceur destroyed";
}

bool Ordonnanceur::loadAllFrames()
{
    bool loaded;
//    int eframeNumbern = 0;
//    int frameTime = 0;
    double dLengthSec = -1;
    unsigned int maxFrames = -1;
//    QList<Ordonnanceur::frame_t> listIm;
    //Number of frames per second for the output video
    double dframeRate;

    loaded = loadVideo(_filename);

    if(!loaded)
    {
        qWarning() << "Could not open video.";
//        return listIm;
        return false;
    }


    dframeRate = (float)m_FrameRateDecodedVideo.num / m_FrameRateDecodedVideo.den;
    dLengthSec = m_decoder.getVideoLengthSeconds();
    maxFrames = (int)(dLengthSec  * dframeRate);

    qWarning() << "Longueur de la vidéo : " << dLengthSec << " secondes\n"
               << "Nombre total de frames de la vidéo :" << maxFrames << '\n'
               << "FPS :" << dframeRate ;

    unsigned i = 0;
    for(; i < maxFrames; ++i)
    {
        Ordonnanceur::frame_t sframe;
//        QImage img;

        if(!m_decoder.getFrame(sframe.frame, &sframe.eframeNumbern, &sframe.frameTime))
        {
            qWarning() << "Error decoding the frame";
//            listIm.clear();
//            _fifoFrame.
//            return listIm;
            return false;
        }

        if(sframe.frame.isNull() || sframe.frame.format() == QImage::Format_Invalid)
        {
            qWarning() << "Ordonnanceur::getAllFrames Got a null invalid frame";
            return false;
//            sframe = listIm.last();
        }
//        else
//            sframe.frame = img;
//        qWarning() << "\tsize before" << sizeof(sframe.frame);
//        sframe.frame = sframe.frame.convertToFormat(QImage::Format_RGB32);
//        qWarning() << "\tsize  after" << sizeof(sframe.frame);
//        listIm.append(sframe);
        _fifoFrame.PushBack(sframe);

        //WriteVideo(sframe, i);

        if(!nextFrame())
        {
            qWarning() << "Current frame:" << sframe.eframeNumbern << "[i =" << i << "]";
            break;
        }
    }

    qWarning() << "getAllFrame: i=" << i;
    return true;
}

short Ordonnanceur::Start()
{
    short nbFrames = 0;
    QString oFileName = "../../videos/output1.avi";

    if(!checkVideoLoadOk())
    {
//       on_actionLoad_video_triggered();
        loadVideo(_filename);
       if(!checkVideoLoadOk())
       {
            LogManager::GetInstance()->LogWarning(0, "could not open video....", true);
            return -1;
       }
    }


//    QList<QImage> listImg = getAllFrames();

//    QString title("Save an encoded video ");
//    QString fileName = QFileDialog::getSaveFileName(this, title,QString(),"Video (*.avi *.mp4 *.mpg)");
//    if(!fileName.isNull())
//      nbFrames = GenerateEncodedVideo(fileName.toStdString().c_str(), false);

//    QString title("Save an encoded video ");
//    QString fileName = QFileDialog::getSaveFileName(this, title,QString(),"Video (*.avi *.asf *.mpg)");
//    if(!fileName.isNull())
//        nbFrames = GenerateEncodedVideo(fileName.toStdString().c_str(), false);

//    nbFrames = GenerateEncodedVideo(fileName.toStdString().c_str(), false);
    nbFrames = GenerateEncodedVideo(oFileName.toStdString().c_str(), false);
    if(nbFrames == -1)
    {
        printf("An error happened...");
//        QMessageBox::information(this,"Info","Couldn't encode video");
        LogManager::GetInstance()->LogError(0, "Couldn't encode video");
        return -1;
    }


//    QString msg = "Encoded "
//            + QString::number(nbFrames)
//            + " frames at "
//            + QString::number(nbFrames / secElapsed)
//            + "f/s (Total Time: "
//            + test.toString("hh:mm:ss")
//            + ").";

//    printf("%s\n", msg.toStdString().c_str());
//    qWarning() << msg;

    return nbFrames;
}

//bool Ordonnanceur::Start()
//{
//    bool ret;
//    int nbF, nbFrames;

// //    CreateThread();

//    if(!checkVideoLoadOk())
//    {
//        loadVideo(_filename);
//        if(!checkVideoLoadOk())
//            return false;
//    }

//    // get all frames
//    _lstFrameToBeEncoded = getAllFrames();
// //    ret = loadAllFrames();
//    nbF = _lstFrameToBeEncoded.length();

//    if(nbF == 0)
//    {
//        qWarning() << "Didn't get any frame.";
//        Ordonnanceur::Kill();
//        return false;
//    }
// //    qWarning() << "getAllFrames() return" << ret;

//    qWarning() << "ORdo got" << nbF << "frames from GetAllFrames";

//    // putframes in FIFO
// //    for(int i = 0; i < nbF; ++i)
// //    {
// //        frame_t tframe = _lstFrameToBeEncoded.takeFirst();
// //        _fifoFrame.PushBack(tframe);
// //    }


//    QString oFileName = "../../videos/output.avi";
// //    nbFrames = GenerateEncodedVideo(fileName.toStdString().c_str(), false);
//    nbFrames = GenerateEncodedVideo(oFileName.toStdString().c_str(), false);
//    if(nbFrames == -1)
//    {
//        printf("An error happened...");
//        LogManager::GetInstance()->LogWarning(0, "Couldn't encode video", true);
//        LogManager::GetInstance()->LogError(0, "Couldn't encode video");
//        return false;
//    }


// //    StartThread();
//    return true;
//}

bool Ordonnanceur::CreateThread()
{
    for (int i = 0; i < _nbThread; ++i)
    {
        AgentEncoder *agent = new AgentEncoder(i, this);

        QObject::connect(this, SIGNAL(ThreadStart()), agent, SLOT(Run()));
        QObject::connect(agent, SIGNAL(Finished(short)), this, SLOT(OnFinished(short)));

        _lstAgent.append(agent);
    }
    return true;
}

int Ordonnanceur::StartThread()
{
    emit ThreadStart();
    return 0;
}

int Ordonnanceur::StopThread()
{
    return 0;
}

void Ordonnanceur::OnFinished(const short idagent)
{
    const int length = _lstAgent.length();
    int i = 0;
//    static short count = 0;
//    ++count;
//    if(count == _nbThread)
//        delete(this);

    if(length > 0)
    {
        // find index of agent with id idagent
        for(; i < length; ++i)
            if(_lstAgent.at(i)->getId() == idagent)
                break;

        // not found
        if(i == length)
        {
            qWarning() << "\tOrdonnanceur::OnFinished" << "ERROR didn't find idagent in _lst' Ln70\n";
            return;
        }

        // take from list & delete
        AgentEncoder *agent = _lstAgent.takeAt(i);
        delete(agent);
//        _lstAgent.removeAt(i); // ???
    }

    if(length == 1)
        Ordonnanceur::Kill(); // can delete
}

void Ordonnanceur::PushFrameToFifo(frame_t frame)
{
    this->_fifoFrame.PushBack(frame);
}

Ordonnanceur::frame_t Ordonnanceur::PopFrame()
{
    return this->_fifoFrame.PopFront();
}

void Ordonnanceur::ClearFifo()
{
    return;
}

unsigned int Ordonnanceur::GetFifoLength() const
{
    return this->_fifoFrame.Count();
}

Ordonnanceur* Ordonnanceur::GetInstance(const short nbThread)
{
    if(_instance == NULL)
    {
        if(nbThread == 0)
            return NULL;
        else
            _instance = new Ordonnanceur(nbThread);
    }

    return _instance;
}

void Ordonnanceur::Kill()
{
    if(_instance == NULL)
        return;

    delete(_instance);
    _instance = NULL;
}



// VIDEO REGION

/**
  Prompts the user for the video to load, and display the first frame
**/
bool Ordonnanceur::loadVideo(QString fileName)
{
    m_decoder.openFile(fileName);
    if(m_decoder.isOk()==false)
    {
       LogManager::GetInstance()->LogError(0, "Error loading the video", true);
       return false;
    }

    // Get a new frame
    nextFrame();

    // Display a frame
    displayFrame();
     ffmpeg::AVRational m_FrameRateDecodedVideotmp;

    m_decoder.GetFPS(&m_FrameRateDecodedVideotmp.num,
                     &m_FrameRateDecodedVideotmp.den);
    m_FrameRateDecodedVideo.num = m_FrameRateDecodedVideotmp.num;
    m_FrameRateDecodedVideo.den = m_FrameRateDecodedVideotmp.den;

    m_encoder.SaveTmpFrameRate(&m_FrameRateDecodedVideo);

    LogManager::GetInstance()->LogInfo(0, "Video loaded", true);
    return true;
}

/**
  Decode a frame
**/
bool Ordonnanceur::displayFrame()
{
    QImage img;
    QPixmap p;
    int et,en;

    // Check we've loaded a video successfully
   if(!checkVideoLoadOk())
      return false;

   // Decode a frame
   if(!m_decoder.getFrame(img,&en,&et))
   {
      LogManager::GetInstance()->LogError(0, "Error decoding the frame", true);
      return false;
   }

   return true;
   // Convert the QImage to a QPixmap for display
//   image2Pixmap(img,p);

   // Display the QPixmap
//   ui->labelVideoFrame->setPixmap(p);

   // Display the video size
//   ui->labelVideoInfo->setText(QString("Size %2 ms. Display: #%3 @ %4 ms.").arg(m_decoder.getVideoLengthSeconds()).arg(en).arg(et));
}

void Ordonnanceur::image2Pixmap(QImage &img,QPixmap &pixmap)
{

    // Convert the QImage to a QPixmap for display
//    pixmap = QPixmap(img.size());
//    QPainter painter;
//    painter.begin(&pixmap);
//    painter.drawImage(0,0,img);
//    painter.end();
    pixmap = QPixmap::fromImage(img);
}

bool Ordonnanceur::checkVideoLoadOk()
{
    if(m_decoder.isOk()==false)
    {
       LogManager::GetInstance()->LogInfo(0, "Video is not loaded.", true);
       return false;
    }
    return true;
}

QList<Ordonnanceur::frame_t> Ordonnanceur::getAllFrames()
{
    double lengthMs = m_decoder.getVideoLengthSeconds();
    double maxFrames = lengthMs * (double)(m_FrameRateDecodedVideo.den
                                          / m_FrameRateDecodedVideo.num) / 1000;
    //    maxFrames = maxFrames < 0 ? 50000 : maxFrames;
    QList<Ordonnanceur::frame_t> listIm;

    qWarning() << "length" << QString::number(lengthMs) + "ms" ;
    qWarning() << "maxframes" << maxFrames ;

    for(int i = 0; i <(int) maxFrames; ++i)
    {
        QImage img;
        Ordonnanceur::frame_t sframe;
        int eframeNumbern, frameTime;
        if(!m_decoder.getFrame(img,&eframeNumbern,&frameTime))
        {
           LogManager::GetInstance()->LogError(0, "Error decoding the frame", true);
           listIm.clear();
           return listIm;
        }

        sframe.frame = img;
        sframe.eframeNumbern = eframeNumbern;
        sframe.frameTime = frameTime;

        listIm.append(sframe);

        if(!nextFrame() || i == 1000000)
        {
            qWarning() << "Current frame:" << eframeNumbern << "[i =" << i << "]";
            break;
        }
    }

    return listIm;
}

bool Ordonnanceur::WriteVideo(frame_t sframe, int iFrame)
{
//    ffmpeg::AVRational toto;
//    m_encoder.GetFramerate(&toto);

//    int result = toto.num / toto.den;

//    if(iFrame == 0)
//    {
//        QString _filename_output = "test_output.avi";
//        m_encoder.createFile(_filename_output,
//                             sframe.frame.width(),
//                             sframe.frame.height(),
//                             1000000,
//                             1,
//                             result);
//    }
//    m_encoder.encodeImage(sframe.frame);
//    return 0;
}

bool Ordonnanceur::nextFrame()
{
    bool bret = false;
    if(!m_decoder.seekNextFrame())   qWarning() << "seekNextFrame failed";
    else                             bret = true;

    return bret;
}

int Ordonnanceur::GenerateEncodedVideo(QString filename, bool vfr)
{
    int bitrate       = 1000000;
    int gop           = 1;
    int eframeNumbern = 0;
    int frameTime     = 0;

    //Number of frames per second for the output video
    double dframeRate = ((double)(m_FrameRateDecodedVideo.den) /
                       (double)m_FrameRateDecodedVideo.num);

    int totalFramesVideo = 0;
    bool fileOk = false;

    // Generate a few hundred frames
    int size = 0;

    // we use an event loop to allow for paint events to show on-screen the generated video
//    QEventLoop evt;

    int i;
    double m_lengthMs;

    // The image on which we draw the frames
    QImage frame;
    QList<Ordonnanceur::frame_t> frames;

    // Display the frame, and processes events to allow for screen redraw
    QPixmap p;

    m_lengthMs = m_decoder.getVideoLengthSeconds();
    qWarning() << "Longueur de la vidéo : " << QString::number(m_lengthMs) + "ms";

    //  number of frames : TIME_TOTAL_MSEC * FRAMES_PER_SEC
    totalFramesVideo = (int)((m_lengthMs  * dframeRate) / 1000);
    qWarning() << "Nombre total de frames de la vidéo :" << totalFramesVideo ;
    qWarning() << "FPS :" << dframeRate ;

    qWarning() << "Retrieving all frames...";
    frames = getAllFrames();

    qWarning() << "Frames received:" << frames.length();



    for(i = 0; i < totalFramesVideo; ++i)
    {
        //  Decoding the frames one by one
        if(!m_decoder.getFrame(frame,&eframeNumbern,&frameTime))
        {
           LogManager::GetInstance()->LogError(0, "Error decoding the frame", true);
           return -1;
        }

        //  If we are with the first frame,
        //  we create the output file
        if(i == 0)
        {
             if(!vfr)
                fileOk = m_encoder.createFile(filename,
                                   frame.width(),
                                   frame.height(),
                                   bitrate,
                                   gop,
                                   (int)dframeRate);        // Fixed frame rate
             else
                fileOk = m_encoder.createFile(filename,
                                   frame.width(),
                                   frame.height(),
                                   bitrate * 1000 / (int)dframeRate,
                                   gop,
                                   1000);  // For variable frame rates: set the time base to e.g. 1ms (1000fps),

            if(!fileOk)
                break;
        }                                                                         // and correct the bitrate according to the expected average frame rate (fps)

        // handle
        if(frame.format() != QImage::Format_RGB32)
            frame = frame.convertToFormat(QImage::Format_RGB32);
        //  Paste the decoded frame into the QPixmap for display the data
//        image2Pixmap(frame,p);
//        ui->labelVideoFrame->setPixmap(p);

        //  Force to paint the QPixmap with the new decoded frame
//        evt.processEvents();

        // Display the video size
//        ui->labelVideoInfo->setText(QString("Size %2 ms. Display: #%3 @ %4 ms.").arg(m_decoder.getVideoLengthSeconds()).arg(eframeNumbern).arg(frameTime));

        //ffmpeg::av_usleep(50000);

        if(!vfr)
          size = m_encoder.encodeImage(frame);                      // Fixed frame rate
        else
        {
          //  Timestamp for the encoded video
          unsigned pts = 0;

          // Variable frame rate: the pts of the first frame is 0,
          // subsequent frames slow down
          pts += ffmpeg::sqrt(i);

          if(!i)
            size = m_encoder.encodeImagePts(frame,0);
          else
          {
            size = m_encoder.encodeImagePts(frame, pts);
          }
        }

//        qWarning() << "Actual frame is " << eframeNumbern << " / " << totalFramesVideo ;
//        printf("Encoded: %d bytes \n",size);

        if(!nextFrame())
        {
            qWarning() << "Finished reading the video to be encoded" ;
            qWarning() << "Last frame is frame:" << eframeNumbern << "[i =" << i << "]";
            break;
        }
    }

    if(fileOk)
    {
        m_encoder.close();
        qWarning() << "Done encoding.";
    }
    else
    {
        qWarning() << "Couldn't encode.";
        i = -1;
    }

    return i;
}
