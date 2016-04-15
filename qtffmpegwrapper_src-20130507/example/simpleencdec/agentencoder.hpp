#ifndef AGENTENCODER_H
#define AGENTENCODER_H

#include <QObject>
#include <QList>
#include <QImage>

class Ordonnanceur;

class AgentEncoder : public QObject
{
    Q_OBJECT
public:
    explicit AgentEncoder(const short id, QObject *parent = 0);
    AgentEncoder(QObject *parent);
    ~AgentEncoder();
    int getId() const{return _id;}

signals:
    int Finished(const short id);

public slots:
    void Run();

private:
    Ordonnanceur* _ordo; // dont delete
    const short _id;
    void EncodeFrames(QList<QImage> lstFrameToEncode, QList<QImage> lstFrameEncoded);
    void EncodeSound();

};

#endif // AGENTENCODER_H
