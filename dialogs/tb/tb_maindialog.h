#ifndef TB_MAINDIALOG_H
#define TB_MAINDIALOG_H

#include <QDialog>
#include "../../models/baseitemmodel.h"
#include "../../models/proxymodel.h"

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

public:
    explicit tb_maindialog(QWidget *parent = 0);
    ~tb_maindialog();

signals:

public slots:

private slots:
    void ShowPers();
    void ShowPersDlg();
    void TvContext(QPoint);
    void AddPers();
    void SetPers(const QString &pers);
    void Refresh();

protected:
    void paintEvent(QPaintEvent *);

private:
    void SetupUI();
    void SetupModel();
    void SetMainModelData(int row, int column, const QString &data, const QColor &color);

    QList<PersTB> TBList;
    BaseItemModel *MainModel;
    ProxyModel *PModel;
    QString Pers;
    int Mode;
};


#endif // tb_maindialog_H
