#ifndef ORDONNANCEUR_H
#define ORDONNANCEUR_H

#include <QImage>
#include <QObject>
#include <QList>
#include "fifo.hpp"
#include "agentEncoder.hpp"

#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"
#include "../../QTFFmpegWrapper/headers/QVideoDecoder.h"

class Ordonnanceur : public QObject
{
    Q_OBJECT

public:
    struct frame_t {
        QImage frame;
        int index;
    };

    static Ordonnanceur* GetInstance(const short nbThread = 0);

    void PushFrameToFifo(frame_t frame);
    frame_t PopFrame();
    void ClearFifo();
    unsigned int GetFifoLength() const;
    int StartThread();
    bool Start();

signals:
    ThreadStart();

private slots:
    //Stop agent,supprimer liste,regarder thread si dans liste et si tout finit
    void OnFinished(const short idagent);
private:
    static Ordonnanceur* _instance;

    Fifo<frame_t> _fifoFrame;
    QList<frame_t> _lstFrameEncoded;
    QList<AgentEncoder*> _lstAgent;
    const short _nbThread;
    QString _filename;
    QVideoDecoder m_decoder;
    QVideoEncoder m_encoder;

    static void Kill();

    bool CreateThread();
    int StopThread();
    bool WriteVideo();

    ~Ordonnanceur();
    Ordonnanceur(const short nbThread, const QString &filename = "./test.avi");
    void loadVideo(QString fileName);
    void displayFrame();
    bool checkVideoLoadOk();
    void image2Pixmap(QImage &img,QPixmap &pixmap);
    QList<QImage> getAllFrames();
    bool nextFrame();



};

#endif // ORDONNANCEUR_H
