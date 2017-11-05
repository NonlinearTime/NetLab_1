#include "server.h"
#include <QApplication>
#include <database.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase::GetInstance();
    Server w;
    w.show();

    return a.exec();
}
