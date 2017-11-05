#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include "database.h"
#include "offlinemsgpool.h"
#include "mysocket.h"

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr socketDescriptor, DataBase *db, QObject *parent = 0);
protected:
    void run();
private:
    MySocket *mySocket;
    DataBase *_db;
    qintptr socketDescriptor;
};

#endif // SOCKETTHREAD_H
