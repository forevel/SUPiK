#include "cmp_newsubsection.h"
#include "../gen/publicclass.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "../widgets/s_tqchoosewidget.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"

cmp_newsubsection::cmp_newsubsection(QWidget *parent) : QDialog(parent)
{
    SomethingChanged = false;
    SetupUI();
}

cmp_newsubsection::~cmp_newsubsection()
{

}

void cmp_newsubsection::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/CompWallpaper.jpg"));

    event->accept();
}

void cmp_newsubsection::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Имя категории:");
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("name");
    connect(le, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Имя в БД:");
    le = new s_tqLineEdit;
    le->setObjectName("tblename");
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Группа параметров:");
    s_tqChooseWidget *cw = new s_tqChooseWidget;
    cw->Setup("2.2..Наборы параметров_сокращ.Наименование");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SetSomethingChanged()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(cw, 1);
    lyout->addLayout(hlyout);
}

void cmp_newsubsection::transliteDirName()
{
    s_tqLineEdit *le1 = this->findChild<s_tqLineEdit *>("name");
    s_tqLineEdit *le2 = this->findChild<s_tqLineEdit *>("tblename");
    if ((le1 == 0) || (le2 == 0))
    {
        CMPNSDBG;
        return;
    }
    if (le2->text().isEmpty())
    {
        QString tmpString = pc.getTranslit(le1->text());
        le2->setText(tmpString);
    }
}

void cmp_newsubsection::SetSomethingChanged()
{
    SomethingChanged = true;
}
