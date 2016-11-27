#ifndef TB_MAINDIALOG_H
#define TB_MAINDIALOG_H

#include <QDialog>

class tb_maindialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit tb_maindialog(QWidget *parent = 0);
    ~tb_maindialog();

signals:

public slots:

private slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    void SetupUI();
};

#endif // tb_maindialog_H
