#include "pingthread.h"
#include <QProcess>
#include <QDebug>

PingHandler::PingHandler()
{
    timer = NULL;
    timerInterval = 10 * 1000;
    pause = false;
}

bool PingHandler::ping()
{
    if(host_address.isEmpty())
        return false;

    QProcess *cmd = new QProcess;
    QString strArg = "ping " + host_address + " -n 1 -w " + QString::number(50) ; //windows下的格式
    cmd->start(strArg);
    cmd->waitForReadyRead(300);
    cmd->waitForFinished();
    QString retStr = cmd->readAll();
    delete cmd;
    if (retStr.indexOf("TTL") != -1)
    {
        qDebug() << "shell ping " + host_address + " success";
        return true;
    } else {
        qDebug() << "shell ping " + host_address + " failed";
        return false;
    }
}

void PingHandler::run()
{
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &PingHandler::action);
    timer->start(timerInterval);

    QThread::exec();
}

void PingHandler::setTimerInterval(int interval)
{
    timerInterval = interval;
}

void PingHandler::clearGoodAccessTime()
{
    QMutexLocker lock(&mutex);
    goodAccessTime = 0;
}

void PingHandler::mypause()
{
    pause = true;
}

void PingHandler::myresume()
{
    pause = false;
}

void PingHandler::action()
{
    if(pause)
        return;

    if(ping()) {
        QMutexLocker lock(&mutex);
        goodAccessTime ++;
    }
}
