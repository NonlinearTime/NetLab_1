#ifndef PERSONLISTBUDDY_H
#define PERSONLISTBUDDY_H

#include <QWidget>
#include <QLabel>
#include <QWidget>


class personlistbuddy : public QWidget
{
    Q_OBJECT
public:
    explicit personlistbuddy(QWidget *parent = nullptr);
    void initUi();
    QWidget *head;
    QLabel *name;
    QLabel *sign;
    QString headPath;
    QString acount;
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
};

#endif // PERSONLISTBUDDY_H
