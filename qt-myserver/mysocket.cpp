#include "mysocket.h"
#include <iostream>
#include <QDateTime>

MySocket::MySocket(qintptr socketDescriptor, DataBase *db, QObject *parent)
    :QTcpSocket(parent), _db(db)
{
    isOnline = true;
    //与Tcpserver绑定
    if (!this->setSocketDescriptor(socketDescriptor)) {
        //emit error(this->errorString());
        qDebug() << error();
        return;
    }

    qDebug() << socketDescriptor;

    //绑定事件分发信号与槽
    connect(this,SIGNAL(newLoginEvent()),this,SLOT(respondLogin()));
    connect(this,SIGNAL(newFindEvent()),this,SLOT(respondFind()));
    connect(this,SIGNAL(newRegEvent()),this,SLOT(respondReg()));
    connect(this,SIGNAL(newLoggedinEvent()),this,SLOT(respondLoggedin()));
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()),Qt::DirectConnection);
    //connect(this,SIGNAL(newContactEvent()),SLOT(respondContact()));
}

void MySocket::respond(const QString &seq)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<(quint16) 0 ;
    out << seq;
    qDebug() << tr("respond start");
    out.device()->seek(0);
    qDebug() << "respoding";
    out<<(quint16) (block.size() - sizeof(quint16));
    //qDebug() << out;

    this->write(block);
    this->flush();
    qDebug() << "responded";
}

void MySocket::recvOfflineMsg()
{
    QString piece;
    for (int i = 3; i < tokens.size() ; i++) {
        if (i == 3) piece.append(tokens[i]);
        else piece.append(QString(" ") + tokens[i]);
    }
    qDebug() << piece;
    OffLineMsgPool::GetInstance()->addMsgPiece(tokens[1],tokens[2],piece);
}

void MySocket::readMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_9);
    if(blockSize == 0) {
        if(this->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if(this->bytesAvailable() < blockSize) return;
    in >> message;
    qDebug() << message;
    messageDistribution();
    qDebug() << tokens;
    //if (message == QString("Haines") + QString("123")) {
    //    respondIsTrue(true);
    //} else respondIsTrue(false);
    blockSize = 0;
}

void MySocket::respondLogin()
{
    QSqlQuery query = _db->itemQuery(QString("acount"),tokens[1]);
    query.next();
    qDebug() << !query.isNull(QString("acount"));
    qDebug() << query.value(0).toString();
    if (!query.isNull(QString("acount")) && (tokens[2] == query.value(1).toString())) {
        respond(QString("0 1 %1 %2 %3").arg(query.value(2).toString()).arg(query.value(3).toString()).arg(query.value(0).toString()));
        qDebug() << "rpndLogin1";
        _db->itemUpdate(QString("ipaddress"),this->peerAddress().toString() , QString("acount"),tokens[1]);
        qDebug() << "rpndLogin2";
        _db->itemUpdate(QString("port"),QString::number(this->peerPort(),10),QString("acount"),tokens[1]);
        qDebug() << "rpndLogin3";
        _db->itemUpdate(QString("isonline"),QString("1"),QString("acount"),tokens[1]);
        qDebug() << "rpndLogin4";
        userAcount = tokens[1];
        qDebug() << "rpndLogin5";
        //QThread::sleep(1000);
        //respond(QString("10 %1 %2").arg(query.value(2).toString()).arg(query.value(3).toString()));
    }
    else respond(QString("0 0"));

//    if (tokens[1] == "Haines" && tokens[2] == "123") {
//        respond(QString("0 1"));
//    } else {
//        respond(QString("0 0"));
//    }
    qDebug() << "0";
}

void MySocket::respondReg()
{
    QSqlQuery query = _db->itemQuery(QString("acount"),tokens[1]);
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
                << QString("0");

        _db->itemInsert(info);
        respond(QString("1 1"));
    }
    qDebug() << "1";
}

void MySocket::respondLoggedin()
{
    QSqlQuery query = _db->itemQuery(QString("acount"),tokens[1]);
    query.next();
    if (query.isNull(QString("acount"))) {
        respond(QString("11 0"));
    }
    else {
        respond(QString("11 1 %1 %2 %3").arg(query.value(2).toString()).arg(query.value(3).toString()).arg(query.value(0).toString()));
    }
    qDebug() << "addFriend";

}

void MySocket::respondContact()
{
    QSqlQuery query = _db->itemQuery(QString("acount"),tokens[1]);
    query.next();
    if (query.value(8) == "1") {
        respond(QString("10 1 %1 %2").arg(query.value(6).toString()).arg(query.value(7).toString()));
        qDebug() << "con1";
    } else {
        respond(QString("10 0"));
        qDebug() << "con0";
    }
    OffLineMsgPool::GetInstance()->insertUser(tokens[1],tokens[2]);
    OffLineMsgPool::GetInstance()->insertUser(tokens[2],tokens[1]);
    for(int i = 0 ; i < OffLineMsgPool::GetInstance()->getOfflineMsg(tokens[2],tokens[1])->size(); i++) {
        respond(QString("20 ") + (*OffLineMsgPool::GetInstance()->getOfflineMsg(tokens[2],tokens[1]))[i]);
    }
}

void MySocket::respondFind()
{
    QSqlQuery query = _db->itemQuery(QString("acount"),tokens[3]);
    //qDebug() << query.value(1);
    query.next();
    if (query.isNull(QString("acount"))) {
        respond(QString("2 3"));
    } else {
        if (tokens[2] != query.value(5) || tokens[1] != query.value(4)) {
            respond(QString("2 2"));
        } else {
            _db->itemUpdate(QString("password"),tokens[4],QString("acount"),tokens[3]);
            respond(QString("2 1"));
        }
    }
//    if (tokens[2] == "222") {
//        respond(QString("2 2"));
//    } else if (tokens[3] == "333") {
//        respond(QString("2 3"));
//    } else respond(QString("2 1"));
    qDebug() << "2";
}

void MySocket::messageDistribution()
{
    tokens= message.split(" ",QString::SkipEmptyParts);
    qDebug() << tokens[0].toInt();
    switch (tokens[0].toInt()) {
    case 0:
        //emit newLoginEvent();
        respondLogin();
        break;
    case 1:
        //emit newRegEvent();
        respondReg();
        break;
    case 2:
        //emit newFindEvent();
        respondFind();
        break;
    case 10:
        //emit newContactEvent();
        respondContact();
        break;
    case 11:
        //emit newLoggedinEvent();
        respondLoggedin();
        break;
    case 20:
        recvOfflineMsg();
        break;
    case 99:
        isOnline = false;
        break;
    default:
        break;
    }
    //qDebug() << tokens[0].toInt();
}
