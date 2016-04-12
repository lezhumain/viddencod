#ifndef AGENT_H
#define AGENT_H
#include <QList>
#include <QImage>

#include <QObject>

class AgentEncoder : public QObject
{
    Q_OBJECT
public:
    explicit AgentEncoder(QObject *parent = 0);

signals:

public slots:
    void EncoderFrames(QList<QImage> lstFrameToEncode,QList<QImage>&lstFrameEncoded);
    void EncodeSound();
};

#endif // AGENT_H
