#ifndef THREADEDSERVER_H
#define THREADEDSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include "database.h"

class ThreadedServer : public QTcpServer
{
    Q_OBJECT

public:
    ThreadedServer(QObject *parent = 0);

signals:


protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QThreadPool *threadPool;

};

#endif // THREADEDSERVER_H
