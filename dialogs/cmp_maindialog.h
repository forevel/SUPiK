#ifndef CMP_MAINDIALOG_H
#define CMP_MAINDIALOG_H

#include <QDialog>

class cmp_maindialog : public QDialog
{
    Q_OBJECT

public:
    explicit cmp_maindialog(QWidget *parent = 0);
    ~cmp_maindialog();
    void SetupUI(int CompType, int CompTable, int CompID); // подготовка диалога к отображению. 1 = тип компонента (Altium,Schemagee,...),
                                                            // 2 = номер таблицы компонента по таблице description, 3 = ИД компонента по таблице

signals:
    void error(int,int);

private:
};

#endif // CMP_MAINDIALOG_H
