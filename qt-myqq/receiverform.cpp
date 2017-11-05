#include "receiverform.h"
#include "ui_receiverform.h"
#include <QDebug>

ReceiverForm::ReceiverForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiverForm)
{
    ui->setupUi(this);

    fileReceived = 0;
    filePerSecond = 0;

    setFileMaxSize();

    updateProgressBar();

    connect(this,SIGNAL(finished()),this,SLOT(receiveFinished()));
}

ReceiverForm::~ReceiverForm()
{
    delete ui;
}

void ReceiverForm::setFileMaxSize()
{
    ui->progressBar->setMaximum(ReceivedFile::GetInstance()->fileMaxSize);
}

void ReceiverForm::updateProgressBar()
{
    ui->progressBar->setValue(ReceivedFile::GetInstance()->fileSize);

    filePerSecond = ReceivedFile::GetInstance()->fileSize - fileReceived;
    fileReceived = ReceivedFile::GetInstance()->fileSize;
    float speed = filePerSecond / 1024 /1024;
    ui->sizeLabel->setText(QString("%1M / %2M").arg(QString::number((double)(fileReceived / 1024 / 1024),10,2)).arg(QString::number((double)(ReceivedFile::GetInstance()->fileMaxSize / 1024 / 1024),10,2)));
    ui->speedLabel->setText(QString("%1M/s").arg(QString::number(speed,10,4)));


    if(ReceivedFile::GetInstance()->fileSize == ReceivedFile::GetInstance()->fileMaxSize) {
        qDebug() << "fin" << ReceivedFile::GetInstance()->fileMaxSize << ReceivedFile::GetInstance()->fileSize;
        emit finished();
        return;
    }

    QTimer::singleShot(1000,this,SLOT(updateProgressBar()));
}

void ReceiverForm::onButtonCloseClicked()
{
    auto res = QMessageBox::warning(this,tr("警告"),tr("文件正在发送，是否关闭?"),QMessageBox::Yes,QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit cancelFile();
        this->close();
    }
}

void ReceiverForm::receiveFinished()
{
    QMessageBox::warning(this,tr("接收完成"),tr("文件接收完成！"),QMessageBox::Yes);
    this->close();
}

void ReceiverForm::on_cancelButton_clicked()
{
    auto res = QMessageBox::warning(this,tr("中断接收"),tr("是否取消接收文件?"),QMessageBox::Yes,QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit cancelFile();
        close();
    }
}
