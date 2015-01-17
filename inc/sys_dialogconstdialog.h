#ifndef SYS_DIALOGCONSTDIALOG_H
#define SYS_DIALOGCONSTDIALOG_H

#include <QDialog>
#include "../inc/s_ncdialog.h"

class sys_dialogconstdialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_dialogconstdialog(QWidget *parent = 0); // создание нового диалога
/*    explicit sys_dialogconstdialog(QString dialog, QWidget *parent = 0); // редактирование существующего диалога (dialog=dialogs из таблицы dialogs)

signals:

public slots:


private:
    QList<constritem *> DItem; // набор экземпляров элементов диалога
    int KeyField; // номер ключевого поля в диалоге */
};

class constritem
{
public:
//    int AddItem(int floc, int fname, int ftype, int ltype, int relfnum, QString link, bool isKeyField);
//    int DeleteItem(int fnum);

private:
    int FieldNum; // порядковый номер поля в диалоге
    int FieldLoc; // местонахождение поля в диалоге (см. СУПиК РП)
    int FieldName; // имя поля, см. описание поля dialogsfields в СУПиК РП
    int FieldType; // тип поля (из семейства FT_)
    int LinkType; // тип ссылки (из семейства L_)
    int RelFieldNum; // номер поля, от информации в котором зависит поведение данного поля (ссылка на FieldNum)
    QStringList Link; // ссылка в формате <db>.<tble>...

};

#endif // SYS_DIALOGCONSTDIALOG_H
