#include "ordonnanceur.hpp"
#include <QDebug>

#include "fifo.hpp"


Ordonnanceur* Ordonnanceur::_instance = NULL;

Ordonnanceur::Ordonnanceur(const short nbThread) :
    QObject(),
    _nbThread(nbThread)
{
    CreateThread();
    qWarning() << "Ordonnanceur created.";
}

Ordonnanceur::~Ordonnanceur()
{
    // TODO delete agents
//    for(int i = 0; i < _nbThread; ++i)
//    {
////        AgentEncoder *agent = _lstAgent.takeFirst();
////        delete(agent);

////        _lstAgent.removeFirst();
//    }

    qWarning() << "Ordonnanceur destroyed";
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
        for(; i < length; ++i)
            if(_lstAgent.at(i)->getId() == idagent)
                break;

        if(i == length)
        {
            qWarning() << "\tOrdonnanceur::OnFinished" << "ERROR didn't find idagent in _lst' Ln70\n";
            return;
        }

        // else
        AgentEncoder *agent = _lstAgent.takeAt(i);
        delete(agent);
    }

    if(length == 1)
        Kill(); // can delete...
}

bool Ordonnanceur::WriteVideo()
{
    return 1;
}

void Ordonnanceur::PushFrameToFifo(QImage frame)
{
    this->_fifoFrame.PushBack(frame);
}

QImage Ordonnanceur::PopFrame()
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
