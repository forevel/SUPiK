#include "persdialog.h"
#include <QAction>
#include <QIcon>
#include <QSizePolicy>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/wd_func.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"
#include "../../gen/client.h"
#include "../tb/tb_func.h"

PersDialog::PersDialog(QString PersID, int DialogType, QWidget *parent) : QDialog(parent)
{
    this->DialogType = DialogType;
    setAttribute(Qt::WA_DeleteOnClose);
    idPers = PersID;
    SetupUI();
    if (Fill())
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

void PersDialog::SetupUI()
{
    QVBoxLayout *vlyout1 = new QVBoxLayout;
    QVBoxLayout *vlyout2 = new QVBoxLayout;
    QHBoxLayout *hlyout1 = new QHBoxLayout;
    QHBoxLayout *hlyout2 = new QHBoxLayout;
    s_tqChooseWidget *cw;

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
    lbl->setMinimumSize(50, 75);
    lbl->setMaximumSize(150, 200);
    lbl->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    vlyout2->addWidget(lbl);
    pb = new s_tqPushButton("Загрузить другое фото");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(LoadPhoto()));
    vlyout2->addWidget(pb);
    hlyout1->addLayout(vlyout2, 50);
    vlyout2 = new QVBoxLayout;
    lbl = new s_tqLabel;
    lbl->setText("ФИО: ");
    hlyout2->addWidget(lbl, 10);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("le.1");
    le->setEnabled(true);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    hlyout2 = new QHBoxLayout;
    lbl = new s_tqLabel("Дата рождения: ");
    hlyout2->addWidget(lbl, 10);
    le = new s_tqLineEdit;
    le->setObjectName("le.2");
    le->setEnabled(true);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    hlyout2 = new QHBoxLayout;
    lbl = new s_tqLabel("Должность: ");
    hlyout2->addWidget(lbl, 10);
    le = new s_tqLineEdit;
    le->setObjectName("le.3");
    le->setEnabled(true);
    hlyout2->addWidget(le, 30);
    vlyout2->addLayout(hlyout2);
    if (DialogType == PDT_TB)
    {
        hlyout2 = new QHBoxLayout;
        lbl = new s_tqLabel("Группа по ЭБ: ");
        hlyout2->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.41");
        le->setEnabled(true);
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
        le->setEnabled(true);
        hlyout1->addWidget(le, 30);
        lbl = new s_tqLabel("Оценка: ");
        hlyout1->addWidget(lbl, 10);
        le = new s_tqLineEdit;
        le->setObjectName("le.33");
        le->setEnabled(true);
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
        cw = new s_tqChooseWidget(true);
        cw->setObjectName("cw.0");
        cw->setEnabled(true);
        cw->Setup("2.18.."); // Date
        hlyout1->addWidget(cw, 30);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата инструктажа по ОТ: ");
        hlyout1->addWidget(lbl, 10);
        cw = new s_tqChooseWidget(true);
        cw->setObjectName("cw.1");
        cw->setEnabled(true);
        cw->Setup("2.18.."); // Date
        hlyout1->addWidget(cw, 30);
        vlyout1->addLayout(hlyout1);
        hlyout1 = new QHBoxLayout;
        lbl = new s_tqLabel("Дата прохождения профосмотра: ");
        hlyout1->addWidget(lbl, 10);
        cw = new s_tqChooseWidget(true);
        cw->setObjectName("cw.2");
        cw->setEnabled(true);
        cw->Setup("2.18.."); // Date
        hlyout1->addWidget(cw, 30);
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
    hlyout1 = new QHBoxLayout;
    hlyout1->addStretch(100);
    pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(Accept()));
    hlyout1->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(close()));
    hlyout1->addWidget(pb);
    hlyout1->addStretch(100);
    vlyout1->addLayout(hlyout1);
    setLayout(vlyout1);
}

bool PersDialog::Fill()
{
    QStringList fl = QStringList() << "Полные ФИО" << "Дата рождения" << "Должность";
    QStringList sl, vl;
    tfl.valuesbyfield("Персонал_полн", fl, "ИД", idPers, vl);
    if ((tfl.result != TFRESULT_NOERROR) || (vl.size() < 3))
    {
        WARNMSG("");
        return false;
    }
    int i;
    for (i=0; i<vl.size(); ++i)
    {
        if (!WDFunc::SetLEData(this, "le."+QString::number(i+1), vl.at(i)))
            WARNMSG("");
    }
    Pers = vl.at(0); // полные ФИО сотрудника
    SetPhoto();
    switch(DialogType)
    {
    case PDT_TB:
    {
        QString table = "Персонал_ТБ_полн";
        QStringList header("Группа");
        QStringList TBGroups, POs, PBs, OTs;
        tfl.valuesbyfield(table, header, "ИД сотрудника", idPers, TBGroups);
        if (tfl.result != TFRESULT_NOERROR)
            return false;
        header = QStringList() << "Дата профосмотра";
        tfl.valuesbyfield(table, header, "ИД сотрудника", idPers, POs);
        if (tfl.result != TFRESULT_NOERROR)
            return false;
        header = QStringList() << "Дата ПБ";
        tfl.valuesbyfield(table, header, "ИД сотрудника", idPers, PBs);
        if (tfl.result != TFRESULT_NOERROR)
            return false;
        header = QStringList() << "Дата ОТ";
        tfl.valuesbyfield(table, header, "ИД сотрудника", idPers, OTs);
        if (tfl.result != TFRESULT_NOERROR)
            return false;
        QString TBGroup = (TBGroups.isEmpty()) ? TB_NODATA : TBGroups.at(0);
        TBGroup = (TBGroup.isEmpty()) ? TB_NODATA : TBGroup;
        QString PO = (POs.isEmpty()) ? TB_NODATA : POs.at(0);
        PO = (PO.isEmpty()) ? TB_NODATA : PO;
        QString PB = (PBs.isEmpty()) ? TB_NODATA : PBs.at(0);
        PB = (PB.isEmpty()) ? TB_NODATA : PB;
        QString OT = (OTs.isEmpty()) ? TB_NODATA : OTs.at(0);
        OT = (OT.isEmpty()) ? TB_NODATA : OT;
        if (!WDFunc::SetLEData(this, "le.41", TBGroup))
            WARNMSG("");
        vl = QStringList() << PO << PB << OT;
        for (i=0; i<vl.size(); ++i)
        {
            if (!WDFunc::SetCWData(this, "cw."+QString::number(i), vl.at(i)))
                WARNMSG("");
        }
        vl.clear();
        fl = QStringList() << "idpers" << "section";
        if (TBGroup.at(0) == TB_NODATA)
            vl << idPers << "2"; // 2 - 2-я группа по ЭБ (без экзаменов)
        else
            vl << idPers << TBGroup.at(0);
        sl = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "date", fl, vl, "date", false); // Dates - дата последнего экзамена
        if ((sl.isEmpty()) || (sqlc.result != SQLC_OK))
        {
            EBDate = "N/A";
            WDFunc::SetLEColor(this, "le.32", CList.at(TBDATE_BAD));
        }
        else
        {
            EBDate = sl.at(0);
            WDFunc::SetLEColor(this, "le.32", CList.at(TBFunc::CheckDate(TBFunc::DT_TB, sl.at(0))));
        }
        WDFunc::SetLEData(this, "le.32", EBDate);
        sl = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "examresults", fl, vl, "date", false); // Dates - дата последнего экзамена
        if ((sl.isEmpty()) || (sqlc.result != SQLC_OK))
            WDFunc::SetLEData(this, "le.33", "N/A");
        else
            WDFunc::SetLEData(this, "le.33", sl.at(0));
        MedDate = WDFunc::CWData(this, "cw.0");
        PBDate = WDFunc::CWData(this, "cw.1");
        OTDate = WDFunc::CWData(this, "cw.2");
    }
    case PDT_PERS:
    {
        break;
    }
    default:
        break;
    }
    return true;
}

void PersDialog::ShowEBHistory()
{

}

void PersDialog::ShowEBProt()
{
    // PDF
}

void PersDialog::ShowMedProt()
{
    // PDF
}

void PersDialog::EnterEBData()
{
    QDialog *dlg = new QDialog(this);
    dlg->setObjectName("EBdlg");
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel(Pers);
    hlyout->addWidget(lbl);
    QVBoxLayout *lyout = new QVBoxLayout;
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Дата проведения экзамена: ");
    hlyout->addWidget(lbl);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->setObjectName("EBdate");
    cw->Setup("2.18..");
    cw->SetValue(EBDate);
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Оценка за экзамен (0-5)");
    hlyout->addWidget(lbl);
    s_tqLineEdit *le = new s_tqLineEdit(WDFunc::LEData(this, "le.33"));
    le->setObjectName("EBmark");
    hlyout->addWidget(le);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    hlyout->addStretch(100);
    s_tqPushButton *pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(AcceptEBData()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb,SIGNAL(clicked()),dlg,SLOT(close()));
    hlyout->addWidget(pb);
    hlyout->addStretch(100);
    lyout->addLayout(hlyout);
    dlg->setLayout(lyout);
    dlg->show();
}

void PersDialog::AcceptEBData()
{
    QDialog *dlg = this->findChild<QDialog *>("EBdlg");
    if (dlg == 0)
    {
        WARNMSG("EBdlg not found");
        return;
    }
    QString tmps = WDFunc::CWData(dlg, "EBdate");
    WDFunc::SetLEData(this, "le.32", tmps);
    WDFunc::SetLEColor(this, "le.32", CList.at(TBFunc::CheckDate(TBFunc::DT_TB, tmps)));
    WDFunc::SetLEData(this, "le.33", WDFunc::LEData(dlg, "EBmark"));
    dlg->close();
}

void PersDialog::Accept()
{

}

void PersDialog::LoadPhoto()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Загрузка фотографии", pc.HomeDir+"pers/photo", "Images (*.png *.jpg *.gif)");
    QString DestFileName = pc.HomeDir+"pers/photo/"+idPers;
    if (FileName.isEmpty())
        return;
    QFile fp(FileName);
    if (!fp.copy(DestFileName))
    {
        WARNMSG("Файл уже существует");
        QFile::remove(DestFileName);
        if (!fp.copy(DestFileName))
        {
            ERMSG("Невозможно переименовать файл");
            return;
        }
    }
    if (Cli->PutFile(DestFileName, FLT_PERS, FLST_PHOTO, idPers) != Client::CLIER_NOERROR)
    {
        WARNMSG("");
        return;
    }
    SetPhoto();
}

void PersDialog::SetPhoto()
{
    QFile fp;
    QPixmap pm;
    fp.setFileName(pc.HomeDir+"pers/photo/"+idPers);
    if (!fp.open(QIODevice::ReadOnly))
    {
        if (Cli->GetFile(FLT_PERS, FLST_PHOTO, idPers) == Client::CLIER_NOERROR)
        {
            if (fp.open(QIODevice::ReadOnly))
            {
                pm.load(fp.fileName());
                fp.close();
            }
        }
        WARNMSG("Нет фотографии для сотрудника с ИД " + idPers);
        pm.load(":/res/Einstein.png");
    }
    else
    {
        pm.load(fp.fileName());
        fp.close();
    }
    if (!WDFunc::SetLBLImage(this, "photo", &pm))
        WARNMSG("");
}
