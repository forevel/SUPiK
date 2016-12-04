#ifndef TB_MAINDIALOG_H
#define TB_MAINDIALOG_H

#include <QDialog>
#include "../../models/baseitemmodel.h"

#define TBDATE_OK   0
#define TBDATE_WARN 1
#define TBDATE_BAD  2

#define DAYS_TO_BAD 14

#define TB_NODATA   "N/A"

class tb_maindialog : public QDialog
{
    Q_OBJECT
    
    struct PersTB
    {
        QString FIO; // ФИО
        QString Group; // группа по электробезопасности
        QString MedDate; // дата профосмотра
        QString PbDate; // пож. безопасность
        QString OtDate; // охрана труда
        QString TbDate; // дата экзамена по ЭБ
    };

    enum DateTypes
    {
        DT_MED,
        DT_OT,
        DT_PB,
        DT_TB
    };

public:
    explicit tb_maindialog(QWidget *parent = 0);
    ~tb_maindialog();

signals:

public slots:

private slots:
    void ShowPers(QModelIndex);

protected:
    void paintEvent(QPaintEvent *);

private:
    void SetupUI();
    void SetupModel();
    int CheckDate(int type, const QString &date);
    void SetMainModelData(int row, int column, const QString &data, const QColor &color);

    QList<PersTB> TBList;
    BaseItemModel *MainModel;
};


#endif // tb_maindialog_H
