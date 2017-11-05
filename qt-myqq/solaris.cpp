#include "solaris.h"
#include "ui_solaris.h"
#include "mytitlebar.h"
#include <QMovie>
#include <QLabel>
#include <QDebug>
#include <publicsocket.h>
#include <QBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

Solaris::Solaris(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solaris)
{
    MyTitleBar *titleBar = new MyTitleBar(this);

    connect(titleBar,SIGNAL(signalButtonCloseClicked()),this,SLOT(onButtonCloseClicked()));
    connect(titleBar,SIGNAL(signalButtonMinClicked()),this,SLOT(onButtonMinClicked()));

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    QLabel *pBack = new QLabel(this);
    QMovie *movie = new QMovie();

    movie->setFileName(":/Resources/LoginWindow/head.png");
    movie->setScaledSize(QSize(this->width(),182));
    movie->start();
    movie->stop();
    pBack->setMovie(movie);
    movie->start();
    pBack->move(0,0);

    titleBar->move(0,0);
    titleBar->raise();
    titleBar->setBackgroundColor(0,0,0,true);
    titleBar->setButtonType(MIN_BUTTON);
    titleBar->setTitleWidth(this->width());

    //sendMessage();

    this->list = new PersonList(ui->listWidget,this->username);
    this->list->resize(ui->listWidget->size());
    this->list->raise();
    //this->list->setStyleSheet("{background:none}");

    //network
    //tcpSocket = new QTcpSocket(this);
    //hostAddress = QHostAddress::LocalHost;
    //hostPort = 6666;
    //newConnection();

    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(PublicSocket::GetInstance()->mySocket,SIGNAL(recvSolMsg(QString)),this,SLOT(read(QString)));
    connect(PublicSocket::GetInstance()->mySocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    //获取昵称
    //sendMessage(QString("10 getNickName"));

    //获取好友列表
    //getFriendsList();

    //添加好友对话框
    addFriend = new QDialog(this);
    addFriend->setWindowTitle(tr("添加好友"));
    addFriend->setFixedHeight(400);
    addFriend->setFixedWidth(600);
    QLabel *addLabel = new QLabel(addFriend);
    addLineEdit = new QLineEdit(addFriend);
    QPushButton *enterButton = new QPushButton(addFriend);
    QPushButton *quitButton = new QPushButton(addFriend);

    addLabel->setText(tr("输入好友昵称"));
    enterButton->setText(tr("添加"));
    quitButton->setText(tr("取消"));

    QHBoxLayout *layout = new QHBoxLayout();
    QFormLayout *mainFormLayout = new QFormLayout(addFriend);
    mainFormLayout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    mainFormLayout->setVerticalSpacing(40);
    mainFormLayout->setHorizontalSpacing(10);
    mainFormLayout->addRow(addLabel,addLineEdit);
    layout->addWidget(enterButton);
    layout->addWidget(quitButton);
    layout->setSpacing(40);
    mainFormLayout->addRow(layout);

    connect(list,SIGNAL(showDlgAdd()),addFriend,SLOT(show()));
    connect(enterButton,SIGNAL(clicked(bool)),this,SLOT(sendAcount()));
    connect(quitButton,SIGNAL(clicked(bool)),addFriend,SLOT(close()));
}

Solaris::~Solaris()
{
    delete ui;
}

void Solaris::newConnection()
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(hostAddress,hostPort);
}

void Solaris::messageDistribution()
{
    QStringList tokens= message.split(" ",QString::SkipEmptyParts);     //服务器返回消息类型
    qDebug() <<tokens;
    if (tokens[0] == "10") {                                             //10即为取用户名返回消息
        //this->username = tokens[1];
        //QStringList info;
        //info << tokens[1] << tokens[2];
        //list->slotAddBuddy(info);
        if (tokens[1] == tr("0")) {
            list->contact.isOnline = 0;
        } else {
            list->contact.isOnline = 1;
            QHostAddress addr(tokens[2]);
            quint16 prt = tokens[3].toInt();
            list->contact.setHostInfo(addr,prt);
            list->contact.newConnection();
        }
    } else if (tokens[0] == "11") {                                       //11即为添加好友消息返回
        if (tokens[1] == tr("1")) {                                       //存在用户，添加成功
            QMessageBox::warning(addFriend,tr("通知"),tr("添加好友成功"),QMessageBox::Yes); //服务器
            QStringList info;
            info << tokens[2] << tokens[3] << tokens[4];
            list->slotAddBuddy(info);
            addFriend->close();
        } else if (tokens[1] == tr("0")) {                               //帐户名不存在
            QMessageBox::warning(addFriend,tr("通知"),tr("帐户不存在"),QMessageBox::Yes);
            addLineEdit->clear();
            addLineEdit->setFocus();
        }
    } else if (tokens[0] == "12"){                                       //2即为找回密码消息返回
        QStringList info;
        QStringList tmp;
        QString piece;
        for (int i = 1; i < tokens.size() ; i++) {
            if (i == 1) piece.append(tokens[i]);
            else piece.append(QString(" ") + tokens[i]);
        }
        qDebug() << piece;
        tmp = piece.split(1,QString::SkipEmptyParts);
        for (int i = 0; i != tmp.size() ; i++) {
            QStringList res = tmp[i].split(" ",QString::SkipEmptyParts);
            qDebug() << res;
            info << res[0] << res[1] << res[2];
            list->slotAddBuddy(info);
            info.clear();
        }
    }
}

void Solaris::sendMessage(const QString &seq)
{
    PublicSocket::GetInstance()->sendData(seq);
}

void Solaris::getFriendsList()
{
    sendMessage(QString("12"));
}

void Solaris::onButtonCloseClicked()
{
    PublicSocket::GetInstance()->sendData("99");
    PublicSocket::GetInstance()->mySocket->disconnectFromHost();
    close();
}

void Solaris::onButtonMinClicked()
{
    if (Qt::Tool == (windowFlags()&Qt::Tool)) {
        hide();
    } else {
        showMinimized();
    }
}

void Solaris::readMessage() //未使用
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_9);
    if(blockSize == 0) {
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize) return;
    in >> message;
    qDebug() << message;
    blockSize = 0;
    messageDistribution();
}

void Solaris::read(QString msg)
{
    message = msg;
    qDebug() << "read";
    messageDistribution();
}

void Solaris::displayError(QAbstractSocket::SocketError)
{
    qDebug() << PublicSocket::GetInstance()->mySocket->errorString();
}

void Solaris::sendAcount()
{
    qDebug() << "sendAcount";
    QString msg = QString("11 ") + addLineEdit->text().trimmed();
    sendMessage(msg);
}

void Solaris::showFndDlg()
{

}


