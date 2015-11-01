#ifndef CMP_NEWSUBSECTION_H
#define CMP_NEWSUBSECTION_H

#include <QDialog>

#define CMPNSWARN WARNMSG(PublicClass::ER_CMPNS, __LINE__)
#define CMPNSDBG  DBGMSG(PublicClass::ER_CMPNS, __LINE__)
#define CMPNSINFO(a)  INFOMSG(PublicClass::ER_CMPNS, __LINE__, a)

class cmp_newsubsection : public QDialog
{
    Q_OBJECT
public:
    explicit cmp_newsubsection(int CompType, QWidget *parent=0);
    ~cmp_newsubsection();

private:
    bool SomethingChanged;
    int CompType;
    void SetupUI();

private slots:
    void transliteDirName();
    void SetSomethingChanged();
    void Ok();
    void Cancel();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // CMP_NEWSUBSECTION_H
