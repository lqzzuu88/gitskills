#ifndef THREAD1_H
#define THREAD1_H
#include <QtNetwork>
#include <QThread>
#include "accesstime.h"


class Timer1Handler : public QThread, public NetworkAccess
{
    Q_OBJECT

    bool        errorFlag;
    bool        pause;

    QTimer*     timer1;
    int         timerInterval;
    QTcpSocket* tcpSocket;
    QUdpSocket* udpSocket;

    QMutex      mutex;

public:
    Timer1Handler();
    void run();

    void setTimerInterval(int interval);

    void mypause();
    void myresume();

    virtual void clearGoodAccessTime();

    void createTcpConnection();
public slots:
    void action();
    void errorHappend(QAbstractSocket::SocketError e);
    void bytesWritten(qint64 len);
    void udpReadMsg();
};
#endif // THREAD1_H
