#include "threadedserver.h"
#include "serverthread.h"

ThreadedServer::ThreadedServer(QObject *parent)
    : QTcpServer(parent)
{
    threadPool = new QThreadPool(this);
    threadPool->setMaxThreadCount(QThread::idealThreadCount());
    threadPool->setExpiryTimeout(5000);
    qDebug() << QThread::idealThreadCount();

}

void ThreadedServer::incomingConnection(qintptr socketDescriptor)
{
    ServerThread *thread = new ServerThread(socketDescriptor, this);
    threadPool->start(thread);
    thread->setAutoDelete(true);
    //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}
