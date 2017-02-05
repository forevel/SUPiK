#ifndef DEV_DEVDIALOG_H
#define DEV_DEVDIALOG_H

#include <QDialog>

class dev_devdialog : public QDialog
{
    Q_OBJECT
public:
    explicit dev_devdialog(QWidget *parent = 0);
    ~dev_devdialog();

signals:

public slots:

private:
    void SetupUI();
    void ResizeMainTV();
    QString GetIndex(int column);

private slots:
    void AddNewDev();
    void EditDev();
    void DeleteDev();
    void MainContextMenu(QPoint pt);
    void Filter();
    void Unfilter();
    void Refresh();

protected:
    void paintEvent(QPaintEvent *);


};

#endif // DEV_DEVDIALOG_H
