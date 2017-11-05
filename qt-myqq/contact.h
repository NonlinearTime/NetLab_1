#ifndef CONTACT_H
#define CONTACT_H

#include <QWidget>
#include <QtNetwork>
#include <clientserver.h>
#include <QLabel>
#include <QFileDialog>
#include <QFile>

#include <receicer.h>
#include <receiverform.h>
#include <receivedfile.h>

#include <sender.h>
#include <sendedfile.h>
#include <senderform.h>
#include <readthread.h>

#include <QThread>

namespace Ui {
class Contact;
}

class Contact : public QWidget
{
    Q_OBJECT

public:
    explicit Contact(QWidget *parent = 0);
    ~Contact();
    //Ui::Contact *ui;
    QLabel *signLabel;
    QString localAcount;
    bool isOnline;
    void setHostInfo(QHostAddress adrs,qint16 port);
    void newConnection();
    //ReceiverForm receiverform;
    //SenderForm senderform;

private:
    Ui::Contact *ui;
    ClientServer *clientServer;
    QStringList messages;
    QHostAddress hostAddress;
    qint16 hostPort;
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    QString message;
    QString filename;
    void messageDistribution();
    void displayMessage(QString msg,bool isOnlineMsg);

signals:
    void sendstart();

public slots:
    void onlineMessage(QString msg);

private slots:
    void onButtonMinClicked();
    void onButtonCloseClicked();
    void displayError(QAbstractSocket::SocketError);
    //void readMessage();
    void sendMessage();
    void read(QString msg);

    void fileCancled();

    void on_fileButton_clicked();
};

#endif // CONTACT_H
