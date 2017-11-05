#include "mainwindow.h"
#include <QApplication>
#include "dialog.h"
#include "solaris.h"
#include <QObject>
#include "publicsocket.h"
#include "receivedfile.h"
#include "sendedfile.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PublicSocket::GetInstance();
    ReceivedFile::GetInstance();
    SendedFile::GetInstance();
    //QTcpSocket *tcpsocket = new QTcpSocket();
    //tcpsocket->connectToHost(QHostAddress::LocalHost,6666);
    ////qintptr socketDescriptor = tcpsocket->socketDescriptor();
    Dialog dlg;
    Solaris w;

    //QObject::connect(tcpsocket,SIGNAL(readyRead()),&dlg,SLOT(recvData()));
    if (dlg.exec() == QDialog::Accepted) {
        w.list->slotAddBuddy(PublicSocket::GetInstance()->getNickName());
        w.getFriendsList();
        w.show();
        //PublicSocket::GetInstance()->sendData("99");
        //PublicSocket::GetInstance()->mySocket->disconnectFromHost();
    }

    return a.exec();
}
