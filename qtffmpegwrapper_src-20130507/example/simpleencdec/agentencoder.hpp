#ifndef AGENTENCODER_H
#define AGENTENCODER_H

#include <QObject>
#include <QList>
#include <QImage>

class AgentEncoder : public QObject
{
    Q_OBJECT
public:
    explicit AgentEncoder(QObject *parent = 0);

signals:

public slots:
    void EncodeFrames(QList<QImage> lstFrameToEncode, QList<QImage> lstFrameEncoded);
    void EncodeSound();
};

#endif // AGENTENCODER_H
