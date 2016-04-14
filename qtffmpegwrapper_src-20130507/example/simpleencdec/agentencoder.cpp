#include "agentencoder.hpp"
#include <QDebug>
#include "ordonnanceur.hpp"

AgentEncoder::AgentEncoder(const short id, QObject *parent) :
    QObject(parent),
    _id(id)
{
    _ordo = reinterpret_cast<Ordonnanceur*>(parent);
    qWarning() << "Agent" << _id << "created.";
}

AgentEncoder::~AgentEncoder()
{
    qWarning() << "Agent" << _id << "destroyed";
}

// Declenchee quand l'ordo envoi le signal 'Start'
void AgentEncoder::Run()
{
//    if(_ordo == NULL)
//        _ordo = Ordonnanceur::GetInstance();
    // get frame from fifo
    Ordonnanceur::frame_t frame = _ordo->PopFrame();
    QImage *img = &(frame.frame);

    // encode it

    // put it in encoded list

    qWarning() << "Agent" << _id << "starts. Got:" << (img->isNull() ? "NULL" : "IMG");
    int max = 100000000000, i;

    while(i < max)
        ++i;

    qWarning() << "Agent" << _id << "finished.";
    emit Finished(_id);
}

void AgentEncoder::EncodeFrames(QList<QImage> lstFrameToEncode, QList<QImage> lstFrameEncoded){}
void AgentEncoder::EncodeSound(){}
