#ifndef PINGTHREAD_H
#define PINGTHREAD_H
#include "accesstime.h"
#include <QThread>
#include <QMutex>
#include <QTimer>

class PingHandler : public QThread, public NetworkAccess
{
    QTimer*  timer;
    int      timerInterval;
    bool     pause;

    QMutex   mutex;

public:
    PingHandler();

    void run();

    bool ping();
    void mypause();
    void myresume();
    void setTimerInterval(int interval);

    virtual void clearGoodAccessTime();

public slots:
    void action();
};

#endif // PINGTHREAD_H
