#ifndef S_2TDIALOG_H
#define S_2TDIALOG_H

// диалог с двумя таблицами - для выбора сначала из одной, потом на основании первого выбора выбора из другой таблицы

#include <QDialog>
#include <QSortFilterProxyModel>

class s_2tdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2tdialog(QWidget *parent = 0);
    void SetupUI(QString);
    void Setup(QStringList links, QString cursel="", QString hdr="");

    void SetMainTvCurrentText (QString);
    void SetSlaveTvCurrentText (QString);

signals:
    void finished (QString);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void accepted();
    void MainItemChoosed(QModelIndex);
    void SlaveItemChoosed(QModelIndex);
    void cancelled();
    void resizemainTV();
    void resizeslaveTV();

private:
    QString tblename, tblefield, tablefield;
};

#endif // S_2TDIALOG_H
