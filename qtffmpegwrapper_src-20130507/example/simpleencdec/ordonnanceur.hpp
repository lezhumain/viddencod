#ifndef ORDONNANCEUR_H
#define ORDONNANCEUR_H

#include <QImage>
#include <QObject>
#include <QList>
#include "fifo.h"
#include "agentencoder.hpp"

#include "../../QTFFmpegWrapper/headers/QVideoEncoder.h"
#include "../../QTFFmpegWrapper/headers/QVideoDecoder.h"

class Ordonnanceur : public QObject
{
    Q_OBJECT

public:
    struct frame_t {
        QImage frame;
        int eframeNumbern;
        int frameTime;
    };

    static Ordonnanceur* GetInstance(const short nbThread = 0);
    static void Kill();


    void PushFrameToFifo(frame_t frame);
    frame_t PopFrame();
    void ClearFifo();
    unsigned int GetFifoLength() const;
    int StartThread();
    bool Start();
    bool EncodeVideo(QString filename);
    bool loadVideo(QString fileName);
    bool checkVideoLoadOk();
    int GenerateEncodedVideo(QString filename, bool vfr);

signals:
    int ThreadStart();

private slots:
    //Stop agent,supprimer liste,regarder thread si dans liste et si tout finit
    void OnFinished(const short idagent);
private:
    static Ordonnanceur* _instance;

    Fifo<frame_t> _fifoFrame;
    QList<frame_t> _lstFrameToBeEncoded;
    QList<frame_t> _lstFrameEncoded;
    QList<AgentEncoder*> _lstAgent;
    const short _nbThread;
    QString _filename;
    QVideoDecoder m_decoder;
    QVideoEncoder m_encoder;
    ffmpeg::AVRational m_FrameRateDecodedVideo;
    long m_NbFramesDecodedVideo;

    Ordonnanceur(const short nbThread, const QString &filename = "../../videos/short_video.mp4");
//    Ordonnanceur(const short nbThread, const QString &filename = "../../videos/B99.mp4");
//    Ordonnanceur(const short nbThread, const QString &filename = "../../videos/Humour.mp4");
    ~Ordonnanceur();

    bool CreateThread();
    int StopThread();
//    bool WriteVideo(frame_t sframe);
    bool WriteVideo(frame_t sframe, int iFrame);
    bool displayFrame();
    void image2Pixmap(QImage &img,QPixmap &pixmap);
    QList<frame_t> getAllFrames();
    bool loadAllFrames();
    bool nextFrame();
};

#endif // ORDONNANCEUR_H
