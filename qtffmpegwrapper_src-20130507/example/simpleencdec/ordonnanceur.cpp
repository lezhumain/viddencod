#include "ordonnanceur.hpp"
#include <QDebug>
#include <QPixmap>
#include <QPainter>

#include "fifo.hpp"


Ordonnanceur* Ordonnanceur::_instance = NULL;

Ordonnanceur::Ordonnanceur(const short nbThread, const QString& filename) :
    QObject(),
    _nbThread(nbThread),
    _filename(filename)
{
//    CreateThread();
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
    QList<QImage> frames = getAllFrames();
    int nbF = frames.length();

    if(nbF == 0)
    {
        qWarning() << "Didn't get any frame.";
        Ordonnanceur::Kill();
        return false;
    }

    // putframes in FIFO
    for(int i = 0; i < nbF; ++i)
    {
        QImage img = frames.takeFirst();

        frame_t tframe;
        tframe.frame = img;
        tframe.index = i;

        _fifoFrame.PushBack(tframe);
    }

    StartThread();
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
//      QMessageBox::critical(this,"Error","Error loading the video");
        qWarning() << "Error loading the video";
        return false;
   }

   // Get first frame
//   nextFrame();
   // Display a frame
//   displayFrame();

   return true;
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
//      QMessageBox::critical(this,"Error","Error decoding the frame");
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
//      errLoadVideo();
        qWarning() << "Error loading the video";
        return false;
   }
   return true;
}

QList<QImage> Ordonnanceur::getAllFrames()
{
    short frameRate = 25;
    int lengthMs, maxFrames;

    bool loaded = loadVideo(_filename);
    QList<QImage> listIm;

    if(loaded)
    {
        lengthMs = m_decoder.getVideoLengthSeconds(); // open video first
        qWarning() << "length" << QString::number(lengthMs) + "ms";

        maxFrames = lengthMs * frameRate / 1000; // not working
        maxFrames = maxFrames < 0 ? 50000 : maxFrames;
        qWarning() << "maxframes" << maxFrames ;

        for(int i = 0; i < maxFrames; ++i)
        {
            QImage img;
            int eframeNumbern, frameTime;
            if(!m_decoder.getFrame(img,&eframeNumbern,&frameTime))
            {
    //           QMessageBox::critical(this,"Error","Error decoding the frame");
                qWarning() << "Error decoding the frame";
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
    }

    return listIm;
}

bool Ordonnanceur::nextFrame()
{
   if(!m_decoder.seekNextFrame())
   {
//      QMessageBox::critical(this,"Error","seekNextFrame failed");
       qWarning() << "seekNextFrame failed";
       return false;
   }

   return true;
}
