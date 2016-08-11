#ifndef TB_MAINDIALOG_H
#define TB_MAINDIALOG_H

#define TBMWARN    WARNMSG(PublicClass::ER_TBM, __LINE__)
#define TBMDBG     DBGMSG(PublicClass::ER_TBM, __LINE__)
#define TBMINFO(a) INFOMSG(PublicClass::ER_TBM, __LINE__,a)

#include <QDialog>
#include "../../models/treemodel.h"

class tb_maindialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit tb_maindialog(QWidget *parent = 0);
    ~tb_maindialog();

signals:

public slots:

private slots:
    void GroupChoosed();

protected:
    void paintEvent(QPaintEvent *);

private:

    void SetupUI();
};

#endif // tb_maindialog_H
