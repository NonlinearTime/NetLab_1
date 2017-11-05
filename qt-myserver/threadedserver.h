#ifndef THREADEDSERVER_H
#define THREADEDSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QThread>
#include "database.h"
#include "mysocket.h"
#include "socketthread.h"
#include "serversocket.h"

class ThreadedServer : public QTcpServer
{
    Q_OBJECT

public:
    ThreadedServer(QObject *parent = 0);
    QList<ServerSocket*> socketpool;

signals:
    void newListItem(QString onlineInfo);
    void logout();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QThreadPool *threadPool;

};

#endif // THREADEDSERVER_H
