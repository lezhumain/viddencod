#include <qstring.h>
#include <qfile.h>
#include <QDate>
#include <QMutex>
#include <QMutexLocker>
#include <qtextstream.h>
#include <QDebug>
#include "logmanager.hpp"

LogManager* LogManager::_instance = NULL;

LogManager::LogManager()
{
    _logFile = new QFile(".\\Videncode.log");
}

LogManager::~LogManager()
{
    if(_logFile->isOpen())
        _logFile->close();

    delete _logFile;
    delete _instance;
}

LogManager* LogManager::GetInstance()
{
    if(_instance == NULL)
        _instance = new LogManager();

    return _instance;
}

bool LogManager::log(int idThread,QString message,QString criticite, bool debug)
{
    QDate date;
    bool isFlush;
    {
        QMutexLocker locker(&_mutex);
//        QTextStream txtStream(_logFile);
        QString dateString = date.currentDate().toString("dd/MM/yyyy"),
                messageLog = criticite + " " + dateString + " " + QString::number(idThread) + " - " + message + "\n";

//        txtStream << messageLog << endl;
//        isFlush = _logFile->flush();

        _logFile->open(QIODevice::Append);
        _logFile->write(messageLog.toUtf8());
        _logFile->close();

        if(debug)
            qWarning() << messageLog;
    } // ici locker est detruit, donc le mutex est release

    return isFlush;
}
bool LogManager::LogInfo(int idThread,QString message, bool debug)
{
    QString criticite ="[INFO]";
    return this->log(idThread,message,criticite, debug);
}
bool LogManager::LogWarning(int idThread,QString message, bool debug)
{
    QString criticite ="[WARNING]";
    return this->log(idThread,message,criticite, debug);
}
bool LogManager::LogError(int idThread,QString message, bool debug)
{
    QString criticite ="[ERROR]";
    return this->log(idThread,message,criticite, debug);
}
void LogManager::Kill()
{
    LogManager::_instance->~LogManager();
}
