#ifndef SOLARIS_H
#define SOLARIS_H

#include <QWidget>
#include "personlist.h"
#include <QtNetwork>

namespace Ui {
class Solaris;
}

class Solaris : public QWidget
{
    Q_OBJECT

public:
    explicit Solaris(QWidget *parent = 0);
    ~Solaris();
    QString username;
    PersonList *list;
    void getFriendsList();

signals:
    void showDlgAdd();

private:
    Ui::Solaris *ui;
    QTcpSocket *tcpSocket;
    QHostAddress hostAddress;
    quint16 hostPort;
    quint16 blockSize;
    QString message;
    void newConnection();
    void messageDistribution();
    void sendMessage(const QString & seq);

    QDialog *addFriend;
    QLineEdit *addLineEdit;



private slots:
    void onButtonCloseClicked();
    void onButtonMinClicked();
    void readMessage();
    void read(QString msg);
    void displayError(QAbstractSocket::SocketError);
    void sendAcount();
    void showFndDlg();
};

#endif // SOLARIS_H
