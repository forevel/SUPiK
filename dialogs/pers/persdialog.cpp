#include "persdialog.h"
#include <QAction>
#include <QIcon>
#include <QImage>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "../s_2cdialog.h"
#include "../../models/proxymodel.h"
#include "../../models/griddelegate.h"
#include "../../models/treemodel.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/wgenfunc.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

PersDialog::PersDialog(QString PersID, int DialogType, QWidget *parent) : QDialog(parent)
{
    this->DialogType = DialogType;
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (Fill(PersID))
    {
        WARNMSG("");
        return;
    }
}

PersDialog::~PersDialog()
{

}

void PersDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/PersWallPaper.png"));

    event->accept();
}

// Настройка интерфейса

void PersDialog::SetupUI(int DialogType)
{
    QVBoxLayout *vlyout1 = new QVBoxLayout;
    QVBoxLayout *vlyout2 = new QVBoxLayout;
    QHBoxLayout *hlyout1 = new QHBoxLayout;
    QHBoxLayout *hlyout2 = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout1->addWidget(pb,0);
    hlyout1->addStretch(100);
    vlyout1->addLayout(hlyout1);

    hlyout1 = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setObjectName("photo");
    lbl->setMinimumSize(150, 200);
    lbl->setScaledContents(true);
    lbl->setPixmap(QPixmap(":/res/Einstein.png"));
    hlyout1->addWidget(lbl, 50);
    lbl = new s_tqLabel;
    lbl->setText("ФИО: ");
    hlyout2->addWidget(lbl, 10);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("le.1");
    le->setEnabled(isEnabled);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    hlyout2 = new QHBoxLayout;
    lbl = new s_tqLabel("Дата рождения: ");
    hlyout2->addWidget(lbl, 10);
    le = new s_tqLineEdit;
    le->setObjectName("le.2");
    le->setEnabled(isEnabled);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    hlyout2 = new QHBoxLayout;
    lbl = new s_tqLabel("Должность: ");
    hlyout2->addWidget(lbl, 10);
    le = new s_tqLineEdit;
    le->setObjectName("le.3");
    le->setEnabled(isEnabled);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    if (DialogType == PDT_TB)
    {
        hlyout2 = new QHBoxLayout;
        lbl = new s_tqLabel("Группа по ЭБ: ");
        hlyout2->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.31");
        le->setEnabled(isEnabled);
        hlyout2->addWidget(le, 30);
        vlyout2->addLayout(hlyout2);
    }
    hlyout1->addLayout(vlyout2);
    vlyout1->addLayout(hlyout1);

    switch(DialogType)
    {
    case PDT_TB:
    {
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата сдачи экзамена по ЭБ: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.32");
        le->setEnabled(isEnabled);
        hlyout1->addWidget(le, 30);
        lbl = new s_tqLabel("Оценка: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.33");
        le->setEnabled(isEnabled);
        hlyout1->addWidget(le, 30);
        pb = new s_tqPushButton("Протокол");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowEBProt()));
        hlyout1->addWidget(pb, 3);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        pb = new s_tqPushButton("Ввод данных по ЭБ");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(EnterEBData()));
        hlyout1->addWidget(pb, 30);
        pb = new s_tqPushButton("История сдачи экзамена");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowEBHistory()));
        hlyout1->addWidget(pb, 30);
        hlyout1->addStretch(100);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата инструктажа по ПБ: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.41");
        le->setEnabled(isEnabled);
        hlyout1->addWidget(le, 30);
        lbl = new s_tqLabel;
        lbl->setObjectName("PBGoodlbl");
        lbl->setPixmap(QPixmap(":/res/cross.png"));
        hlyout1->addWidget(lbl, 5);
        pb = new s_tqPushButton("Ввод данных");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(EnterPBData()));
        hlyout1->addWidget(pb, 10);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата инструктажа по ОТ: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.42");
        le->setEnabled(isEnabled);
        hlyout1->addWidget(le, 30);
        lbl = new s_tqLabel;
        lbl->setObjectName("OTGoodlbl");
        lbl->setPixmap(QPixmap(":/res/cross.png"));
        hlyout1->addWidget(lbl, 5);
        pb = new s_tqPushButton("Ввод данных");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(EnterOTData()));
        hlyout1->addWidget(pb, 10);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата прохождения профосмотра: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.43");
        le->setEnabled(isEnabled);
        hlyout1->addWidget(le, 30);
        lbl = new s_tqLabel;
        lbl->setObjectName("MedGoodlbl");
        lbl->setPixmap(QPixmap(":/res/cross.png"));
        hlyout1->addWidget(lbl, 5);
        pb = new s_tqPushButton("Ввод данных");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(EnterMedData()));
        hlyout1->addWidget(pb, 10);
        pb = new s_tqPushButton("Протокол");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowMedProt()));
        hlyout1->addWidget(pb, 3);
        vlyout1->addLayout(hlyout1);
        break;
    }
    case PDT_PERS:
    {
        break;
    }
    default:
        break;
    }
    setLayout(vlyout1);
}

int PersDialog::Fill(QString PersID)
{
    QString FIO;
    QStringList fl = QStringList() << "Полные ФИО" << "Дата рождения" << "Должность";
    QStringList vl;
    tfl.valuesbyfield("Персонал_полн", fl, "ИД", PersID, vl);
    if ((tfl.result != TFRESULT_NOERROR) || (vl.size() < 3))
    {
        TFWARN;
        return;
    }
    int i;
    for (i=0; i<vl.size(); ++i)
    {
        if (!SetLEData(this, "le."+QString::number(i), vl.at(i)))
            WARNMSG("");
    }
    fl = QStringList() <<
    return 0;
}

void PersDialog::Filter()
{

}

void PersDialog::Unfilter()
{

}

void PersDialog::ShowEBHistory()
{

}

void PersDialog::ShowEBProt()
{

}

void PersDialog::ShowMedProt()
{

}

void PersDialog::EnterEBData()
{

}

void PersDialog::EnterMedData()
{

}

void PersDialog::EnterOTData()
{

}

void PersDialog::EnterPBData()
{

}
