#include "server.h"
#include "ui_server.h"
#include <iostream>
#include "database.h"
#include <QScrollBar>


Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    //tcpServer = new QTcpServer(this);
    //if (!tcpServer->listen(QHostAddress::LocalHost,6666)) {
    //    qDebug() << tcpServer->errorString();
    //    close();
    //}
    //connect(tcpServer,SIGNAL(newConnection()),this,SLOT(updateIp()));

    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));

    //设置该单元格为屏蔽状态，不能编辑和选中。
    //ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEditable);


    //取消表头的在选中单元格时的高亮状态。
    ui->tableView->horizontalHeader()->setHighlightSections(false);

    //取消表头的在选中单元格时的高亮状态。
    ui->tableView->verticalHeader()->setHighlightSections(false);
    //ui->tableView->itemDelegate()

    //绑定事件分发信号与槽
    connect(this,SIGNAL(newLoginEvent()),SLOT(respondLogin()));
    connect(this,SIGNAL(newFindEvent()),SLOT(respondFind()));
    connect(this,SIGNAL(newRegEvent()),SLOT(respondReg()));

    model = new QSqlTableModel(this);
    model->setTable("users");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行

    //不显示name属性列,如果这时添加记录，则该属性的值添加不上
    // model->removeColumn(1);

    ui->tableView->setModel(model);

    //使其不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //设置表头字体加粗
    QFont font = ui->tableView->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView->horizontalHeader()->setFont(font);

    //设置焦点
    ui->tableView->setFocusPolicy(Qt::NoFocus);

    ui->tableView->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    //ui->tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setDefaultSectionSize(30); //设置行高
    ui->tableView->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableView->setShowGrid(false); //设置不显示格子线
    ui->tableView->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableView->horizontalHeader()->resizeSection(0,150); //设置表头第一列的宽度为150
    ui->tableView->horizontalHeader()->setFixedHeight(30); //设置表头的高度
    ui->tableView->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    //ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    //修改滚动条样式
    ui->tableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    ui->tableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    //修改在线列表样式
    ui->clientList->setFrameShape(QFrame::NoFrame);

    //修改在线列表滚动条样式
    ui->clientList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");
    ui->clientList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");


    //多线程服务端
    _ts = new ThreadedServer(this);
    if (!_ts->listen(QHostAddress::LocalHost,6666)) {
        qDebug() << _ts->errorString();
        close();
    }
    connect(_ts,SIGNAL(newListItem(QString)),this,SLOT(updateClientList(QString)));
    connect(_ts,SIGNAL(logout()),this,SLOT(updataLogout()));
    //connect(_ts,SIGNAL(newConnection()),this,SLOT(updateIp()));
}

Server::~Server()
{
    delete ui;
}

void Server::updateIp()
{
    blockSize = 0;
    //tcpSocket = tcpServer->nextPendingConnection();
    //qintptr ID = tcpSocket->socketDescriptor();
    //QTcpSocket *tmpSocket = new QTcpSocket();
    //tmpSocket = tcpServer->nextPendingConnection();
    //ServerSocket *socket =  new ServerSocket(tmpSocket);
    //connect(socket,SIGNAL(newListItem(QString)),this,SLOT(updateClientList(QString)));
    //connect(socket,SIGNAL(logout()),this,SLOT(updataLogout()));
    //socketpool << socket;
    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void Server::respond(const QString &seq)
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<(quint16) 0 ;
    out << seq;
    qDebug() << tr("log");
    qDebug() << block;
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));
    //qDebug() << out;

    tcpSocket->write(block);
    tcpSocket->flush();
}

void Server::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_9);
    if(blockSize == 0) {
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
    messageDistribution();
    qDebug() << message;
    qDebug() << tokens;
    //if (message == QString("Haines") + QString("123")) {
    //    respondIsTrue(true);
    //} else respondIsTrue(false);
    blockSize = 0;
}

void Server::respondLogin()
{
    QSqlQuery query = DataBase::GetInstance()->itemQuery(QString("acount"),tokens[1]);
    query.next();
    qDebug() << !query.isNull(QString("acount"));
    qDebug() << query.value(0).toString();
    if (!query.isNull(QString("acount")) && (tokens[2] == query.value(1).toString())) {
        respond(QString("0 1"));
        DataBase::GetInstance()->itemUpdate(QString("ipaddress"),tcpSocket->peerAddress().toString() , QString("acount"),tokens[1]);
        DataBase::GetInstance()->itemUpdate(QString("port"),QString::number(tcpSocket->peerPort(),10),QString("acount"),tokens[1]);
        //_db->itemUpdate(QString("isonline"),QString("1"),QString("acount"),tokens[1]);
    }
    else respond(QString("0 0"));

//    if (tokens[1] == "Haines" && tokens[2] == "123") {
//        respond(QString("0 1"));
//    } else {
//        respond(QString("0 0"));
//    }
    qDebug() << "0";
}

void Server::respondReg()
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
                << QString("Haines")
                << QString("Hello world!")
                << tokens[4]
                << tokens[5]
                << QString("")
                << QString("")
                << QString("0");

        DataBase::GetInstance()->itemInsert(info);
        respond(QString("1 1"));
    }
    qDebug() << "1";
}

void Server::respondFind()
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
//    if (tokens[2] == "222") {
//        respond(QString("2 2"));
//    } else if (tokens[3] == "333") {
//        respond(QString("2 3"));
//    } else respond(QString("2 1"));
    qDebug() << "2";
}

void Server::messageDistribution()
{
    tokens= message.split(" ",QString::SkipEmptyParts);
    switch (tokens[0].toInt()) {
    case 0:
        emit newLoginEvent();
        break;
    case 1:
        emit newRegEvent();
        break;
    case 2:
        emit newFindEvent();
        break;
    default:
        break;
    }
    //qDebug() << tokens[0].toInt();
}

void Server::updateClientList(QString onlineInfo)
{
    ui->clientList->addItem(onlineInfo);
}

void Server::updataLogout()
{
    model->select();
    ui->tableView->setModel(model);
}
