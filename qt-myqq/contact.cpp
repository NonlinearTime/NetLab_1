#include "contact.h"
#include "ui_contact.h"
#include "mytitlebar.h"
#include <QDateTime>
#include "publicsocket.h"
#include "messagaitem.h"
#include <QScrollBar>
#include <QMessageBox>

Contact::Contact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contact)
{
    ui->setupUi(this);
    MyTitleBar *m_titleBar = new MyTitleBar(this);
    connect(m_titleBar, SIGNAL(signalButtonMinClicked()),this,SLOT(onButtonMinClicked()));
    connect(m_titleBar,SIGNAL(signalButtonCloseClicked()),this,SLOT(onButtonCloseClicked()));

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

    QPixmap *land = new QPixmap(":/Resources/LoginWindow/land.png");
    QPixmap *headimage = new QPixmap(":/Resources/LoginWindow/HeadImage_small.png");
    land->scaled(ui->land->size(),Qt::KeepAspectRatio);
    headimage->scaled(ui->headimage->size(),Qt::KeepAspectRatio);
    ui->land->setPixmap(*land);
    ui->headimage->setPixmap(*headimage);

    //ui->pushButton->setStyleSheet ("border-radius:10px;");
    //QListWidgetItem::setFlags(Qt::ItemIsSelectable);
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,0%);"
                                                       "margin:0px,0px,0px,0px;"
                                                       "padding-top:9px;"
                                                       "padding-bottom:9px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,25%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::handle:vertical:hover"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,50%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/3.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/1.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/4.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/2.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                       "{"
                                                       "background:rgba(0,0,0,10%);"
                                                       "border-radius:4px;"
                                                       "}"
                                                       );


    m_titleBar->move(0,0);
    m_titleBar->raise();
    m_titleBar->setBackgroundColor(144,144,144,true);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());

    signLabel = new QLabel(this);
    signLabel->move(100,30);
    //signLabel->adjustSize();
    QFont signFont("Timers",12);
    signLabel->setFont(signFont);
    //signLabel->setText("Hello world!");

    //tcpsocket
    tcpSocket = new QTcpSocket(this);

    //创建接收Tcp服务端
    clientServer = new ClientServer(this);
    connect(clientServer,SIGNAL(passMessage(QString)),this,SLOT(onlineMessage(QString)));;

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));

    //绑定共用socket信号与本窗口的槽
    connect(PublicSocket::GetInstance()->mySocket,SIGNAL(recvConMsg(QString)),this,SLOT(read(QString)));
}

Contact::~Contact()
{
    delete ui;
}

void Contact::setHostInfo(QHostAddress adrs, qint16 port)
{
    hostAddress = adrs;
    hostPort = port;
}

void Contact::newConnection()
{
    blockSize = 0;
    tcpSocket->abort();
    qDebug() << "connectioning";
    tcpSocket->connectToHost(hostAddress,hostPort);
    qDebug() << "connected";
    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}

void Contact::messageDistribution()
{
    QStringList tokens= message.split(" ",QString::SkipEmptyParts);     //服务器返回消息类型
    if (tokens[0] == "20") {                                             //10即为取用户名返回消息
        QString piece;
        for (int i = 1; i < tokens.size() ; i++) {
            if (i == 1) piece.append(tokens[i]);
            else piece.append(QString(" ") + tokens[i]);
        }
        qDebug() << piece;
        QStringList tmp = piece.split(1,QString::SkipEmptyParts);
        for (int i = 0 ; i != tmp.size() ; i++)
            displayMessage(tmp[i],true);
    }
}

void Contact::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
}

void Contact::sendMessage()
{
    QString seq = ui->textEdit->toPlainText();
    QString s;
    s = QDateTime::currentDateTime().toString() + "\n" + seq;
    if(isOnline) {
        qDebug() << seq;
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_9);
        out<<(quint16) 0 ;
        out << seq;
        qDebug() << "contact sending";
        out.device()->seek(0);
        out<<(quint16) (block.size() - sizeof(quint16));
        //qDebug() << out;
        tcpSocket->write(block);
        tcpSocket->flush();
    } else {
        QString msg;
        msg = QString("20 ") + signLabel->text() + QString(" ") + PublicSocket::GetInstance()->getNickName()[2] + QString(" ") + s;
        PublicSocket::GetInstance()->sendData(msg);
    }
    displayMessage(s,false);
    ui->textEdit->clear();
    ui->textEdit->setFocus();
}

void Contact::read(QString msg)
{
    message = msg;
    qDebug() << "read";
    messageDistribution();
}

void Contact::fileCancled()
{
    tcpSocket->write(QString("4").toUtf8());
}

void Contact::displayMessage(QString msg, bool isOnlineMsg)
{   
    messages.append(msg);

    QListWidgetItem *newItem = new QListWidgetItem();
    MessagaItem *newM = new MessagaItem(msg,isOnlineMsg);


    qDebug() << newM->newMsg->size();

    newItem->setSizeHint(QSize(newM->labelSize.width(),newM->labelSize.height()+30));
    ui->listWidget->addItem(newItem);
    ui->listWidget->setItemWidget(newItem,newM);
    newItem->setFlags(Qt::NoItemFlags);
    newItem->setTextAlignment(Qt::AlignRight);
    //ui->listWidget->insertItem(messages.size(),msg);
    qDebug() << "displayMessage";
}

void Contact::onlineMessage(QString msg)
{
    QStringList msgList = msg.split(1,QString::SkipEmptyParts);
    qDebug() << msgList;
    if (msgList[0] == "1") {
        if(QMessageBox::warning(this,tr("接收文件"),tr("对方向你发起了文件传输，是否接受文件 %1(%2M) ？").arg(msgList[1]).arg(msgList[2].toULongLong() / 1024 /1024),tr("接收"),tr("拒绝")) == 0){
            qDebug() << QMessageBox::Yes;

            QString filepath = QFileDialog::getSaveFileName(nullptr,QString(),msgList[1]);
            //PublicSocket::GetInstance()->mySocket->localAddress();

            //创建进度显示及控制窗口
            ReceiverForm *receiverform = new ReceiverForm();

            ReceivedFile::GetInstance()->fileMaxSize = msgList[2].toULongLong();
            qDebug() << ReceivedFile::GetInstance()->fileMaxSize;
            ReceivedFile::GetInstance()->fileSize = 0;
            qDebug() << ReceivedFile::GetInstance()->fileSize;

            //创建接收文件实例
            Receicer *receiver = new Receicer();
            receiver->setFileName(filepath);
            receiver->getIpAddress(tcpSocket->peerAddress(),tcpSocket->peerPort());
            receiver->initSocket(PublicSocket::GetInstance()->getClientPort());
            receiver->initContainer();

            //创建线程
            QThread *thread = new QThread();
            connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

            connect(receiverform,SIGNAL(cancelFile()),thread,SLOT(quit()));
            connect(receiverform,SIGNAL(finished()),thread,SLOT(quit()));

            //线程分离
            receiver->moveToThread(thread);
            thread->start();

            //显示接收窗口
            receiverform->setFileMaxSize();
            receiverform->show();
            receiverform->updateProgressBar();


            //发送接受准备完毕消息，告知发送端开始发送文件
            char c = 1;
            QString msg = QString("2%1").arg(c);
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_9);
            out<<(quint16) 0 ;
            out << msg;
            qDebug() << "contact sending";
            out.device()->seek(0);
            out<<(quint16) (block.size() - sizeof(quint16));
            //qDebug() << out;
            tcpSocket->write(block);
            tcpSocket->flush();

        }
        else {
            char c = 1;
            QString msg = QString("3%1").arg(c);
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_9);
            out<<(quint16) 0 ;
            out << msg;
            qDebug() << "contact sending";
            out.device()->seek(0);
            out<<(quint16) (block.size() - sizeof(quint16));
            //qDebug() << out;
            tcpSocket->write(block);
            tcpSocket->flush();
        }
    } else if (msgList[0] == "2") {

        qDebug() << "1";
        //创建发送实例
        Sender *sender = new Sender();
        qDebug() << "11";
        sender->getFileName(filename);
        //创建发送窗口
        SenderForm *senderform = new SenderForm();
        qDebug() << "12";
        sender->initSocket(PublicSocket::GetInstance()->getClientPort());
        qDebug() << "13";
        qDebug() << tcpSocket->peerAddress() << tcpSocket->peerPort();
        sender->getIpAddress(tcpSocket->peerAddress(),tcpSocket->peerPort());

        qDebug() << "14";
        connect(this,SIGNAL(sendstart()),sender,SLOT(sendFile()));
        qDebug() << "2";
        //创建线程
        QThread *thread = new QThread();
        connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
        qDebug() << "3";
        connect(senderform,SIGNAL(cancelFile()),thread,SLOT(quit()));
        connect(senderform,SIGNAL(finished()),thread,SLOT(quit()));
        qDebug() << "4";
        //线程分离
        sender->moveToThread(thread);
        thread->start();
        qDebug() << "5";
        //显示发送窗口
        senderform->setFileMaxSize();
        senderform->show();
        senderform->updateProgressBar();
        qDebug() << "6";
        //开始发送
        emit sendstart();


    } else if (msgList[0] == "3" ) {
        QMessageBox::warning(this,tr("文件传输失败"),tr("对方拒绝了你的文件传输请求！"),QMessageBox::Yes);
    } else if (msgList[0] == "4") {
        QMessageBox::warning(this,tr("发送终端"),tr("对方取消了文件接收!"),QMessageBox::Yes);

    } else{
        QString s = QDateTime::currentDateTime().toString() + "\n" + msg;
        displayMessage(s,true);
    }
}

void Contact::onButtonMinClicked()
{
    if (Qt::Tool == (windowFlags() & Qt::Tool)) {
        hide();
    } else {
        showMinimized();
    }
}

void Contact::onButtonCloseClicked()
{
    ui->textEdit->clear();
    ui->listWidget->clear();
    messages.clear();
    close();
}



void Contact::on_fileButton_clicked()
{
    //QString url = QFileDialog::getSaveFileName(nullptr,QString(),QString("123.h"));
    //qDebug() << url;
    filename = QFileDialog::getOpenFileName(this,tr("选择文件"));
    qDebug() << filename;
    QString file = filename.split("/",QString::SkipEmptyParts).last();
    qDebug() << file;
    QFile p_file;
    p_file.setFileName(filename);
    p_file.open(QFile::ReadOnly);
    qDebug() << p_file.size();
    QString rq = QString("1%1%2%3%4").arg(char(1)).arg(file).arg(char(1)).arg(p_file.size());
    p_file.close();
    //qDebug() << rq << QMessageBox::warning(this," "," ",QMessageBox::Yes);
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<(quint16) 0 ;
    out << rq;
    qDebug() << "contact sending";
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));
    //qDebug() << out;
    tcpSocket->write(block);
    tcpSocket->flush();
    //tcpSocket->write(rq.toUtf8());
}
