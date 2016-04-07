#ifndef ORDONNANCEUR_H
#define ORDONNANCEUR_H
#include <QImage>
#include <QObject>
#include <QList>
#include "fifo.hpp"
#include "agentEncoder.hpp"


class Ordonnanceur : public QObject
{
    Q_OBJECT
signals:
private slots:
    //Stop agent,supprimer liste,regarder thread si dans liste et si tout finit
    void OnFinished(int idagent);
private:
    Fifo _fifoFrame<QImage>;
    QList<QImage> _lstFrameEncoded;
    QList<AgentEncoder> _lstAgent;
    bool CreateThread();
    int StartThread();
    int StopThread();
    bool WriteVideo();

public:
    Ordonnanceur();
    void PushFrameToFifo(QImage frame);
    QImage PopFrame();
    void ClearFifo();
    unsigned int GetFifoLength();


};

#endif // ORDONNANCEUR_H
