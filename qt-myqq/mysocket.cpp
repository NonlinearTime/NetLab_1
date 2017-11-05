#include "mysocket.h"
#include <iostream>
#include <QtNetwork>


MySocket::MySocket(QObject *parent):
    QTcpSocket(parent)
{
    blockSize = 0;
    connectToHost(QHostAddress::LocalHost,6666);
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void MySocket::sendMessage(QString msg)
{
    //QByteArray block;
    //QDataStream out(&block,QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_9);
    //out<<(quint16) 0 ;
    //out << msg;
    qDebug() << msg;
    qDebug() << "beginsend";
    //qDebug() << block;
    //out.device()->seek(0);
    //out<<(quint16) (block.size() - sizeof(quint16));
    write(msg.toUtf8());
    flush();
    qDebug() << "endsend";


}

void MySocket::messageDistribution()
{
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);
    int index = tokens[0].toInt();
    if (index < 10) {emit recvDlgMsg(message); qDebug() << "Dlg";}
    else if (index <20) {emit recvSolMsg(message); qDebug() << "Sol";}
    else if (index <30) emit recvConMsg(message);
}

void MySocket::readMessage()
{
    //QDataStream in(this);
    //in.setVersion(QDataStream::Qt_5_9);
    //if(blockSize == 0) {
    //    if(this->bytesAvailable() < (int)sizeof(quint16)) return;
    //    in >> blockSize;
    //}
    //if(this->bytesAvailable() < blockSize) return;
    //in >> message;
    qDebug() << "read.";
    message = QString(readAll());
    qDebug() << message;
    qDebug() << "read...";
    //blockSize = 0;
    messageDistribution();
}


