#ifndef RECEIVERFORM_H
#define RECEIVERFORM_H

#include <QWidget>
#include <QMessageBox>
#include "receivedfile.h"
#include <QTimer>

namespace Ui {
class ReceiverForm;
}

class ReceiverForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiverForm(QWidget *parent = 0);
    ~ReceiverForm();

public slots:
    void setFileMaxSize();

    void updateProgressBar();

private slots:
    void onButtonCloseClicked();

    void receiveFinished();

    void on_cancelButton_clicked();

private:
    Ui::ReceiverForm *ui;

    double fileReceived;
    double filePerSecond;

signals:
    void cancelFile();
    void finished();
};

#endif // RECEIVERFORM_H
