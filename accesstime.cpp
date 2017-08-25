#include "accesstime.h"
#include "ip.h"

NetworkAccess::NetworkAccess()
{
    host_address = "";
    conn_port    = 8888;
    newSetIp     = false;

    goodAccessTime = 0;

    machineName = get_localmachine_name();
    localIP = get_localmachine_ip();
    localMac = get_localmachine_mac(localIP);
}

void NetworkAccess::setHostAddress(const QString& ip)
{
    newSetIp = true;
    host_address = ip;
}

QString NetworkAccess::getHostAddress()
{
    return host_address;
}

int NetworkAccess::getGoodAccessTime()
{
    return goodAccessTime;
}

void NetworkAccess::clearGoodAccessTime()
{
    goodAccessTime = 0;
}

QString NetworkAccess::getLocalIP()
{
    return localIP;
}



