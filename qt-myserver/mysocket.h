#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include "database.h"
#include "offlinemsgpool.h"

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(qintptr socketDescriptor, DataBase *db, QObject *parent = 0);

signals:
    //void error(QTcpSocket::SocketError socketError);
    void newLoginEvent();
    void newFindEvent();
    void newRegEvent();
    void newLoggedinEvent();
    void newContactEvent();

public slots:
    void readMessage();
    void respondLogin();
    void respondFind();
    void respondReg();
    void respondLoggedin();
    void respondContact();

private:
    //int socketDescriptor;
    bool isOnline;
    QString userAcount;
    QTcpSocket *tcpSocket;
    QString message;
    quint16 blockSize;
    QStringList tokens;
    DataBase *_db;
    void messageDistribution();
    void respond(const QString & seq);
    void recvOfflineMsg();
};

#endif // MYSOCKET_H
