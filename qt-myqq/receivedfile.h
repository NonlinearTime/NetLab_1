#ifndef RECEIVEDFILE_H
#define RECEIVEDFILE_H

#include <QObject>

class ReceivedFile
{
public:
    static ReceivedFile* GetInstance();
    static void Release();
    quint64 getReceivedSize();
    quint64 fileSize;
    quint64 fileMaxSize;

private:
    ReceivedFile();
    static ReceivedFile* pInstance;


};

#endif // RECEIVEDFILE_H
