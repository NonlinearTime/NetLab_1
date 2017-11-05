#ifndef SENDERFORM_H
#define SENDERFORM_H

#include <QWidget>
#include "sendedfile.h"
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class SenderForm;
}

class SenderForm : public QWidget
{
    Q_OBJECT

public:
    explicit SenderForm(QWidget *parent = 0);
    ~SenderForm();

public slots:
    void setFileMaxSize();

    void updateProgressBar();

private slots:
    void onButtonCloseClicked();



    void sendFinished();

    void on_cancelButton_clicked();

private:
    Ui::SenderForm *ui;

    double fileSended;
    double filePerSecond;

signals:
    void cancelFile();
    void finished();
};

#endif // SENDERFORM_H
