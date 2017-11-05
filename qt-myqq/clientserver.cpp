#include "clientserver.h"
#include "publicsocket.h"

ClientServer::ClientServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any,PublicSocket::GetInstance()->getClientPort())) {
        qDebug() << tcpServer->errorString();
        //close();
    }
    qDebug() << tcpServer->isListening();
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(createNewConnection()));
}

void ClientServer::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_9);
    if(blockSize == 0) {
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
    qDebug() << message;
    emit passMessage(message);
    blockSize = 0;
}

void ClientServer::createNewConnection()
{
    blockSize = 0;
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}
