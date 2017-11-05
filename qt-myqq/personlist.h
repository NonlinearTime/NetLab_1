#ifndef PERSONLIST_H
#define PERSONLIST_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QMenu>
#include "contact.h"
#include <personlistbuddy.h>

class PersonList : public QListWidget
{
    Q_OBJECT

public:
    explicit PersonList(QListWidget *parent=nullptr, QString username = nullptr);
    QMenu *blankMenu;
    QString username;
    QListWidgetItem *currentItem;
    Contact contact;
    void initMenu();
    int row;
    QMap<int,personlistbuddy*> ListMap;
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);


signals:
    void showDlgAdd();

public slots:
    void slotAddBuddy(QStringList info);
    void slotAddBuddy();


private slots:
    void showAddDlg();
};

#endif // PERSONLIST_H
