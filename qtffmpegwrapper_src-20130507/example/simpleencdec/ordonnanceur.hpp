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
    ThreadStart();

private slots:
    //Stop agent,supprimer liste,regarder thread si dans liste et si tout finit
    void OnFinished(const short idagent);
private:
    static Ordonnanceur* _instance;

    Fifo<QImage> _fifoFrame;
    QList<QImage> _lstFrameEncoded;
    QList<AgentEncoder*> _lstAgent;
    const short _nbThread;

    static void Kill();

    bool CreateThread();
    int StopThread();
    bool WriteVideo();

    ~Ordonnanceur();
    Ordonnanceur(const short nbThread);

public:
    static Ordonnanceur* GetInstance(const short nbThread = 0);

    void PushFrameToFifo(QImage frame);
    QImage PopFrame();
    void ClearFifo();
    unsigned int GetFifoLength() const;
    int StartThread();


};

#endif // ORDONNANCEUR_H
