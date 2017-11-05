#include "publicsocket.h"

PublicSocket* PublicSocket::pInstance = NULL;

PublicSocket *PublicSocket::GetInstance()
{
    if(pInstance == NULL) {
        pInstance = new PublicSocket();
        atexit(Release);
    }
    return pInstance;
}

void PublicSocket::Release()
{
    if (pInstance) {
        delete pInstance;
        pInstance = NULL;
    }
}

void PublicSocket::sendData(const QString &msg)
{
    mySocket->sendMessage(msg);
}

QStringList PublicSocket::getNickName()
{
    QStringList info;
    info << nickName << sign << acount;
    return info;
}

void PublicSocket::setNickName(QStringList info)
{
    nickName = info[0];
    sign = info[1];
    acount = info[2];
}

QHostAddress PublicSocket::getClientIP()
{
    return clientServerIP;
}

void PublicSocket::setClientIP(QHostAddress IP)
{
    clientServerIP = IP;
}

quint16 PublicSocket::getClientPort()
{
    return clientServerPort;
}

void PublicSocket::setClientPort(quint16 port)
{
    clientServerPort = port;
}

PublicSocket::PublicSocket()
{
    mySocket = new MySocket();
    //mySocket->connectToHost(QHostAddress::LocalHost,6666);
}
