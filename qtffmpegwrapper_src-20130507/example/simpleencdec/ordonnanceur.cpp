#include "ordonnanceur.hpp"

#include "fifo.hpp"
Ordonnanceur::Ordonnanceur(const short nbThread) :
    QObject(),
    _nbThread(nbThread)
{

}


bool Ordonnanceur::CreateThread()
{
    for (int i = 0; i < _nbThread; ++i)
    {

    }
}

int Ordonnanceur::StartThread()
{

}

int Ordonnanceur::StopThread()
{

}

void Ordonnanceur::OnFinished(int idagent)
{

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
