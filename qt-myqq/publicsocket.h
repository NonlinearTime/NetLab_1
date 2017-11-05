#ifndef PUBLICSOCKET_H
#define PUBLICSOCKET_H

#include <QtNetwork>
#include <mysocket.h>

class PublicSocket
{
public:
    static PublicSocket* GetInstance();
    static void Release();
    void sendData(const QString &msg);
    //void readData(const QString &msg);
    QStringList getNickName();
    void setNickName(QStringList info);
    QHostAddress getClientIP();
    void setClientIP(QHostAddress IP);
    quint16 getClientPort();
    void setClientPort(quint16 port);
    MySocket *mySocket;

private:
    PublicSocket();
    //MySocket *mySocket;
    static PublicSocket *pInstance;
    QString nickName;
    QString sign;
    QString acount;
    QHostAddress clientServerIP;
    quint16 clientServerPort;

};

#endif // PUBLICSOCKET_H
