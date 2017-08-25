#include "thread1.h"
#include "ip.h"

Timer1Handler::Timer1Handler()
{
    errorFlag    = false;
    pause        = false;
    timer1       = NULL;
    timerInterval = 5000;
    tcpSocket = NULL;
    udpSocket = NULL;
}

void Timer1Handler::run()
{
    timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(action()));
    timer1->start(timerInterval);
    udpSocket = new QUdpSocket;
    udpSocket->bind(8889);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadMsg()));
    QThread::exec();
}

void Timer1Handler::setTimerInterval(int interval)
{
    timerInterval = interval;
}

void Timer1Handler::clearGoodAccessTime()
{
    QMutexLocker lock(&mutex);
    goodAccessTime = 0;
}

void Timer1Handler::mypause()
{
    pause = true;
}

void Timer1Handler::myresume()
{
    pause = false;
}

void Timer1Handler::udpReadMsg()
{
    while(udpSocket/*
          && udpSocket->state() == QAbstractSocket::ConnectedState*/
          && udpSocket->hasPendingDatagrams())  //是否有数据报过来
    {
        QByteArray qdata;
        qdata.resize(udpSocket->pendingDatagramSize());      //数据报大小
        udpSocket->readDatagram(qdata.data(),qdata.size());      //读取
        QString ss;
        ss = QString::fromUtf8(qdata);      //中文
        QTcpSocket* tcp = new QTcpSocket;
        tcp->connectToHost(host_address, conn_port);
        tcp->waitForConnected();
        QString d = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
        QString data = machineName+","+localIP+","+localMac+","+d;
        tcp->write(data.toUtf8());
        tcp->waitForBytesWritten();
        delete tcp;
        qDebug() << "respond to udp";
        //action();
        //if (ss == "Who are you")
        //{
        //    udpSocket->writeDatagram("It's me", QHostAddress("172.26.29.40"), 8889);
        //
        //}
    }
}

void Timer1Handler::action()
{
    if(pause)
        return;

    if(!tcpSocket || newSetIp || errorFlag) {
        createTcpConnection();
        qDebug() << "connect server";
    }
    newSetIp = false;
    errorFlag = false;
    if(tcpSocket/* && tcpSocket->state() == QAbstractSocket::ConnectedState*/) {
        QString d = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
        QString data = machineName+","+localIP+","+localMac+","+d;
        tcpSocket->write(data.toUtf8());
        qDebug() << "write data";
        //tcpSocket->waitForBytesWritten();
    }
}

void Timer1Handler::createTcpConnection()
{
    if(tcpSocket) {
        tcpSocket->close();
        delete tcpSocket;
    }
    tcpSocket = new QTcpSocket;
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorHappend(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    tcpSocket->connectToHost(host_address, conn_port);
    //tcpSocket->waitForConnected();
}

void Timer1Handler::errorHappend(QAbstractSocket::SocketError e)
{
    //delete tcpSocket;
    //tcpSocket = NULL;
    errorFlag = true;
    qDebug() << "error happened";
}

void Timer1Handler::bytesWritten(qint64 len)
{
    QMutexLocker locker(&mutex);
    goodAccessTime++;
}
