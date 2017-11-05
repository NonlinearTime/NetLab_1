#ifndef MESSAGAITEM_H
#define MESSAGAITEM_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QStringList>

class MessagaItem : public QWidget
{
    Q_OBJECT
public:
    explicit MessagaItem(QString msg, bool isOnlineMsg, QWidget *parent = nullptr);
    QLabel *newMsg;
    QLabel *newDate;
    QSize labelSize;

signals:

public slots:
};

#endif // MESSAGAITEM_H
