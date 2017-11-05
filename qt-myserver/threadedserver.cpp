#include "threadedserver.h"
#include "serverthread.h"
#include "mythread.h"

ThreadedServer::ThreadedServer(QObject *parent)
    : QTcpServer(parent)
{
    //int actualThreadCount=100;
    //threadPool = new QThreadPool();
    //threadPool->setMaxThreadCount(actualThreadCount);
    //threadPool->setExpiryTimeout(5000);
    qDebug() << "idealThreadCount";
    qDebug() << QThread::idealThreadCount();
    //qDebug() << "actualThreadCount";
    //qDebug() << actualThreadCount;
    //qDebug() << threadPool->maxThreadCount();
    qDebug() << maxPendingConnections();

}

void ThreadedServer::incomingConnection(qintptr socketDescriptor)
{
    /*qDebug() << "start0";
    //MySocket *mysocket = new MySocket(socketDescriptor, _db);
    //SocketThread *thread = new SocketThread(socketDescriptor,_db);
    //MyThread *thread = new MyThread(socketDescriptor,_db,this);
    qDebug() << "start1";
    QThread *thread = new QThread(mysocket);
    qDebug() << "start2";
    //connect(mysocket,SIGNAL(disconnected()),thread,SLOT(quit()));
    //connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    //connect(mysocket,SIGNAL(disconnected()),mysocket,SLOT(deleteLater()));
    qDebug() << "start3";
    mysocket->moveToThread(thread);
    qDebug() << "start4";
    thread->start();
    //ServerThread *thread = new ServerThread(socketDescriptor, _db);
    //threadPool->start(thread);
    //thread->setAutoDelete(true);
    //qDebug() << threadPool->activeThreadCount();
    //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));*/
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    MyThread *thread = new MyThread(socketDescriptor);
    //ServerSocket *socket = new ServerSocket(socketDescriptor);
    //connect(socket,SIGNAL(newListItem(QString)),this,SIGNAL(newListItem(QString)));
    //connect(socket,SIGNAL(logout()),this,SIGNAL(logout()));

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(newListItem(QString)),this,SIGNAL(newListItem(QString)));
    connect(thread,SIGNAL(logout()),this,SIGNAL(logout()));
    //connect(socket,SIGNAL(newListItem(QString)),this,SIGNAL(newListItem(QString)));
    //connect(socket,SIGNAL(logout()),this,SIGNAL(logout()));

    thread->start();
    //socketpool << socket;
}
