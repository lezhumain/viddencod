#include "ordonnanceur.hpp"
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include "logmanager.hpp"

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

bool Ordonnanceur::Start()
{
    CreateThread();

    // get all frames
//    _lstFrameToBeEncoded = getAllFrames();
//    int nbF = _lstFrameToBeEncoded.length();
    bool ret = loadAllFrames();
    int nbF = _lstFrameToBeEncoded.length();

    if(nbF == 0)
    {
        qWarning() << "Didn't get any frame.";
        Ordonnanceur::Kill();
        return false;
    }
    qWarning() << "getAllFrames() return" << ret;

    qWarning() << "ORdo got" << nbF << "frames from GetAllFrames";

    // putframes in FIFO
//    for(int i = 0; i < nbF; ++i)
//    {
//        frame_t tframe = _lstFrameToBeEncoded.takeFirst();
//        _fifoFrame.PushBack(tframe);
//    }

    StartThread();
    return true;
}

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
    double maxFrames = lengthMs * (double)((m_FrameRateDecodedVideo.den
                                          / m_FrameRateDecodedVideo.num));
    //    maxFrames = maxFrames < 0 ? 50000 : maxFrames;
    QList<Ordonnanceur::frame_t> listIm;

    qWarning() << "length" << lengthMs ;
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

}
