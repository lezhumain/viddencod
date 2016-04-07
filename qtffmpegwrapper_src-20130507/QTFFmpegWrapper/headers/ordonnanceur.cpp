#include "ordonnanceur.hpp"
#include "fifo.hpp"
bool Ordonnanceur::CreateThread()
{

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

}

Ordonnanceur::Ordonnanceur()
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
