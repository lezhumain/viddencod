#ifndef AGENT_H
#define AGENT_H

#include <QObject>

class Agent : public QObject
{
    Q_OBJECT
public:
    explicit Agent(QObject *parent = 0);

signals:

public slots:
};

#endif // AGENT_H