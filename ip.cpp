#include "ip.h"
#include <QtNetwork>


QString get_localmachine_name()
{
    QString machineName     = QHostInfo::localHostName();
    return machineName;
}

QString get_localmachine_ip()
{
    QString localHostName = get_localmachine_name();
    QHostInfo info = QHostInfo::fromName(localHostName);
    //qDebug() <<"IP Address:" <<info.addresses();
    foreach(QHostAddress address,info.addresses())
    {
        //qDebug() << "In info.address:" << address.toString();
        if (address.toString().startsWith("169.")){
            continue; //ignore the IP is not valid
        }
        if (address.isLoopback()){
            continue; //ignore the IP is not valid
        }
        if(address.protocol() == QAbstractSocket::IPv4Protocol) {
            //qDebug() << address.toString();
            return address.toString();
        }
    }
    return QString();
}


QString get_localmachine_mac(QString ip)
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    int i = 0;
    foreach(QNetworkInterface ni,nets)
    {
        i++;
        //qDebug()<<i<<ni.name()<<ni.hardwareAddress()<<ni.humanReadableName();
        QList<QNetworkAddressEntry> entryList = ni.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)
        {
            //qDebug()<<"IP Address:"<<entry.ip().toString();
            if(entry.ip().toString() == ip)
                return ni.hardwareAddress();
        }
    }
    return QString();
}
