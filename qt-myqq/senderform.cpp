#include "senderform.h"
#include "ui_senderform.h"
#include <QDebug>

SenderForm::SenderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SenderForm)
{
    ui->setupUi(this);

    fileSended = 0;
    filePerSecond = 0;

    //setFileMaxSize();

    //updateProgressBar();

    connect(this,SIGNAL(finished()),this,SLOT(sendFinished()));
}

SenderForm::~SenderForm()
{
    delete ui;
}

void SenderForm::setFileMaxSize()
{
    ui->progressBar->setMaximum(SendedFile::GetInstance()->fileMaxSize);
}

void SenderForm::updateProgressBar()
{
    qDebug() << SendedFile::GetInstance()->fileSize;
    ui->progressBar->setValue(SendedFile::GetInstance()->fileSize);

    filePerSecond = SendedFile::GetInstance()->fileSize  - fileSended;
    fileSended = SendedFile::GetInstance()->fileSize ;
    float speed = filePerSecond/ 1024 /1024;
    ui->sizeLabel->setText(QString("%1M / %2M").arg(QString::number((double)(fileSended / 1024 / 1024),10,2)).arg(QString::number((double)(SendedFile::GetInstance()->fileMaxSize / 1024 / 1024),10,2)));
    ui->speedLabel->setText(QString("%1M/s").arg(QString::number(speed,10,4)));

    if(SendedFile::GetInstance()->fileSize == SendedFile::GetInstance()->fileMaxSize) {
        emit finished();
        return;
    }

    QTimer::singleShot(1000,this,SLOT(updateProgressBar()));
}

void SenderForm::onButtonCloseClicked()
{
    auto res = QMessageBox::warning(this,tr("警告"),tr("文件正在发送，是否关闭?"),QMessageBox::Yes,QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit cancelFile();
        this->close();
    }
}

void SenderForm::sendFinished()
{
    QMessageBox::warning(this,tr("发送完成"),tr("文件发送完成！"),QMessageBox::Yes);
    this->close();
}


void SenderForm::on_cancelButton_clicked()
{
    auto res = QMessageBox::warning(this,tr("中断发送"),tr("是否取消发送文件?"),QMessageBox::Yes,QMessageBox::No);
    if (res == QMessageBox::Yes) {
        emit cancelFile();
        this->close();
    }

}
//void SenderForm::on
