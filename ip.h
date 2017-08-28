#ifndef IP_H
#define IP_H
#include <QString>

//local machine name
QString get_localmachine_name();

//local ip
QString get_localmachine_ip();

//local mac address, it depend on ip, you see
QString get_localmachine_mac(QString ip);

#endif // IP_H
