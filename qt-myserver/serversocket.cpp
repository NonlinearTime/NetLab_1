#include "serversocket.h"
#include <iostream>
#include <QDateTime>

ServerSocket::ServerSocket(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    tcpSocket = socket;

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()), Qt::DirectConnection);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socketDescriptor << " Client connected";
}

void ServerSocket::readyRead()
{
    // get the information
    QByteArray Data = tcpSocket->readAll();

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << Data;

    tcpSocket->write(Data);
}

void ServerSocket::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    tcpSocket->deleteLater();
}

void ServerSocket::respond(const QString &seq)
{
    //QByteArray block;
    //QDataStream out(&block,QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_9);
    //out<<(quint16) 0 ;
    //out << seq;
    //qDebug() << tr("log");
    //qDebug() << block;
    //out.device()->seek(0);
    //out<<(quint16) (block.size() - sizeof(quint16));
    //qDebug() << out;
    qDebug() << "respond";
    tcpSocket->write(seq.toUtf8());
    tcpSocket->flush();
}

void ServerSocket::recvOfflineMsg()
{
    QString piece;
    for (int i = 3; i < tokens.size() ; i++) {
        if (i == 3) piece.append(tokens[i]);
        else piece.append(QString(" ") + tokens[i]);
    }
    qDebug() << piece;
    OffLineMsgPool::GetInstance()->addMsgPiece(tokens[1],tokens[2],piece);
}

void ServerSocket::recvLogout()
{
    isOnline = false;
    DataBase::GetInstance()->itemUpdate(QString("isonline"),QString("0"),QString("acount"),userAcount);
    emit logout();
}

void ServerSocket::readMessage()
{
    qDebug() << "read begin";
    //QDataStream in(tcpSocket);
    //in.setVersion(QDataStream::Qt_5_9);
    qDebug() << "reading.";
    //if(blockSize == 0) {
    //    if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
    //    in >> blockSize;
    //}
    qDebug() << "reading..";
    //if(tcpSocket->bytesAvailable() < blockSize) return;
    //in >> message;
    message = QString(tcpSocket->readAll());
    qDebug() << "reading...";
    qDebug() << message;
    messageDistribution();
    qDebug() << socketDescriptor << " Data in: " << message;
    qDebug() << tokens;
    blockSize = 0;
    //emit logout();
}

void ServerSocket::respondLogin()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[1]);
    query.next();
    qDebug() << !query.isNull(QString("acount"));
    qDebug() << query.value(0).toString();
    if (!query.isNull(QString("acount")) && (tokens[2] == query.value(1).toString())) {
        respond(QString("0 1 %1 %2 %3").arg(query.value(2).toString()).arg(query.value(3).toString()).arg(query.value(0).toString()));
        DataBase::GetInstance()->itemUpdate(QString("ipaddress"),tcpSocket->peerAddress().toString() , QString("acount"),tokens[1]);
        DataBase::GetInstance()->itemUpdate(QString("port"),QString::number(tcpSocket->peerPort(),10),QString("acount"),tokens[1]);
        DataBase::GetInstance()->itemUpdate(QString("isonline"),QString("1"),QString("acount"),tokens[1]);
        DataBase::GetInstance()->itemUpdate(QString("listenIp"),tokens[3],QString("acount"),tokens[1]);
        DataBase::GetInstance()->itemUpdate(QString("listenport"),tokens[4],QString("acount"),tokens[1]);
        userAcount = tokens[1];
        emit newListItem(QString("%1 %2 %3").arg(query.value(0).toString()).arg(query.value(2).toString()).arg(tcpSocket->peerAddress().toString()));
        //QThread::sleep(1000);
        //respond(QString("10 %1 %2").arg(query.value(2).toString()).arg(query.value(3).toString()));
    }
    else respond(QString("0 0"));

    qDebug() << "0";
}

void ServerSocket::respondReg()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[1]);
    //qDebug() << query.value(1);
    query.next();
    if (!query.isNull(QString("acount"))) {
        respond(QString("1 2"));
    } else {
        QStringList info;
        info << tokens[1]
                << tokens[2]
                << tokens[6]
                << tokens[7]
                << tokens[4]
                << tokens[5]
                << QString("")
                << QString("")
                << QString("0")
                << QString("")
                << QString("")
                << QString("");

        DataBase::GetInstance()->itemInsert(info);
        respond(QString("1 1"));
    }
    qDebug() << "1";
}

//添加好友
void ServerSocket::respondLoggedin()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[1]);
    QSqlQuery q = DataBase::GetInstance()->itemQuery(QString("acount"),userAcount);
    q.next();
    query.next();
    if (query.isNull(QString("acount"))) {
        respond(QString("11 0"));
    }
    else {
        respond(QString("11 1 %1 %2 %3").arg(query.value(2).toString()).arg(query.value(3).toString()).arg(query.value(0).toString()));

        DataBase::GetInstance()->itemUpdate(QString("friendslist"),q.value(11).toString() + QString(" ") + tokens[1],QString("acount"),userAcount);
        emit logout();
    }
    qDebug() << "addFriend";

}

void ServerSocket::respondContact()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[1]);
    query.next();
    if (query.value(8) == "1") {
        respond(QString("10 1 %1 %2").arg(query.value(9).toString()).arg(query.value(10).toString()));
        qDebug() << "con1";
    } else {
        respond(QString("10 0"));
        qDebug() << "con0";
    }
    OffLineMsgPool::GetInstance()->insertUser(tokens[1],tokens[2]);
    OffLineMsgPool::GetInstance()->insertUser(tokens[2],tokens[1]);
    QString seq = "20 ";
    QChar c = 1;
    for(int i = 0 ; i < OffLineMsgPool::GetInstance()->getOfflineMsg(tokens[2],tokens[1])->size(); i++) {
        seq = seq + (*OffLineMsgPool::GetInstance()->getOfflineMsg(tokens[2],tokens[1]))[i] + QString("%1").arg(c);
    }
    qDebug() << seq;
    respond(seq);
}

void ServerSocket::respondFind()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[3]);
    //qDebug() << query.value(1);
    query.next();
    if (query.isNull(QString("acount"))) {
        respond(QString("2 3"));
    } else {
        if (tokens[2] != query.value(5) || tokens[1] != query.value(4)) {
            respond(QString("2 2"));
        } else {
            DataBase::GetInstance()->itemUpdate(QString("password"),tokens[4],QString("acount"),tokens[3]);
            respond(QString("2 1"));
        }
    }
    qDebug() << "2";
}

void ServerSocket::respondReqOfFiendsList()
{
    qDebug() << userAcount;
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),userAcount);
    query.next();
    qDebug() <<query.value(11).toString();
    QStringList friendsList;
    friendsList = query.value(11).toString().split(" ",QString::SkipEmptyParts);
    qDebug() << userAcount;
    qDebug() << friendsList;
    QString seq = QString("12 ");
    QChar c = 1;
    for (int i = 0 ; i < friendsList.size() ; i++) {
        QSqlQuery &q = DataBase::GetInstance()->itemQuery(QString("acount"),friendsList[i]);
        qDebug() << q.isNull(QString("acount"));
        q.next();
        seq  = seq + q.value(2).toString() + QString(" ")
                   + q.value(3).toString() + QString(" ")
                   + q.value(0).toString() + QString("%1").arg(c);

    }
    qDebug() << seq;
    respond(seq);
}

void ServerSocket::messageDistribution()
{
    tokens= message.split(" ",QString::SkipEmptyParts);
    qDebug() << tokens[0].toInt();
    switch (tokens[0].toInt()) {
    case 0:
        respondLogin();
        break;
    case 1:
        respondReg();
        break;
    case 2:
        respondFind();
        break;
    case 10:
        respondContact();
        break;
    case 11:
        respondLoggedin();
        break;
    case 12:
        respondReqOfFiendsList();
        break;
    case 20:
        recvOfflineMsg();
        break;
    case 99:
        recvLogout();
        break;
    default:
        break;
    }
    //qDebug() << tokens[0].toInt();
}
