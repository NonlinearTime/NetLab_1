#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QString>
#include <QSqlRecord>

class DataBase
{
private:
    DataBase();

    static DataBase* pInstance;

    QSqlDatabase _db;

    QSqlQuery query;

    bool openDatabase();

    void createTable();

public:
    static DataBase* GetInstance();

    static void Release();

    void itemInsert(const QStringList & infoList);

    void itemUpdate(const QString & column, const QString & item, const QString & row, const QString &rowVar);

    void itemDelete(const QString &row, const QString &rowVar);

    QSqlQuery& itemQuery(const QString &row, const QString &rowVar);

    int tableNum();

    ~DataBase();
};

#endif // DATABASE_H
