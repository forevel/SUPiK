#include <QApplication>
#include <QtXlsx/xlsxdocument.h>
#include <QTimer>
#include "sys_importclass_t.h"
#include "../gen/s_sql.h"

sys_ImportClass_T::sys_ImportClass_T(QString Filename, QObject *parent) : QThread(parent)
{
    this->Filename = Filename;
    QTimer *tmr = new QTimer;
    tmr->setInterval(500);
    connect(tmr,SIGNAL(timeout()),this,SLOT(EmitRowProcessing()));
    tmr->start();
}

void sys_ImportClass_T::run()
{
    QXlsx::Document TableDoc(Filename.toUtf8());
    RowNum = 1;
    QString ParentID;
    QString TableName = "eskd_class";
    QString TableDB = "ent";
    while (true)
    {
        QString ClassNum = TableDoc.read(RowNum,1).toString();
        QString ClassDescription = TableDoc.read(RowNum, 2).toString();
        if (ClassNum.isEmpty())
            break;
        int position = ClassNum.size();
        if (position == ROOTSIZE)
            ParentID = "0";
        else
        {
            QString ParentClass = ClassNum.left(position-1);
            ParentID = sqlc.GetValueFromTableByField(sqlc.GetDB(TableDB),TableName,"id"+TableName,TableName,ParentClass);
            if (sqlc.result)
            {
                SYSICTER("Не найден родитель для класса "+ClassNum);
                emit ProcessFinished();
                return;
            }
        }
        QString tmps = sqlc.GetValueFromTableByField(sqlc.GetDB(TableDB),TableName,"id"+TableName,TableName,ClassNum);
        QStringList fl = QStringList() << TableName << "description" << "idalias";
        QStringList vl = QStringList() << ClassNum << ClassDescription << ParentID;
        if (sqlc.result == 1) // нет такой записи
        {
            sqlc.InsertValuesToTable(sqlc.GetDB(TableDB),TableName, fl, vl);
            if (sqlc.result)
            {
                SYSICTER("Проблема при записи очередного поля № "+QString::number(RowNum));
                emit ProcessFinished();
                return;
            }
        }
        else if (sqlc.result == 0)
        {
            sqlc.UpdateValuesInTable(sqlc.GetDB(TableDB),TableName,fl,vl,"id"+TableName,tmps);
            if (sqlc.result)
            {
                SYSICTER("Проблема при обновлении очередного поля № "+QString::number(RowNum));
                emit ProcessFinished();
                return;
            }
        }
        else
        {
            emit ProcessFinished();
            SYSICTER("Проблема при получении данных по очередному полю № "+QString::number(RowNum));
            return;
        }
        RowNum++;
    }
    SYSICTINFO("Импорт завершён успешно!");
    emit ProcessFinished();
}

void sys_ImportClass_T::EmitRowProcessing()
{
    QString tmps = "Обработка строки " + QString::number(RowNum);
    emit RowProcessing(tmps);
}
