#ifndef OFFLINEMSGPOOL_H
#define OFFLINEMSGPOOL_H

#include <QString>
#include <QStringList>
#include <QMap>

class OffLineMsgPool
{
public:
    static OffLineMsgPool *GetInstance();
    static void Release();
    void addMsgPiece(const QString & userName,const QString & senderName,const QString & piece);
    void clearUserMsg(const QString & userName,const QString & senderName);
    void insertUser(const QString & userName,const QString & senderName);
    QStringList* getOfflineMsg(const QString & userName,const QString & senderName);

private:
    OffLineMsgPool();
    static OffLineMsgPool* pInstance;
    QMap<QString,QMap<QString,QStringList*>*> offLineMsgPool;
};

#endif // OFFLINEMSGPOOL_H
