#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QtNetwork>
#include "database.h"
#include "offlinemsgpool.h"

class ServerThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ServerThread(int socketDescriptor, DataBase *db, QObject *parent = nullptr);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
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
    int socketDescriptor;
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

#endif // SERVERTHREAD_H
