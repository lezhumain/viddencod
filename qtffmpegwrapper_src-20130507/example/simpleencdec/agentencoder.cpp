#include "agentencoder.hpp"
#include <QDebug>


AgentEncoder::AgentEncoder(const short id, QObject *parent) :
    QObject(parent),
    _id(id)
{
    qWarning() << "Agent" << _id << "created.";
}

AgentEncoder::~AgentEncoder()
{
    qWarning() << "Agent" << _id << "destroyed";
}

void AgentEncoder::EncodeFrames(QList<QImage> lstFrameToEncode, QList<QImage> lstFrameEncoded)
{

}

void AgentEncoder::EncodeSound()
{

}

// Declenchee quand l'ordo envoi le signal 'Start'
void AgentEncoder::Run()
{
    // get frame from fifo

    // encode it

    // put it in encoded list

    qWarning() << "Agent" << _id << "starts.";
    int max = 1000000000, i;

    while(i < max)
        ++i;

    qWarning() << "Agent" << _id << "finished.";
    emit Finished(_id);
}
