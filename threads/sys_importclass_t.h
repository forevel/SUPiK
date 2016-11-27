#ifndef SYS_IMPORTCLASS_T_H
#define SYS_IMPORTCLASS_T_H

#include "../gen/publicclass.h"
#include <QMap>

#define ROOTSIZE    2 // размер корневого элемента

class sys_ImportClass_T : public QObject
{
    Q_OBJECT
public:
    explicit sys_ImportClass_T(QObject *parent = 0);

    struct imp_struct
    {
        QString filename;
        QMap<QString, QString> *map;
        bool istree;
        QString tablename;
    };

    void Set(imp_struct &data);

signals:
    void ProcessFinished();
    void RowProcessing(QString row);

public slots:
    void Run();

private:
    imp_struct ImpInitial;
    int RowNum;

private slots:
    void EmitRowProcessing();

protected:
};

#endif // SYS_IMPORTCLASS_T_H
