#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QtNetwork>
#include "database.h"
#include "threadedserver.h"
#include <QSqlTableModel>
#include "serversocket.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

public slots:


private slots:
    void updateIp();
    void readMessage();
    void respondLogin();
    void respondFind();
    void respondReg();
    void updateClientList(QString onlineInfo);

    void updataLogout();
signals:
    void newLoginEvent();
    void newFindEvent();
    void newRegEvent();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    //DataBase *_db;
    ThreadedServer *_ts;
    QString message;
    quint16 blockSize;
    QStringList tokens;
    QSqlTableModel *model;
    void messageDistribution();
    void respond(const QString & seq);
    QList<ServerSocket*> socketpool;

};

#endif // SERVER_H
