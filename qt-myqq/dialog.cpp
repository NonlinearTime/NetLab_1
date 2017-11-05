#include "dialog.h"
#include "ui_dialog.h"
#include <QLabel>
#include <QMovie>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QBoxLayout>
#include <QFormLayout>
#include "mytitlebar.h"
#include "solaris.h"
#include "publicsocket.h"
//#include <QSqlDatabase>
//#include <QSqlQuery>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    MyTitleBar *m_titleBar = new MyTitleBar(this);
    //m_titleBar->loadStyleSheet(":/Resources/MyTitle.css");
    connect( m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect( m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    ui->setupUi(this);

    //背景图;
    QLabel* pBack = new QLabel(this);
    QMovie *movie = new QMovie();
    movie->setFileName(":/Resources/LoginWindow/backgound.gif");
    movie->setScaledSize(QSize(this->width(),this->height() / 2));
    movie->start();
    movie->stop();
    pBack->setMovie(movie);
    movie->start();
    pBack->move(0, 0);
    //注释;

    ui->usernameEdit->setPlaceholderText(QStringLiteral("QQ号码/手机/邮箱"));
    ui->passwordEdit->setPlaceholderText(QStringLiteral("密码"));
    ui->passwordEdit->setEchoMode(QLineEdit::Password);

    QPixmap *pixmap = new QPixmap(":/Resources/LoginWindow/HeadImage.png");
    pixmap->scaled(ui->userHead->size(),Qt::KeepAspectRatio);
    ui->userHead->setScaledContents(true);
    ui->userHead->setPixmap(*pixmap);
    ui->userHead->stackUnder(ui->loginState);
    ui->loginState->setIcon(QIcon(":/Resources/LoginWindow/LoginState/state_online.png"));
    ui->loginState->setIconSize(QSize(25, 25));
    ui->loginState->setFlat(true);
    ui->moreAccountLogin->setIcon(QIcon(":/Resources/LoginWindow/more_accountlogin.png"));
    ui->moreAccountLogin->setIconSize(QSize(25, 25));
    ui->moreAccountLogin->setFlat(true);
    ui->pushButtonFlicker->setIcon(QIcon(":/Resources/LoginWindow/flicker.png"));
    ui->pushButtonFlicker->setIconSize(QSize(25, 25));
    ui->pushButtonFlicker->setFlat(true);

    //bar

    m_titleBar->move(0, 0);
    m_titleBar->raise();
    m_titleBar->setBackgroundColor(0, 0, 0 , true);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
    m_titleBar->setTitleContent("qq 2018");

    //network
    tcpSocket = new QTcpSocket();
    //tcpSocket->setSocketDescriptor(tcpSocket);
    hostAddress = QHostAddress::LocalHost;
    hostPort = 6666;
    newConnection();

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    //connect(this,SIGNAL(revData()),this,SLOT(readMessage()));
    connect(PublicSocket::GetInstance()->mySocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    //connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sendNameAndPwd()));
    connect(PublicSocket::GetInstance()->mySocket,SIGNAL(recvDlgMsg(QString)),this,SLOT(read(QString)));

    //find password back
    findPwd = new QDialog(this);
    findPwd->setWindowTitle(tr("找回密码"));
    findPwd->setFixedHeight(400);
    findPwd->setFixedWidth(600);
    oldPwdLineEdit = new QLineEdit(findPwd);
    oldPwdLineEdit->setEchoMode(QLineEdit::Password);
    QLabel *oldPwdLabel = new QLabel(findPwd);
    newPwdLineEdit = new QLineEdit(findPwd);
    newPwdLineEdit->setEchoMode(QLineEdit::Password);
    QLabel *newPwdLabel = new QLabel(findPwd);
    nameLineEdit = new QLineEdit(findPwd);
    QLabel *nameLabel = new QLabel(findPwd);
    questionLineEdit = new QLineEdit(findPwd);
    QLabel *questionLabel = new QLabel(findPwd);
    answerLineEdit = new QLineEdit(findPwd);
    QLabel *answerLabel = new QLabel(findPwd);
    QPushButton *enterButton = new QPushButton(findPwd);
    QPushButton *loginButton = new QPushButton(findPwd);
    nameLabel->setText(tr("帐户名:"));
    oldPwdLabel->setText(tr("新的密码:"));
    newPwdLabel->setText(tr("重新输入一次:"));
    questionLabel->setText(tr("输入密保问题:"));
    answerLabel->setText(tr("问题答案:"));
    enterButton->setText(tr("确定"));
    loginButton->setText(tr("返回"));

//    QVBoxLayout *layout1 = new QVBoxLayout(findPwd);
//    QVBoxLayout *layout2 = new QVBoxLayout(findPwd);
    QHBoxLayout *layout3 = new QHBoxLayout();
    QFormLayout *mainFormLayout = new QFormLayout(findPwd);
    mainFormLayout->setSizeConstraint(QLayout::SetFixedSize);
    layout3->setSizeConstraint(QLayout::SetFixedSize);
    mainFormLayout->setVerticalSpacing(40);
    mainFormLayout->setHorizontalSpacing(10);
    mainFormLayout->addRow(questionLabel,questionLineEdit);
    mainFormLayout->addRow(answerLabel,answerLineEdit);
    mainFormLayout->addRow(nameLabel,nameLineEdit);
    mainFormLayout->addRow(oldPwdLabel,oldPwdLineEdit);
    mainFormLayout->addRow(newPwdLabel,newPwdLineEdit);
    layout3->addWidget(enterButton);
    layout3->addWidget(loginButton);
    layout3->setSpacing(40);
    mainFormLayout->addRow(layout3);

    connect(ui->pushButtonForgetPassword,SIGNAL(clicked(bool)),this,SLOT(showFindPwdDlg()));
    connect(loginButton,SIGNAL(clicked(bool)),findPwd,SLOT(close()));
    connect(enterButton,SIGNAL(clicked(bool)),this,SLOT(sendNewNameAndPwd()));

    //register
    rstDialog = new QDialog(this);
    rstDialog->setWindowTitle(tr("注册帐户"));
    rstDialog->setFixedHeight(400);
    rstDialog->setFixedWidth(600);
    regQstLineEdit = new QLineEdit(rstDialog);
    regAnsLineEdit = new QLineEdit(rstDialog);
    regNameLineEdit = new QLineEdit(rstDialog);
    regPwdLineEdit = new QLineEdit(rstDialog);
    regPwdLineEdit->setEchoMode(QLineEdit::Password);
    regDPwdLineEdit = new QLineEdit(rstDialog);
    regDPwdLineEdit->setEchoMode(QLineEdit::Password);
    regNickLineEdit = new QLineEdit(rstDialog);
    regSignLineEdit = new QLineEdit(rstDialog);

    QLabel *regQstLabel = new QLabel(rstDialog);
    QLabel *regAnsLabel = new QLabel(rstDialog);
    QLabel *regNameLabel = new QLabel(rstDialog);
    QLabel *regPwdLabel = new QLabel(rstDialog);
    QLabel *regDPwdLabel = new QLabel(rstDialog);
    QLabel *regNickLabel = new QLabel(rstDialog);
    QLabel *regSignLabel = new QLabel(rstDialog);
    regQstLabel->setText(tr("密保问题:"));
    regAnsLabel->setText(tr("问题答案:"));
    regNameLabel->setText(tr("帐户名称:"));
    regPwdLabel->setText(tr("帐户密码"));
    regDPwdLabel->setText(tr("再输入一次:"));
    regNickLabel->setText(tr("昵称"));
    regSignLabel->setText(tr("个性签名"));
    QPushButton *regEnter = new QPushButton(tr("注册"));
    QPushButton *regLogin = new QPushButton(tr("返回"));

    QHBoxLayout *layout1 = new QHBoxLayout();
    QFormLayout *regLayout = new QFormLayout(rstDialog);
    regLayout->setSizeConstraint(QLayout::SetFixedSize);
    layout1->setSizeConstraint(QLayout::SetFixedSize);
    regLayout->setVerticalSpacing(40);
    regLayout->setHorizontalSpacing(10);
    regLayout->addRow(regNameLabel,regNameLineEdit);
    regLayout->addRow(regPwdLabel,regPwdLineEdit);
    regLayout->addRow(regDPwdLabel,regDPwdLineEdit);
    regLayout->addRow(regQstLabel,regQstLineEdit);
    regLayout->addRow(regAnsLabel,regAnsLineEdit);
    regLayout->addRow(regNickLabel,regNickLineEdit);
    regLayout->addRow(regSignLabel,regSignLineEdit);
    layout1->addWidget(regEnter);
    layout1->addWidget(regLogin);
    layout1->setSpacing(40);
    regLayout->addRow(layout1);

    connect(ui->pushButtonRegister,SIGNAL(clicked(bool)),this,SLOT(showRegDlg()));
    connect(regLogin,SIGNAL(clicked(bool)),rstDialog,SLOT(close()));
    connect(regEnter,SIGNAL(clicked(bool)),this,SLOT(sendRegNameAndPwd()));

    //设置本机聊天服务端端口
    PublicSocket::GetInstance()->setClientIP(QHostAddress::LocalHost);
    PublicSocket::GetInstance()->setClientPort(9005);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::newConnection()
{
    blockSize = 0;
    //tcpSocket->abort();
    //tcpSocket->connectToHost(hostAddress,hostPort);
}

void Dialog::messageDistribution()
{
    QStringList tokens= message.split(" ",QString::SkipEmptyParts);     //服务器返回消息类型
    if (tokens[0] == "0") {                                             //0即为登陆消息返回
        if (tokens[1] == tr("1")) {
            //username = tokens[2];
            //sendMessage(QString("99 close"));
            //tcpSocket->disconnectFromHost();
            QStringList info;
            info << tokens[2] << tokens[3] << tokens[4];
            PublicSocket::GetInstance()->setNickName(info);
            accept();
        } else {
            QMessageBox::warning(this,tr("警告"),tr("帐户名或者密码错误"),QMessageBox::Yes);
            //clear wrong context
            ui->usernameEdit->clear();
            ui->passwordEdit->clear();
            ui->usernameEdit->setFocus();
        }
    } else if (tokens[0] == "1") {                                       //1即为注册消息返回
        if (tokens[1] == tr("1")) {                                     //注册成功
            QMessageBox::warning(rstDialog,tr("通知"),tr("帐户注册成功"),QMessageBox::Yes); //服务器
            rstDialog->close();
        } else if (tokens[1] == tr("2")) {                              //帐户名已被注册
            QMessageBox::warning(rstDialog,tr("通知"),tr("帐户名已被注册"),QMessageBox::Yes);
            regNameLineEdit->clear();
            regNameLineEdit->setFocus();
        }
    } else if (tokens[0] == "2"){                                       //2即为找回密码消息返回
        if (tokens[1] == tr("1")) {                                     //修改成功
            QMessageBox::warning(findPwd,tr("通知"),tr("帐户密码修改成功"),QMessageBox::Yes);
            findPwd->close();
        } else if (tokens[1] == tr("2")) {                              //密保问题回答错误
            QMessageBox::warning(findPwd,tr("通知"),tr("密保问题或回答错误"),QMessageBox::Yes);
            answerLineEdit->clear();
            answerLineEdit->setFocus();
        } else if (tokens[1] == tr("3")) {                              //帐户名不存在
            QMessageBox::warning(findPwd,tr("通知"),tr("帐户名不存在"),QMessageBox::Yes);
            nameLineEdit->clear();
            nameLineEdit->setFocus();
        }
    }
}

void Dialog::sendNameAndPwd()
{
    //QByteArray block;
    //QDataStream out(&block,QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_9);
    //out<<(quint16) 0 ;
    //out<<(QString("0 ") + ui->usernameEdit->text().trimmed() + QString(" ") + ui->passwordEdit->text());
    //out.device()->seek(0);
    qDebug() << "sendNameAndPwd";
    //out<<(quint16) (block.size() - sizeof(quint16));

    //tcpSocket->write(block);
    //tcpSocket->flush();

    QString msg = QString("0 ") + ui->usernameEdit->text().trimmed() + QString(" ") + ui->passwordEdit->text()
            + QString(" ") + PublicSocket::GetInstance()->getClientIP().toString() + QString(" ") +
            QString::number(PublicSocket::GetInstance()->getClientPort(),10);
    sendMessage(msg);

}

void Dialog::displayError(QAbstractSocket::SocketError)
{
    qDebug() << PublicSocket::GetInstance()->mySocket->errorString();
}

void Dialog::showFindPwdDlg()
{
    findPwd->show();
}

void Dialog::showRegDlg()
{
    rstDialog->show();
}

void Dialog::sendNewNameAndPwd()
{
    if (newPwdLineEdit->text() != oldPwdLineEdit->text()) {
        QMessageBox::warning(findPwd,tr("警告"),tr("两次密码输入不一致"));
        newPwdLineEdit->clear();
        oldPwdLineEdit->clear();
        oldPwdLineEdit->setFocus();
        //oldPwdLineEdit->setFocus();
    } else {
        //QByteArray block;
        //QDataStream out(&block,QIODevice::WriteOnly);
        //out.setVersion(QDataStream::Qt_5_9);
        //out<<(quint16) 0 ;
        //out<<(QString("2 ") + questionLineEdit->text().trimmed() +QString(" ") + answerLineEdit->text().trimmed()
        //     + QString(" ") + nameLineEdit->text().trimmed() + QString(" ") + oldPwdLineEdit->text()
        //      + QString(" ") + newPwdLineEdit->text());
        //out.device()->seek(0);
        //out.device()->seek(0);
        //out<<(quint16) (block.size() - sizeof(quint16));

        //tcpSocket->write(block);
        //tcpSocket->flush();
        qDebug() << "sendNewNameAndPwd";
        QString msg = (QString("2 ") + questionLineEdit->text().trimmed() +QString(" ") + answerLineEdit->text().trimmed()
                       + QString(" ") + nameLineEdit->text().trimmed() + QString(" ") + oldPwdLineEdit->text()
                       + QString(" ") + newPwdLineEdit->text());
        sendMessage(msg);
    }
}

void Dialog::changedSuccessfully()
{

}

void Dialog::sendRegNameAndPwd()
{
    if (regPwdLineEdit->text() != regDPwdLineEdit->text()) {
        QMessageBox::warning(rstDialog,tr("警告"),tr("两次密码输入不一致"));
        regPwdLineEdit->clear();
        regDPwdLineEdit->clear();
        regPwdLineEdit->setFocus();
        //oldPwdLineEdit->setFocus();
    } else {
        //QByteArray block;
        //QDataStream out(&block,QIODevice::WriteOnly);
        //out.setVersion(QDataStream::Qt_5_9);
        //out<<(quint16) 0 ;
        //out<<(QString("1 ") + regNameLineEdit->text().trimmed() + QString(" ")
        //      + regPwdLineEdit->text()+ QString(" ") + regDPwdLineEdit->text() + QString(" ") +
        //      regQstLineEdit->text().trimmed() +QString(" ") + regAnsLineEdit->text().trimmed());
        //out.device()->seek(0);
        //out<<(quint16) (block.size() - sizeof(quint16));

        qDebug() << "sendRegNameAndPwd";
        QString msg = (QString("1 ") + regNameLineEdit->text().trimmed() + QString(" ")
                   + regPwdLineEdit->text()+ QString(" ") + regDPwdLineEdit->text() + QString(" ") +
                   regQstLineEdit->text().trimmed() +QString(" ") + regAnsLineEdit->text().trimmed()) + QString(" ")
                   + regNickLineEdit->text().trimmed() + QString(" ") + regSignLineEdit->text().trimmed();
        sendMessage(msg);
    }
}

void Dialog::registeredSuccessfull()
{

}

void Dialog::read(QString msg)
{
    message = msg;
    qDebug() << "read";
    messageDistribution();
}

void Dialog::recvData()
{
    emit revData();
    qDebug() << "log1";
    readMessage();
    qDebug() << "log2";
}

void Dialog::sendMessage(const QString &seq)
{
    PublicSocket::GetInstance()->sendData(seq);
}

void Dialog::on_pushButton_clicked()
{
    sendNameAndPwd();
//    if (ui->usernameEdit->text().trimmed() == "Haines" && ui->passwordEdit->text() == "123") {
//        accept();
//    } else {
//        QMessageBox::warning(this,tr("Warning!!!"),tr("Username or password wrong!!!"),QMessageBox::Yes);
//        //clear wrong context
//        ui->usernameEdit->clear();
//        ui->passwordEdit->clear();
//        ui->usernameEdit->setFocus();
//    }
}

void Dialog::onButtonMinClicked()
{
    if (Qt::Tool == (windowFlags() && Qt::Tool)) {
        hide();
    } else {
        showMinimized();
    }
}

void Dialog::onButtonCloseClicked()
{
    PublicSocket::GetInstance()->sendData("99");
    PublicSocket::GetInstance()->mySocket->disconnectFromHost();
    close();
}

void Dialog::readMessage()
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
    qDebug() << "log3";
    blockSize = 0;
    messageDistribution();
}
