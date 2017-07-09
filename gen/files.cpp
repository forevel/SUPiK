#include "files.h"
#include "publicclass.h"

Files::Files()
{

}

QFile *Files::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly))
    {
        WARNMSG("Ошибка открытия файла "+fileName+" для записи");
        return Q_NULLPTR;
    }
    return file.take();
}

QFile *Files::openFileForRead(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::ReadOnly))
    {
        WARNMSG("Ошибка открытия файла "+fileName+" для чтения");
        return Q_NULLPTR;
    }
    return file.take();
}
