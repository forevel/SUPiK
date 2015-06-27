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
    void Setup(QString links, QString hdr="");

    void SetMainTvCurrentText (QString);
    void SetSlaveTvCurrentText (QString);

signals:
    void finished (QString, QString);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void accepted();
    void MainItemChoosed(QModelIndex);
    void SlaveItemChoosed(QModelIndex);
    void cancelled();

private:
    QString tblename, tablefield;

    void ShowErMsg(int);
};

#endif // S_2TDIALOG_H
