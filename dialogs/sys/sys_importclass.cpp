#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QHeaderView>
#include <QPaintEvent>
#include <QPainter>
#include <QtXlsx/xlsxdocument.h>
#include "sys_importclass.h"
#include "../../threads/sys_importclass_t.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../messagebox.h"

sys_ImportClass::sys_ImportClass(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void sys_ImportClass::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));

    event->accept();
}

void sys_ImportClass::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqTableView *mainTV = new s_tqTableView;
    mainTV->setObjectName("maintv");
    mainTV->verticalHeader()->setVisible(false);
    s_tqPushButton *pb = new s_tqPushButton("Импорт");
    pb->setIcon(QIcon(":/res/import.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(ImpExpPBPressed()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(CancelPBPressed()));
    hlyout->addWidget(pb);
    s_tqLabel *lbl = new s_tqLabel("Файл:");
    s_tqChooseWidget *cw = new s_tqChooseWidget;
    cw->setObjectName("loadfilecw");
    cw->Setup("2.15..XLSX Documents (*_xlsx)");
    hlyout->addWidget(lbl);
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    lyout->addWidget(mainTV, 100);
    setLayout(lyout);
}

void sys_ImportClass::ImpExpPBPressed()
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("loadfilecw");
    if (cw == 0)
    {
        MessageBox::information(this,"","Элемент CW не найден");
        return;
    }
    sys_ImportClass_T *t = new sys_ImportClass_T(cw->Value());
    WWidget = new WaitWidget;
    WWidget->setObjectName("waitwidget");
    connect(t,SIGNAL(RowProcessing(QString)),WWidget,SLOT(SetMessage(QString)));
    connect(t,SIGNAL(ProcessFinished()),this,SLOT(SysICTFinished()));
    connect(t,SIGNAL(finished()),t,SLOT(deleteLater()));
    t->start();
    WWidget->Start();
}

void sys_ImportClass::CancelPBPressed()
{
    this->close();
}

void sys_ImportClass::SysICTFinished()
{
    WWidget->Stop();
}
