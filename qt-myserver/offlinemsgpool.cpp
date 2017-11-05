#include "offlinemsgpool.h"

OffLineMsgPool* OffLineMsgPool::pInstance = NULL;

OffLineMsgPool *OffLineMsgPool::GetInstance()
{
    if(pInstance == NULL) {
        pInstance = new OffLineMsgPool();
        atexit(Release);
    }
    return pInstance;
}

void OffLineMsgPool::Release()
{
    if (pInstance) {
        delete pInstance;
        pInstance = NULL;
    }
}

void OffLineMsgPool::addMsgPiece(const QString &userName,const QString & senderName ,const QString &piece)
{
    insertUser(userName,senderName);
    offLineMsgPool.value(userName)->value(senderName)->append(piece);
}

void OffLineMsgPool::clearUserMsg(const QString &userName,const QString & senderName)
{
    if (offLineMsgPool.find(userName) == offLineMsgPool.end())
        return;
    else if (offLineMsgPool.value(userName)->find(senderName) == offLineMsgPool.value(userName)->end())
        return;
    else
        offLineMsgPool.value(userName)->value(senderName)->clear();
}

void OffLineMsgPool::insertUser(const QString &userName, const QString &senderName)
{
    if (offLineMsgPool.find(userName) == offLineMsgPool.end()) {
        offLineMsgPool.insert(userName,new QMap<QString,QStringList*>);
        offLineMsgPool.value(userName)->insert(senderName,new QStringList());
    }
    else {
        if (offLineMsgPool.value(userName)->find(senderName) == offLineMsgPool.value(userName)->end())
            offLineMsgPool.value(userName)->insert(senderName,new QStringList());
    }
}

QStringList *OffLineMsgPool::getOfflineMsg(const QString &userName, const QString &senderName)
{
    return offLineMsgPool.value(userName)->value(senderName);
}

OffLineMsgPool::OffLineMsgPool()
{

}

