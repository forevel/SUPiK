#ifndef SYS_BACKUPRESTOREDIRDIALOG_H
#define SYS_BACKUPRESTOREDIRDIALOG_H

#define MAX_COL_NUM 20
#include <QDialog>
#include <QStringListModel>
#include "../../models/treemodel.h"

class sys_backuprestoredirdialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_backuprestoredirdialog(bool, QWidget *parent = 0);

signals:

public slots:

private:
    TreeModel *mainmodel;
    QStringListModel *dirM, *fM;
    void SetupUI();
    void updateView();
    bool isImport;
    QString filename;

private slots:
    void ImpExpPBPressed();
    void CancelPBPressed();
    void LoadFilePBPressed();
    void DirChoosed(QString);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SYS_BACKUPRESTOREDIRDIALOG_H
