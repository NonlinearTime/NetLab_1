#include "socketthread.h"
#include <iostream>
#include <QDateTime>

SocketThread::SocketThread(qintptr socketDescriptor, DataBase *db, QObject *parent)
    :QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
    this->_db = db;
}

void SocketThread::run()
{
    mySocket = new MySocket(socketDescriptor,_db);
    connect(mySocket,SIGNAL(disconnected()),mySocket,SLOT(deleteLater()));

    QThread::sleep(1);
    this->exec();
}
