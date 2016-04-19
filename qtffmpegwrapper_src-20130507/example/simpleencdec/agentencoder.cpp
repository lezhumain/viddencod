#include "agentencoder.hpp"
#include <QDebug>
#include "ordonnanceur.hpp"
#include "logmanager.hpp"

AgentEncoder::AgentEncoder(const short id, QObject *parent) :
    QObject(parent),
    _id(id)
{
    _ordo = reinterpret_cast<Ordonnanceur*>(parent);
    LogManager::GetInstance()->LogInfo(0, "Agent " + QString::number(_id) + " created.", true);
}

AgentEncoder::~AgentEncoder()
{
    LogManager::GetInstance()->LogInfo(0, "Agent " + QString::number(_id) + " destroyed.", true);
}

// Declenchee quand l'ordo envoi le signal 'Start'
void AgentEncoder::Run()
{
    LogManager::GetInstance()->LogInfo(0, "Agent " + QString::number(_id) + " started.", true);

    if(_ordo == NULL)
    {
        LogManager::GetInstance()->LogInfo(0, "\t /!\ Ordo was NULL", true);
        _ordo = Ordonnanceur::GetInstance();
    }
    // get frame from fifo
    Ordonnanceur::frame_t frame = _ordo->PopFrame();
    QImage *img = &(frame.frame);

    // encode it

    // put it in encoded list

    LogManager::GetInstance()->LogInfo(0, "Agent " + QString::number(_id) + " got: " + (img->isNull() ? "NULL" : "IMG"), true);
    int max = 100000000000, i;

    while(i < max)
        ++i;

    LogManager::GetInstance()->LogInfo(0, "Agent " + QString::number(_id) + " finished.", true);
    emit Finished(_id);
}

void AgentEncoder::EncodeFrames(QList<QImage> lstFrameToEncode, QList<QImage> lstFrameEncoded){}
void AgentEncoder::EncodeSound(){}
