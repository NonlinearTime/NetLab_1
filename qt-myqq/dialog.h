#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QtNetwork>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <mysocket.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString username;



private:
    Ui::Dialog *ui;
    QPushButton *m_keyboardButton;
    QTcpSocket *tcpSocket;
    //qintptr socketDescriptor;
    QHostAddress hostAddress;
    quint16 hostPort;
    quint16 blockSize;
    QString message;
    QDialog *findPwd;
    QDialog *rstDialog;
    QLineEdit *answerLineEdit;
    QLineEdit *newPwdLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *questionLineEdit;
    QLineEdit *oldPwdLineEdit;

    QLineEdit *regAnsLineEdit;
    QLineEdit *regPwdLineEdit;
    QLineEdit *regNameLineEdit;
    QLineEdit *regQstLineEdit;
    QLineEdit *regDPwdLineEdit;
    QLineEdit *regNickLineEdit;
    QLineEdit *regSignLineEdit;

    void newConnection();
    void messageDistribution();
    void sendMessage(const QString & seq);


private slots:
    void on_pushButton_clicked();
    void onButtonMinClicked();
    void onButtonCloseClicked();
    void readMessage();
    void sendNameAndPwd();
    void displayError(QAbstractSocket::SocketError);
    void showFindPwdDlg();
    void showRegDlg();
    void sendNewNameAndPwd();
    void changedSuccessfully();
    void sendRegNameAndPwd();
    void registeredSuccessfull();
    void read(QString msg);

signals:
    void revData();

public slots:
    void recvData();

};

#endif // DIALOG_H
