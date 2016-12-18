#include "s_tqdatetimewidget.h"
#include "s_tqlabel.h"
#include "s_tqchoosewidget.h"
#include "s_tqpushbutton.h"
#include "wd_func.h"
#include <QTimeEdit>
#include <QVBoxLayout>

s_tqDateTimeWidget::s_tqDateTimeWidget(QWidget *parent) : QWidget(parent)
{
//    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Дата:");
    hlyout->addWidget(lbl);
    s_tqChooseWidget *cw = new s_tqChooseWidget;
    cw->Setup("2.18..");
    cw->setObjectName("cw");
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Время:");
    hlyout->addWidget(lbl);
    QTimeEdit *te = new QTimeEdit;
    te->setAttribute(Qt::WA_TranslucentBackground);
    te->setObjectName("te");
    te->setDisplayFormat("hh:mm:ss");
    hlyout->addWidget(te);
    lyout->addLayout(hlyout);
    s_tqPushButton *pb = new s_tqPushButton("Готово");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(FinishedEditing()));
    lyout->addWidget(pb);
    setLayout(lyout);
}

void s_tqDateTimeWidget::FinishedEditing()
{
    QDateTime dtm;
    QString cwdata = WDFunc::CWData(this, "cw");
    QDate dte = QDate::fromString(cwdata, DATEFORMAT);
    QTime tme;
    QTimeEdit *te = this->findChild<QTimeEdit *>("te");
    if (te != 0)
        tme = QTime::fromString(te->text(), TIMEFORMAT);
    else
        tme = QTime();
    dtm.setDate(dte);
    dtm.setTime(tme);
    emit activated(dtm);
    this->close();
}

void s_tqDateTimeWidget::SetDateTime(const QDateTime &dtm)
{
    WDFunc::SetCWData(this, "cw", dtm.toString(DATEFORMAT));
    QTimeEdit *te = this->findChild<QTimeEdit *>("te");
    if (te != 0)
        te->setTime(dtm.time());
}
