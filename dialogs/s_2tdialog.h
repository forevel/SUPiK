#ifndef S_2TDIALOG_H
#define S_2TDIALOG_H

// диалог с двумя таблицами - для выбора сначала из одной, потом на основании первого выбора выбора из другой таблицы

#include <QDialog>
#include "../models/proxymodel.h"
#include "../models/treemodel.h"
#include "../widgets/treeview.h"

class s_2tdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2tdialog(QStringList links, QString MainSlaveItemId, QString hdr="", QWidget *parent = 0);
    void SetupUI(QStringList links, QString, QString MainSlaveItemId);
    void Setup();

signals:
    void finished (QString);

protected:
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void MainItemChoosed();
    void SlaveItemChoosed();

private:
    QString MainDb, MainTble, MainTableField, MainTableFieldInRus;
    TreeModel *MainModel, *SlaveModel;
    ProxyModel *MainProxyModel, *SlaveProxyModel;
    bool FirstRun;

    void FindAndSetId(TreeView *tv, QString id);
    QString TvData(TreeView *tv, int row, int column);
    void Refresh();
    void ResizeTv(TreeView *tv);
};

#endif // S_2TDIALOG_H
