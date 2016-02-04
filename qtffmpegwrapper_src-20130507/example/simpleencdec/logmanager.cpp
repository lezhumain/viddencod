#include "logmanager.hpp"
#include <qstring.h>
#include <qfile.h>
#include <QDate>
#include <qmutex.h>
LogManager* LogManager::_instance;
LogManager::LogManager()
{
    _logFile = new QFile();
    _mutex = new QMutex();
    _logFile->open();

}
LogManager::~LogManager()
{
    _logFile->close();
    delete _logFile;
    delete QMutex();
    delete _instance;
}
bool log(int idThread,QString message,QString criticite)
{
    QDate date;
    QString dateString = date.currentDate().toString("dd/mm/yyyy");
    QString messageLog = criticite+" "+dateString+" "+idThread+" "+message;
    try
    {
        _logFile->append(messageLog);
        return true;
    }
    catch()
    {
        return false;
    }
}
bool logInfo(int idThread,QString message)
{
    QString criticite ="[INFO]";
    this.log(idThread,message,criticite);
}
bool logWarning(int idThread,QString message)
{
    QString criticite ="[WARNING]";
    this.log(idThread,message,criticite);
}
bool logError(int idThread,QString message)
{
    QString criticite ="[ERROR]";
    this.log(idThread,message,criticite);
}
void LogManager::Kill()
{
    LogManager::_instance->~LogManager();
}
