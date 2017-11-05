#include "database.h"
#include <QDebug>

DataBase * DataBase::pInstance = NULL;

DataBase::DataBase()
{
    qDebug() << openDatabase();
    createTable();
}

bool DataBase::openDatabase()
{
    _db = QSqlDatabase::addDatabase(QString("QMYSQL"));
    _db.setDatabaseName("userinformation");
    _db.setHostName("localhost");
    _db.setPort(3306);
    _db.setUserName("root");
    _db.setPassword("lhm199710");
    query = QSqlQuery(_db);
    return _db.open();
}

void DataBase::createTable()
{
    query.exec("create table users ("
               "acount varchar(40),"
               "password varchar(40),"
               "nickname varchar(40),"
               "sign varchar(80),"
               "question varchar(80),"
               "qstanswer varchar(80),"
               "ipaddress varchar(40),"
               "port varchar(10),"
               "isonline varchar(1),"
               "listenIp varchar(40),"
               "listenport varchar(10),"
               "friendslist varchar(10000))");
}

DataBase *DataBase::GetInstance()
{
    if (pInstance == NULL) {
        pInstance = new DataBase();
        atexit(Release);
    }
    return pInstance;
}

void DataBase::Release()
{
    if (pInstance) {
        delete pInstance;
        pInstance = NULL;
    }
}

void DataBase::itemInsert(const QStringList &infoList)
{
    QString sentence;
    sentence = QString("insert into Users values('%1', '%2' , '%3', '%4', '%5', '%6', '%7', '%8', '%9'").arg(
                infoList[0],infoList[1],infoList[2],infoList[3],infoList[4],infoList[5],infoList[6],infoList[7],infoList[8])
            + QString(", '%1', '%2', '')").arg(infoList[9]).arg(infoList[10]);
    query.exec(sentence);
}

void DataBase::itemUpdate(const QString &column, const QString & item, const QString & row, const QString &rowVar)
{
    QString sentence;
    sentence = QString("update Users set %1 = '%2' where %3 = '%4'").arg(column,item,row,rowVar);
    query.exec(sentence);
}

void DataBase::itemDelete(const QString &row, const QString &rowVar)
{
    QString sentence;
    sentence = QString("delete from Users where %1 = '%2'").arg(row,rowVar);
    query.exec(sentence);
}

QSqlQuery& DataBase::itemQuery(const QString &row, const QString &rowVar)
{
    QString sentence;
    sentence = QString("select * from Users where %1 = '%2'").arg(row,rowVar);
    query.exec(sentence);
    return query;
}

int DataBase::tableNum()
{
    query.exec("select count(1) from Users");
    return query.record().count();
}

DataBase::~DataBase()
{
    _db.close();
}
