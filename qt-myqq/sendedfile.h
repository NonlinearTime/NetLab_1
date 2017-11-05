#ifndef SENDEDFILE_H
#define SENDEDFILE_H

#include <QObject>

class SendedFile
{
public:
    quint64 fileSize;
    quint64 fileMaxSize;
    static SendedFile* GetInstance();
    static void Release();
private:
    SendedFile();
    static SendedFile* pInstance;
};

#endif // SENDEDFILE_H
