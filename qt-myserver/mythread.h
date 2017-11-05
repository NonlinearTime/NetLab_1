#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <database.h>
#include <QObject>
#include <QtNetwork>
#include "offlinemsgpool.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID, QObject *parent = 0);

    void run();


signals:
    void error(QTcpSocket::SocketError socketerror);
    void newListItem(QString onlineInfo);
    void logout();

public slots:
    void readyRead();
    void disconnected();
    void readMessage();
    void respondLogin();
    void respondFind();
    void respondReg();
    void respondLoggedin();
    void respondContact();
    void respondReqOfFiendsList();

private:
    QTcpSocket *tcpSocket;
    qintptr socketDescriptor;

    bool isOnline;
    QString userAcount;
    QString message;
    quint16 blockSize;
    QStringList tokens;
    void messageDistribution();
    void respond(const QString & seq);
    void recvOfflineMsg();
    void recvLogout();




};

#endif // MYTHREAD_H
