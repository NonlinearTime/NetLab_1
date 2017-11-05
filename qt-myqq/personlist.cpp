#include "personlist.h"
#include "personlistbuddy.h"
#include "contact.h"
#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QWidget>
#include <publicsocket.h>
#include <QScrollBar>

PersonList::PersonList(QListWidget *parent, QString username) : QListWidget(parent)
{
    this->username = username;
    initMenu();
    //this->setStyleSheet("{background:none}");
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,0%);"
                                                       "margin:0px,0px,0px,0px;"
                                                       "padding-top:9px;"
                                                       "padding-bottom:9px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,25%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::handle:vertical:hover"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,50%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/3.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/1.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/4.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/2.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                       "{"
                                                       "background:rgba(0,0,0,10%);"
                                                       "border-radius:4px;"
                                                       "}"
                                                       );
}

void PersonList::initMenu()
{
    row = 0;
    blankMenu = new QMenu();
    QAction *addBuddy = new QAction("添加好友",this);
    blankMenu->addAction(addBuddy);

    connect(addBuddy,SIGNAL(triggered(bool)),this,SLOT(showAddDlg()));
}

void PersonList::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if (currentItem == NULL) return;
    else{
        //this->contact.changeUsername(this->userData());
        //qDebug() << this->contact.username;
        //ListMap.value(currentItem)->acount = "111";
        contact.signLabel->setText(ListMap.value(currentRow())->acount);
        PublicSocket::GetInstance()->sendData(QString("10 ") + ListMap.value(currentRow())->acount + QString(" ") + PublicSocket::GetInstance()->getNickName()[2]);
        qDebug() << currentItem;
        ListMap.value(currentRow())->acount;
        this->contact.show();
    }
}

void PersonList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));
}

void PersonList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);
    if (currentItem == NULL) {
        blankMenu->exec(QCursor::pos());
    }
    //blankMenu->exec(QCursor::pos());
}

void PersonList::slotAddBuddy()
{
    personlistbuddy *buddy = new personlistbuddy();
    buddy->headPath = ":/Resources/LoginWindow/HeadImage.png";
    this->username == "Haines" ? buddy->name->setText("Haines") : buddy->name->setText("...");
    //buddy->name->setText(info[0]);
    //buddy->sign->setText(info[1]);
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(60,60));
    ListMap.insert(row,buddy);
    //this->insertItem(row(currentItem) + item.count(),newItem);
    this->addItem(newItem);
    row++;
    this->setItemWidget(newItem,buddy);

}

void PersonList::showAddDlg()
{
    emit showDlgAdd();
}

void PersonList::slotAddBuddy(QStringList info)
{
    personlistbuddy *buddy = new personlistbuddy();
    buddy->headPath = ":/Resources/LoginWindow/HeadImage.png";
    //this->username == "Haines" ? buddy->name->setText("Haines") : buddy->name->setText("...");
    buddy->name->setText(info[0]);
    buddy->sign->setText(info[1]);
    buddy->acount = info[2];
    QList<QListWidgetItem*> item ;

    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(60,60));
    //this->insertItem(row(currentItem) + item.count(),newItem);
    //this->insertItem(item.count(),newItem);
    ListMap.insert(row,buddy);
    qDebug() << ListMap.value(row)->acount;
    qDebug() << currentItem;
    this->addItem(newItem);
    row++;
    this->setItemWidget(newItem,buddy);
}
