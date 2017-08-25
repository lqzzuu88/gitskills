#ifndef THREAD2_H
#define THREAD2_H

#include <QThread>
#include <QTimer>
#include "accesstime.h"

class Timer1Handler;
class PingHandler;

class Timer2Handler : public QThread
{
    Q_OBJECT

    NetworkAccess*  tcphandler; //Timer1Handler
    //NetworkAccess* secondary; //Timer1Handler
    NetworkAccess*  pinghandler; //PingHandler
    NetworkAccess*  pingself; //PingHandler

    QTimer*        timer2;
    int            timerInterval;

    bool           doRemoveFile;
    bool           doShutDown;
public:
    Timer2Handler();

    void setTcpHandler(Timer1Handler* handler);
    void setPingGatewayHandler(PingHandler* handler);
    void setPingSelfHandler(PingHandler* handler);

    void run();

    void shutdown();

    void setTimerInterval(int interval);

    void setRemoveFile(bool flag);
    void setShutDown(bool flag);

public slots:
    void action();
};


#endif // THREAD2_H
