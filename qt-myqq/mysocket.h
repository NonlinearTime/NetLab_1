#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
//#include <QObject>

class MySocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit MySocket(QObject *parent = nullptr);
    void sendMessage(QString msg);

private:
    QString message;
    quint16 blockSize;


signals:
    void recvDlgMsg(QString msg);
    void recvSolMsg(QString msg);
    void recvConMsg(QString msg);

public slots:
    void messageDistribution();
    void readMessage();

};

#endif // MYSOCKET_H
