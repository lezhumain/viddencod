#include "logmanager.hpp"
#include <qstring.h>
#include <qfile.h>
#include <QDate>
#include <qmutex.h>
#include <qtextstream.h>
LogManager* LogManager::_instance;
LogManager::LogManager()
{
    _logFile = new QFile("C:\\User\\lucdef\\Desktop\\log.txt");
    _mutex = new QMutex();
    _logFile->open(QIODevice::Append);

}
LogManager::~LogManager()
{
    _logFile->close();
    delete _logFile;
    delete _mutex;
    delete _instance;
}
bool LogManager::log(int idThread,QString message,QString criticite)
{
    QDate date;
    bool isFlush;
    _mutex->lock();
    QTextStream txtStream(_logFile);
    QString dateString = date.currentDate().toString("dd/mm/yyyy");
    QString messageLog = criticite+" "+dateString+" "+idThread+" "+message;


        txtStream<<messageLog<<endl;
         isFlush = _logFile->flush();
         _mutex->unlock();

        return isFlush;

}
bool LogManager::LogInfo(int idThread,QString message)
{
    QString criticite ="[INFO]";
    return this->log(idThread,message,criticite);
}
bool LogManager::LogWarning(int idThread,QString message)
{
    QString criticite ="[WARNING]";
    return this->log(idThread,message,criticite);
}
bool LogManager::LogError(int idThread,QString message)
{
    QString criticite ="[ERROR]";
    return this->log(idThread,message,criticite);
}
void LogManager::Kill()
{
    LogManager::_instance->~LogManager();
}
