#include "messagaitem.h"
#include <QDebug>
#include <QPainter>

MessagaItem::MessagaItem(QString msg,bool isOnlineMsg,QWidget *parent) : QWidget(parent)
{
    newMsg = new QLabel(this);
    newDate = new QLabel(this);

    int stringWeigt = 0;
    int initIndex = 0;
    int num = 0;
    QStringList rstList;
    QString rstString;
    QString rstDate;

    QStringList sList = msg.split("\n");
    //qDebug() << sList;
    for(auto item = sList.begin(); item != sList.end() ; ++item) {
        //qDebug() << *item;
        QString i = *item;
        //qDebug() << i.left(10);
        initIndex = 0;
        for (int c = 0 ; c != i.size() ; c++) {
            auto ca = i[c];
            if(ca >= 0 && ca <=127)
                stringWeigt += 1;
            else {
                stringWeigt += 2;
                //c++;
            }
            num++;
            if(stringWeigt >=30) {
                //qDebug() << i.mid(initIndex,num);
                rstList.append(QString("") + i.mid(initIndex,num) + QString("\n"));
                //qDebug() << stringWeigt;
                //qDebug() << num;
                initIndex = c;
                stringWeigt = 0;
                num=0;
            }
        }
        //qDebug() << i.mid(initIndex,num);
        if (num)
            rstList.append(QString("") + i.mid(initIndex,num) + QString("\n"));
        num = 0;
        stringWeigt = 0;
    }
    rstDate = rstList[0];
    rstList.pop_front();
    for (int i = 0 ; i < rstList.size() ; i ++) {
        qDebug() << rstList[i];
        rstString.append(rstList[i]);
    }
    qDebug() << rstString;

    //newDate->setWordWrap(true);
    newDate->setAlignment(Qt::AlignTop);
    newDate->setText(rstDate);
    newDate->adjustSize();
    newDate->setScaledContents(true);
    newDate->setFixedSize(206,20);
    qDebug() << newDate->size();

    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    newDate->setPalette(color);
    newDate->setStyleSheet("QLabel{font: 7pt 'Arial'}"
                           "QLabel{background-color: rgb(234, 246, 255)}");

    newMsg->setMargin(13);
    newMsg->setWordWrap(true);
    newMsg->setAlignment(Qt::AlignTop);
    newMsg->setText(rstString);
    newMsg->adjustSize();
    newMsg->setScaledContents(true);
    qDebug() << newMsg->size();
    newMsg->setFixedSize(newMsg->size().width(),newMsg->size().height()-10);
    labelSize = newMsg->size();

    if(isOnlineMsg) {
        newMsg->setStyleSheet("QLabel{background-color: rgb(255,255,255)}"
                              "QLabel{font: 9pt 'Arial'}"
                              "QLabel{border-radius:8px}"
                             );
        newMsg->move(newMsg->pos().x(),newMsg->pos().y() + 25);
    } else {
        newMsg->setStyleSheet("QLabel{background-color: rgb(30, 139, 255)}"
                              "QLabel{font: 9pt 'Arial'}"
                              "QLabel{border-radius:8px}"
                             );
        newMsg->move(517 - labelSize.width(),newMsg->pos().y() + 25);
    }

    newDate->move(286 - newDate->width() / 2,newDate->pos().y() + 5);

}
