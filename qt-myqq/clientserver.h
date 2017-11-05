#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include <QtNetwork>

class ClientServer : public QObject
{
    Q_OBJECT
public:
    explicit ClientServer(QObject *parent = nullptr);

signals:
    void passMessage(QString msg);

public slots:
    void readMessage();
    void createNewConnection();

private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString message;
    quint16 blockSize;
    //QStringList tokens;

};

#endif // CLIENTSERVER_H
