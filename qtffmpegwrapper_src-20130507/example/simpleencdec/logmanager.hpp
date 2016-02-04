#pragma once
class Qstring;
class QFile;
class QMutex;

class LogManager
{

public:
    static LogManager* GetInstance();
    static void Kill();
    bool LogWarning(int idThread, QString message);
    bool LogError(int idThread, QString message);
    bool LogInfo(int idThread, QString message);


private:

    ~LogManager();
    LogManager();
    static LogManager *_instance;
    bool log(int idThread, QString message, QString criticite);
    QFile *_logFile;
    QMutex *_mutex;

};

