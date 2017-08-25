#ifndef ACCESSTIME_H
#define ACCESSTIME_H

#include <QString>

class NetworkAccess
{
protected:
    QString host_address;
    int     conn_port;
    bool    newSetIp;

    int     goodAccessTime;

    QString machineName;
    QString localIP;
    QString localMac;

public:
    NetworkAccess();

    virtual void setHostAddress(const QString& ip);
    virtual QString getHostAddress();

    virtual int getGoodAccessTime();
    virtual void clearGoodAccessTime();

    QString getLocalIP();

};

#endif // ACCESSTIME_H
