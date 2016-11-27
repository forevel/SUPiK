#include <QApplication>
#include <QtXlsx/xlsxdocument.h>
#include <QTimer>
#include "sys_importclass_t.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"

sys_ImportClass_T::sys_ImportClass_T(QObject *parent) : QObject(parent)
{
}

void sys_ImportClass_T::Set(imp_struct &data)
{
    ImpInitial = data;
}

void sys_ImportClass_T::Run()
{
    QTimer *tmr = new QTimer;
    tmr->setInterval(500);
    connect(tmr,SIGNAL(timeout()),this,SLOT(EmitRowProcessing()));
    tmr->start();
    QXlsx::Document TableDoc(ImpInitial.filename.toUtf8());
    RowNum = 1;
    QStringList sl;
    QString field = "ИД";
    tfl.tablefields(ImpInitial.tablename, field, sl);
    if (tfl.result == TFRESULT_ERROR)
    {
        ERMSG("Проблема получения данных из таблицы "+ImpInitial.tablename);
        emit ProcessFinished();
        return;
    }
    QString DbTble = sl.at(0);
    sl = DbTble.split(".");
    if (sl.size() < 2)
    {
        ERMSG("Ошибка в таблице tablefields");
        emit ProcessFinished();
        return;
    }
    QString TableName = sl.at(1);
    QString TableDB = sl.at(0);
    QStringList TableFields; // подготовка списка полей для записи
    QMap<QString, QString>::const_iterator i;
    for (i=ImpInitial.map->constBegin(); i!=ImpInitial.map->constEnd(); i++)
    {
        QStringList tmpsl;
        QString Key = i.key();
        tfl.tablefields(ImpInitial.tablename, Key, tmpsl);
        if (tmpsl.size()>1)
            TableFields.append(tmpsl.at(1));
        else
        {
            ERMSG("Пустой ответ");
            break;
        }
    }
    if (ImpInitial.istree)
    {
        QString ParentID;
        TableFields.append("idalias");
        while (true)
        {
            QString ClassNum = TableDoc.read(RowNum,1).toString();
            QStringList TableValues;
            for (int j=0; j<TableFields.size()-1; j++) // кроме idalias
            {
                QString tmps = ImpInitial.map->values().at(j);
                if (tmps.isEmpty())
                    continue;
                if (tmps.at(0) == "_") // значение по умолчанию
                {
                    tmps.remove(0, 1);
                    TableValues.append(tmps);
                }
                else
                {
                    bool ok;
                    int tmpi = tmps.toInt(&ok);
                    if (!ok)
                        continue;
                    TableValues.append(TableDoc.read(RowNum,tmpi).toString());
                }
            }
            if (ClassNum.isEmpty()) // пустое поле в первой колонке
                break;
            int position = ClassNum.size();
            if (position == ROOTSIZE)
                ParentID = "0";
            else
            {
                QString ParentClass = ClassNum.left(position-1);
                ParentID = sqlc.GetValueFromTableByField(TableDB,TableName,"id"+TableName,TableName,ParentClass);
                if (sqlc.result)
                {
                    ERMSG("Не найден родитель для класса "+ClassNum);
                    emit ProcessFinished();
                    return;
                }
            }
            QString tmps = sqlc.GetValueFromTableByField(TableDB,TableName,"id"+TableName,TableName,ClassNum);
            TableValues.append(ParentID);
            if (sqlc.result == 1) // нет такой записи
            {
                sqlc.InsertValuesToTable(TableDB,TableName, TableFields, TableValues);
                if (sqlc.result)
                {
                    ERMSG("Проблема при записи очередного поля № "+QString::number(RowNum));
                    emit ProcessFinished();
                    return;
                }
            }
            else if (sqlc.result == 0)
            {
                sqlc.UpdateValuesInTable(TableDB,TableName,TableFields,TableValues,"id"+TableName,tmps);
                if (sqlc.result)
                {
                    ERMSG("Проблема при обновлении очередного поля № "+QString::number(RowNum));
                    emit ProcessFinished();
                    return;
                }
            }
            else
            {
                emit ProcessFinished();
                ERMSG("Проблема при получении данных по очередному полю № "+QString::number(RowNum));
                return;
            }
            RowNum++;
        }
    }
    else
    {
        while (true)
        {
            QStringList TableValues;
            bool RecordIsEmpty = true;
            for (int j=0; j<TableFields.size(); j++) // кроме idalias
            {
                QString tmps = ImpInitial.map->values().at(j);
                if (tmps.isEmpty())
                    continue;
                if (tmps.at(0) == "_") // значение по умолчанию
                {
                    tmps.remove(0, 1);
                    TableValues.append(tmps);
                }
                else
                {
                    bool ok;
                    int tmpi = tmps.toInt(&ok);
                    if (!ok)
                        continue;
                    QString tmpString = TableDoc.read(RowNum,tmpi).toString();
                    TableValues.append(tmpString);
                    if (!tmpString.isEmpty())
                        RecordIsEmpty = false;
                }
            }
            if (RecordIsEmpty) // пустая строка
                break;
            QString tmps = sqlc.GetValueFromTableByFields(TableDB,TableName,"id"+TableName,TableFields,TableValues);
            if (sqlc.result == SQLC_EMPTY) // нет такой записи
            {
                sqlc.InsertValuesToTable(TableDB, TableName, TableFields, TableValues);
                if (sqlc.result != SQLC_OK)
                {
                    ERMSG("Проблема при записи очередного поля № "+QString::number(RowNum));
                    emit ProcessFinished();
                    return;
                }
            }
            else if (sqlc.result == SQLC_OK)
            {
                sqlc.UpdateValuesInTable(TableDB,TableName,TableFields,TableValues,"id"+TableName,tmps);
                if (sqlc.result != SQLC_OK)
                {
                    ERMSG("Проблема при обновлении очередного поля № "+QString::number(RowNum));
                    emit ProcessFinished();
                    return;
                }
            }
            else
            {
                emit ProcessFinished();
                ERMSG("Проблема при получении данных по очередному полю № "+QString::number(RowNum));
                return;
            }
            RowNum++;
        }
    }
    INFOMSG("Импорт завершён успешно!");
    emit ProcessFinished();
}

void sys_ImportClass_T::EmitRowProcessing()
{
    QString tmps = "Обработка строки " + QString::number(RowNum);
    emit RowProcessing(tmps);
}
