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
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/wd_func.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../gen/messagebox.h"
#include "../../gen/client.h"
#include "../tb/tb_func.h"

PersDialog::PersDialog(QString PersID, int DialogType, int Mode, QWidget *parent) : QDialog(parent)
{
    this->DialogType = DialogType;
    this->Mode = Mode;
    setAttribute(Qt::WA_DeleteOnClose);
    idPers = PersID;
    idRec = "0";
    idRecEkz = "0";
    EBDataChanged = false;
    EkzType = EKZ_GENERAL;
    SetupUI();
    if (!Fill())
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
    connect(pb, SIGNAL(clicked()), this, SLOT(CloseDialog()));
    pb->setToolTip("Закрыть вкладку");
    hlyout1->addWidget(pb,0);
    hlyout1->addStretch(100);
    vlyout1->addLayout(hlyout1);

    hlyout1 = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setObjectName("photo");
    lbl->setMinimumSize(50, 75);
    lbl->setMaximumSize(150, 200);
    lbl->setScaledContents(true);
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
    connect(pb,SIGNAL(clicked()),this,SLOT(CloseDialog()));
    hlyout1->addWidget(pb);
    hlyout1->addStretch(100);
    vlyout1->addLayout(hlyout1);
    setLayout(vlyout1);
}

bool PersDialog::Fill()
{
    QStringList fl = QStringList() << "Полные ФИО" << "Дата рождения" << "Должность";
    QStringList vl;
    tfl.valuesbyfield("Персонал_полн", fl, "ИД", idPers, vl);
    if ((tfl.result != TFRESULT_NOERROR) || (vl.size() < 3))
    {
        WARNMSG("");
        return false;
    }
    int i;
    for (i=0; i<vl.size(); ++i)
        WDFunc::SetLEData(this, "le."+QString::number(i+1), vl.at(i));
    Pers = vl.at(0); // полные ФИО сотрудника
    SetPhoto();
    if (Mode == MODE_EDIT)
    {
        switch(DialogType)
        {
        case PDT_TB:
        {
            QString table = "Персонал_ТБ_полн";
            QStringList headers = QStringList() << "Группа" << "Дата профосмотра" << "Дата ПБ" << "Дата ОТ" << "ИД";
            QStringList sl;
            QString TBGroup, PO, PB, OT;
            tfl.valuesbyfield(table, headers, "ИД сотрудника", idPers, sl);
            if ((tfl.result != TFRESULT_NOERROR) || (sl.size() < 5))
            {
                WARNMSG("Bad tfl result");
                return false;
            }
            TBGroup = sl.at(0);
            PO = sl.at(1);
            PB = sl.at(2);
            OT = sl.at(3);
            idRec = sl.at(4);
            TBGroup = (TBGroup.isEmpty()) ? TB_NODATA : TBGroup;
            PO = (PO.isEmpty()) ? TB_NODATA : PO;
            PB = (PB.isEmpty()) ? TB_NODATA : PB;
            OT = (OT.isEmpty()) ? TB_NODATA : OT;
            WDFunc::SetLEData(this, "le.41", TBGroup);
            vl = QStringList() << PB << OT << PO;
            for (i=0; i<vl.size(); ++i)
                WDFunc::SetCWData(this, "cw."+QString::number(i), vl.at(i));
            vl.clear();
            fl = QStringList() << "idpers" << "section";
            if (TBGroup.at(0) == TB_NODATA)
                vl << idPers << "2"; // 2 - 2-я группа по ЭБ (без экзаменов)
            else
                vl << idPers << TBGroup.at(0);
            sl = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "date", fl, vl, "date", false); // Date - дата последнего экзамена
            if ((sl.isEmpty()) || (sqlc.result != SQLC_OK))
            {
                EBDate = "N/A";
                WDFunc::SetLEColor(this, "le.32", CList.at(TBDATE_BAD));
            }
            else
            {
                EBDate = sl.at(0);
                WDFunc::SetLEColor(this, "le.32", CList.at(TBFunc_CheckDateTime(DT_TB, sl.at(0))));
            }
            WDFunc::SetLEData(this, "le.32", EBDate);
            sl = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "examresults", fl, vl, "date", false);
            if ((sl.isEmpty()) || (sqlc.result != SQLC_OK))
                WDFunc::SetLEData(this, "le.33", "N/A");
            else
                WDFunc::SetLEData(this, "le.33", sl.at(0));
            sl = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "idexamresults", fl, vl, "date", false);
            if ((sl.isEmpty()) || (sqlc.result != SQLC_OK))
                idRecEkz = "0";
            else
                idRecEkz = sl.at(0);
        }
        case PDT_PERS:
        {
            break;
        }
        default:
            break;
        }
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
    dlg->setAttribute(Qt::WA_DeleteOnClose);
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
    cw->Setup("2.21.."); // FW_DATETIME
    cw->SetValue(EBDate);
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Оценка за экзамен (0-5)");
    hlyout->addWidget(lbl);
    QString tmps;
    WDFunc::LEData(this, "le.33", tmps);
    s_tqLineEdit *le = new s_tqLineEdit(tmps);
    le->setObjectName("EBmark");
    hlyout->addWidget(le);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->addItem("Очередной");
    cb->addItem("Внеочередной");
    connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(SetEkzType(QString)));
    cb->setCurrentIndex(0);
    lbl = new s_tqLabel("Тип экзамена:");
    hlyout->addWidget(lbl);
    hlyout->addWidget(cb);
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

void PersDialog::SetEkzType(const QString &ekztype)
{
    this->EkzType = (ekztype == "Очередной") ? EKZ_GENERAL : EKZ_NGENERAL;
}

void PersDialog::AcceptEBData()
{
    EBDataChanged = true;
    QDialog *dlg = this->findChild<QDialog *>("EBdlg");
    if (dlg == 0)
    {
        WARNMSG("EBdlg not found");
        return;
    }
    QString tmps = WDFunc::CWData(this, "EBdate");
    WDFunc::SetLEData(this, "le.32", tmps);
    EBDate = tmps;
    WDFunc::SetLEColor(this, "le.32", CList.at(TBFunc_CheckDateTime(DT_TB, tmps)));
    WDFunc::LEData(this, "EBmark", tmps);
    WDFunc::SetLEData(this, "le.33", tmps);
    dlg->close();
}

void PersDialog::Accept()
{
    QString ID, tmps;
    // запишем сначала общие поля - полные ФИО, Дата рождения, Должность
    QStringList fl = QStringList() << "Полные ФИО" << "Дата рождения" << "Должность";
    QStringList vl;
    for (int i=0; i<fl.size(); ++i)
    {
        WDFunc::LEData(this, "le."+QString::number(i+1), tmps);
        vl.append(tmps);
    }
    fl << "ИД";
    vl << idPers;
    tfl.Update("Персонал_полн", fl, vl);
    if (tfl.result != TFRESULT_NOERROR)
        WARNMSG("");
    // теперь надо записать конкретные параметры, зависящие от типа диалога редактирования сотрудника
    switch (DialogType)
    {
    case PDT_TB:
    {
        if (Mode == MODE_EDITNEW)
        {
            tfl.Insert("Персонал_ТБ_полн", ID);
            if (tfl.result != TFRESULT_NOERROR)
            {
                WARNMSG("Bad TFL result");
                return;
            }
        }
        else
            ID = idRec;
        MedDate = WDFunc::CWData(this, "cw.2");
        PBDate = WDFunc::CWData(this, "cw.0");
        OTDate = WDFunc::CWData(this, "cw.1");
        QString table = "Персонал_ТБ_полн";
        QStringList headers = QStringList() << "Группа" << "Дата профосмотра" << "Дата ПБ" << "Дата ОТ" << "ИД" << "ИД сотрудника";
        WDFunc::LEData(this, "le.41", tmps);
        QStringList values = QStringList() << tmps << MedDate << PBDate << OTDate << ID << idPers;
        tfl.Update(table, headers, values);
        if (tfl.result != TFRESULT_NOERROR)
        {
            WARNMSG("Bad TFL result");
            return;
        }
        if (EBDataChanged)
        {
            if (idRecEkz == "0") // новая запись должна быть сделана
            {
                tfl.Insert("Экзам рез_полн", idRecEkz);
                if (tfl.result != TFRESULT_NOERROR)
                {
                    WARNMSG("Bad TFL result");
                    return;
                }
            }
            QString EkzTypeToWrite = (EkzType == EKZ_GENERAL) ? "1" : "0";
            headers = QStringList() << "Раздел" << "Результат" << "Тип" << "ИД" << "Дата" << "ИДПольз";
            WDFunc::LEData(this, "le.41", tmps);
            values = QStringList() << tmps;
            WDFunc::LEData(this, "le.33", tmps);
            values << tmps << EkzTypeToWrite << idRecEkz;
            WDFunc::LEData(this, "le.32", tmps);
            values << tmps << idPers;
            tfl.Update("Экзам рез_полн", headers, values);
            if (tfl.result != TFRESULT_NOERROR)
            {
                WARNMSG("Bad TFL result");
                return;
            }
        }
        break;
    }
    case PDT_PERS:
    {
        break;
    }
    default:
        return;
    }
    this->CloseDialog();
}

void PersDialog::LoadPhoto()
{
    QString FileName = QFileDialog::getOpenFileName(this, "Загрузка фотографии", pc.HomeDir+"/pers/photo", "Images (*.png *.jpg *.gif)");
    QString DestFileName = pc.HomeDir+"/pers/photo/"+idPers;
    if (FileName.isEmpty())
    {
        WARNMSG("Пустое имя файла");
        return;
    }
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
        WARNMSG("Client Putfile bad result");
        return;
    }
    SetPhoto();
}

void PersDialog::SetPhoto()
{
    QFile fp;
    QPixmap pm;
    fp.setFileName(pc.HomeDir+"/pers/photo/"+idPers);
    if (Cli->GetFile(FLT_PERS, FLST_PHOTO, idPers) == Client::CLIER_NOERROR)
    {
        if (fp.open(QIODevice::ReadOnly))
        {
            pm.load(fp.fileName());
            fp.close();
        }
    }
    else
    {
        WARNMSG("Нет фотографии для сотрудника с ИД " + idPers);
        pm.load(":/res/Einstein.png");
    }
    WDFunc::SetLBLImage(this, "photo", &pm);
}

void PersDialog::CloseDialog()
{
    emit DialogClosed();
    close();
}
