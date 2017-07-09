#ifndef FILES_H
#define FILES_H

#include <QFile>

class Files
{
public:
    Files();

    QFile *openFileForWrite(const QString &fileName);
    QFile *openFileForRead(const QString &fileName);
};

#endif // FILES_H
