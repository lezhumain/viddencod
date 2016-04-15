#include "ordonnanceur.hpp"
#include <QDebug>
#include <QPixmap>
#include <QPainter>

#include "fifo.h"


Ordonnanceur* Ordonnanceur::_instance = NULL;

Ordonnanceur::Ordonnanceur(const short nbThread, const QString& filename) :
    QObject(),
    _nbThread(nbThread),
    _filename(filename)
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

bool Ordonnanceur::Start()
{
    CreateThread();

    // get all frames
    QList<frame_t> frames = getAllFrames();
    int nbF = frames.length();

    if(nbF == 0)
    {
        qWarning() << "Didn't get any frame.";
        Ordonnanceur::Kill();
        return false;
    }

    qWarning() << "ORdo got" << nbF << "frames from GetAllFrames";

    // putframes in FIFO
    for(int i = 0; i < nbF; ++i)
    {
        frame_t tframe = frames.takeFirst();
        _fifoFrame.PushBack(tframe);
    }

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
}

int Ordonnanceur::StartThread()
{
    emit ThreadStart();
}

int Ordonnanceur::StopThread()
{

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

bool Ordonnanceur::WriteVideo()
{
    return 1;
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
    if(_instance != NULL)
        delete(_instance);

    _instance = NULL;
}

/**
  Prompts the user for the video to load, and display the first frame
**/
bool Ordonnanceur::loadVideo(QString fileName)
{
    m_decoder.openFile(fileName);
    if(m_decoder.isOk()==false)
    {
       qWarning() << "Error loading the video";
       return false;
    }

    // Get a new frame
    nextFrame();

    // Display a frame
    displayFrame();
    ffmpeg::AVRational frameRateDecodedVideotmp;
    frameRateDecodedVideotmp.den = -1;
    frameRateDecodedVideotmp.num = -1;

    m_decoder.GetFPS(&frameRateDecodedVideotmp.num,
                     &frameRateDecodedVideotmp.den);
    m_FrameRateDecodedVideo.num = frameRateDecodedVideotmp.num;
    m_FrameRateDecodedVideo.den = frameRateDecodedVideotmp.den;

    // TODO test following
//    m_decoder.GetFPS(&m_FrameRateDecodedVideo.num,
//                     &m_FrameRateDecodedVideo.den);

    m_encoder.SaveTmpFrameRate(&m_FrameRateDecodedVideo);
}

/**
  Decode and display a frame
**/
void Ordonnanceur::displayFrame()
{
    QImage img;
    QPixmap p;
    int et,en;

    // Check we've loaded a video successfully
   if(!checkVideoLoadOk())
      return;

   // Decode a frame
   if(!m_decoder.getFrame(img,&en,&et))
   {
        qWarning() << "Error decoding the frame";
        return;
   }

   // Convert the QImage to a QPixmap for display
   image2Pixmap(img,p);

   // Display the QPixmap
//   ui->labelVideoFrame->setPixmap(p);

   // Display the video size
//   ui->labelVideoInfo->setText(QString("Size %2 ms. Display: #%3 @ %4 ms.").arg(m_decoder.getVideoLengthSeconds()).arg(en).arg(et));
}

void Ordonnanceur::image2Pixmap(QImage &img,QPixmap &pixmap)
{
    // Convert the QImage to a QPixmap for display
    pixmap = QPixmap(img.size());
    QPainter painter;
    painter.begin(&pixmap);
    painter.drawImage(0,0,img);
    painter.end();
}

bool Ordonnanceur::checkVideoLoadOk()
{
    if(m_decoder.isOk()==false)
    {
       qWarning() << "No video loaded";
        return false;
    }
    return true;
}

QList<Ordonnanceur::frame_t> Ordonnanceur::getAllFrames()
{
    bool loaded = loadVideo(_filename);

    double lengthMs = m_decoder.getVideoLengthSeconds(),
            maxFrames = m_decoder.getVideoFrameCount(),
            frameRate = maxFrames / lengthMs;
//    double maxFrames = (double)lengthMs * ((double)m_FrameRateDecodedVideo.num
//                                          / (double)m_FrameRateDecodedVideo.den);

    QList<Ordonnanceur::frame_t> listIm;

    qWarning() << "length" << lengthMs ;
    qWarning() << "maxFrames" << maxFrames ;
    qWarning() << "frameRate" << frameRate ;

    for(double i = 0; i < maxFrames; ++i)
    {
//        QImage img;
        Ordonnanceur::frame_t sframe;
//        int eframeNumbern, frameTime;

        if(!m_decoder.getFrame(sframe.frame, &sframe.eframeNumbern, &sframe.frameTime))
        {
            qWarning() << "Error decoding the frame";
            listIm.clear();
            return listIm;
        }
        if(sframe.frame.format() == QImage::Format_Invalid)
            sframe = listIm.last();

        listIm.append(sframe);

        if(!nextFrame())
        {
            qWarning() << "Current frame:" << sframe.eframeNumbern << "[i =" << i << "]";
            break;
        }
    }
}

bool Ordonnanceur::nextFrame()
{
    bool bret = false;
    if(!m_decoder.seekNextFrame())
        qWarning() << "seekNextFrame failed";
    else
        bret = true;

    return bret;
}

bool EncodeVideo(QString filename)
{

}
