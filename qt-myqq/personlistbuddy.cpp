#include "personlistbuddy.h"
#include <QPainter>
#include <QEvent>

personlistbuddy::personlistbuddy(QWidget *parent) : QWidget(parent)
{
    initUi();
}

void personlistbuddy::initUi()
{
    head = new QWidget(this);
    name = new QLabel(this);
    sign = new QLabel(this);

    //set the size of head
    head->setFixedSize(40,40);
    //set the sign
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);

    //location
    head->move(7,7);
    name->move(54,7);
    sign->move(54,30);

    //load the eventfilter
    head->installEventFilter(this);
}

bool personlistbuddy::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == head) {
        if (event->type() == QEvent::Paint) {
            QPainter painter(head);
            painter.drawPixmap(head->rect(),QPixmap(headPath));
        }
    }
    return QWidget::eventFilter(watched,event);
}
