#include "ordonnanceur.hpp"
#include "fifo.h"
bool Ordonnanceur::CreateThread()
{
    return true;
}

int Ordonnanceur::StartThread()
{
    return 0;
}

int Ordonnanceur::StopThread()
{
return 0;
}

void Ordonnanceur::OnFinished(int idagent)
{
return
}

bool Ordonnanceur::WriteVideo()
{
return true;
}

Ordonnanceur::Ordonnanceur()
{

}

Ordonnanceur::~Ordonnanceur()
{

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

unsigned int Ordonnanceur::GetFifoLength()
{
    return this->_fifoFrame.Count();
}
