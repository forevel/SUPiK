#ifndef SYS_IMPORTCLASS_T_H
#define SYS_IMPORTCLASS_T_H

#include <QThread>
#include "../gen/publicclass.h"

#define ROOTSIZE    2 // размер корневого элемента

#define SYSICTER(a)     ERMSG(PublicClass::ER_SYSICT, __LINE__, a)
#define SYSICTINFO(a)   INFOMSG(PublicClass::ER_SYSICT, __LINE__, a)

class sys_ImportClass_T : public QThread
{
    Q_OBJECT
public:
    explicit sys_ImportClass_T(QString Filename, QObject *parent = 0);

signals:
    void ProcessFinished();
    void RowProcessing(QString row);

public slots:

private:
    QString Filename;
    int RowNum;

private slots:
    void EmitRowProcessing();

protected:
    void run();
};

#endif // SYS_IMPORTCLASS_T_H
