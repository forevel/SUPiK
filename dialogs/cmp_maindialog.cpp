#include "cmp_maindialog.h"
#include <QTabWidget>
#include <QPainter>
#include <QStringListModel>
//#include "../widgets/s_tqtreeview.h"
//#include "../widgets/s_tqtableview.h"
//#include "../widgets/s_tqframe.h"
//#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqwidget.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqgroupbox.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqchoosewidget.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqcheckbox.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"

#include <QVBoxLayout>
#include <QFont>

cmp_maindialog::cmp_maindialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Редактор компонентов");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignLeft);
    hlyout->addStretch(5);
    lbl=new s_tqLabel("Раздел:");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setEnabled(false);
    le->setObjectName("section");
    hlyout->addWidget(le,5);
    lbl=new s_tqLabel("Подраздел:");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    le = new s_tqLineEdit;
    le->setEnabled(false);
    le->setObjectName("subsection");
    hlyout->addWidget(le,10);
    lbl=new s_tqLabel("ИД:");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    le = new s_tqLineEdit;
    le->setEnabled(false);
    le->setObjectName("id");
    hlyout->addWidget(le,4);
    lyout->addLayout(hlyout);
    QTabWidget *ctw = new QTabWidget;
    ctw->setStyleSheet("QTabWidget::pane {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px;}"\
                       "QTabWidget::tab {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px;}");
    s_tqWidget *cp1 = new s_tqWidget;
    cp1->setObjectName("cp1");
    s_tqWidget *cp2 = new s_tqWidget;
    cp2->setObjectName("cp2");
    ctw->addTab(cp1, "Основные");
    ctw->addTab(cp2, "Дополнительные");
    lyout->addWidget(ctw);
    s_tqPushButton *pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(WriteAndClose()));
    hlyout = new QHBoxLayout;
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(close()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);
    setLayout(lyout);
}

cmp_maindialog::~cmp_maindialog()
{

}

void cmp_maindialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/WPAltium.png"));
    e->accept();
}

// подготовка диалога к отображению. 1 = тип компонента (Altium,Schemagee,...), 2 = номер таблицы компонента по таблице description, 3 = ИД компонента по таблице

void cmp_maindialog::SetupUI(int CompType, int CompTable, int CompID)
{
    QStringList sl = QStringList() << "" << "А" << "З" << "Э" << "К" << "У";
    QStringList dbsl = QStringList() << "" << "alt" << "sch" << "sol" << "con" << "dev";
    QStringList sectsl = QStringList() << "" << "Altium" << "Schemagee" << "Solidworks" << "Конструктивы,материалы" << "Устройства";
    this->CompDb = dbsl.at(CompType);
    QStringList fl = QStringList() << "Наименование" << "Описание";
    QStringList tblesl = tfl.valuesbyfield(sl.at(CompType)+"Компоненты_описание_сокращ",fl,"ИД",QString::number(CompTable));
    if (tfl.result)
    {
        emit error(ER_CMPMAIN+tfl.result,0x01);
        return;
    }
    this->CompTble = tblesl.at(0);
    this->CompId = QString::number(CompID);
    this->CompType = CompType;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("section");
    if (le == 0)
    {
        emit error(ER_CMPMAIN,0x02);
        return;
    }
    le->setText(sectsl.at(CompType));
    le = this->findChild<s_tqLineEdit *>("subsection");
    if (le == 0)
    {
        emit error(ER_CMPMAIN,0x03);
        return;
    }
    le->setText(tblesl.at(1));
    le = this->findChild<s_tqLineEdit *>("id");
    if (le == 0)
    {
        emit error(ER_CMPMAIN,0x04);
        return;
    }
    le->setText(CompId);
    fl.clear();
    switch (CompType)
    {
    case CTYPE_ALT:
    {
        SetAltDialog();
        fl << "Library Ref" << "Library Path" << "Footprint Ref" << "Footprint Path" << "Sim Description" << "Sim File" << \
              "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
              "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
              "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
              "Nominal" << "Unit" << "par4" << "par5";
        QStringList vl = sqlc.getvaluesfromtablebyfield(sqlc.getdb(CompDb),CompTble,fl,"id",CompId);
        if (sqlc.result); // новый элемент, ещё нет в БД
        else
            FillAltDialog(vl);
        break;
    }
    case CTYPE_SCH:
    {
        break;
    }
    case CTYPE_SOL:
    {
        break;
    }
    case CTYPE_CON:
    {
        break;
    }
    case CTYPE_DEV:
    {
        break;
    }
    default:
        break;
    }
}

void cmp_maindialog::SetAltDialog()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqWidget *cp = this->findChild<s_tqWidget *>("cp1");
    if (cp == 0)
    {
        emit error(ER_CMPMAIN,0x11);
        return;
    }
    s_tqWidget *cp2 = this->findChild<s_tqWidget *>("cp2");
    if (cp2 == 0)
    {
        emit error(ER_CMPMAIN,0x12);
        return;
    }
    s_tqGroupBox *gb = new s_tqGroupBox;
    gb->setTitle("Компонент");
    QGridLayout *glyout = new QGridLayout;
    s_tqLabel *lbl = new s_tqLabel("Наименование");
    glyout->addWidget(lbl,0,0,1,1);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("partnumber");
    glyout->addWidget(le,0,1,1,2);
    lbl = new s_tqLabel("Производитель");
    glyout->addWidget(lbl,1,0,1,1);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup("2.2..Производители_сокращ.Наименование");
    cw->setObjectName("manufacturer");
    glyout->addWidget(cw,1,1,1,1);
    s_tqPushButton *pb = new s_tqPushButton(QString("Добавить"));
    connect(pb,SIGNAL(clicked()),this,SLOT(AddManuf()));
    glyout->addWidget(pb,1,2,1,1);
    glyout->setColumnStretch(0, 20);
    glyout->setColumnStretch(1, 80);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Библиотеки");
    lbl = new s_tqLabel("УГО (символ)");
    glyout = new QGridLayout;
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    connect(cw,SIGNAL(error(int,int)),this,SLOT(emiterror(int,int)));
    cw->setObjectName("libref");
    int i = 0;
    QStringList tmpsl = CompTble.split("_", QString::KeepEmptyParts);
    QString PathString = "";
    QString tmps = tmpsl.last();
    while ((tmpsl.value(i, "") != "") && (tmpsl.value(i, "") != tmps))
    {
        PathString += "/";
        PathString += tmpsl.value(i++);
    }
    tmps = pc.PathToLibs + "Symbols" + PathString + "/" + CompTble + ".SchLib";
    cw->Setup("2.17.."+tmps+"."+pc.symfind);
    glyout->addWidget(cw,0,1,1,1);
    lbl = new s_tqLabel("Посадочное место");
    glyout->addWidget(lbl,1,0,1,1);
    cw = new s_tqChooseWidget(true);
    connect(cw,SIGNAL(error(int,int)),this,SLOT(emiterror(int,int)));
    cw->setObjectName("footref");
    tmps = pc.PathToLibs + "Footprints" + PathString + "/" + CompTble + ".PcbLib";
    cw->Setup("2.17.."+tmps+"."+pc.footfind);
    glyout->addWidget(cw,1,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb=new s_tqGroupBox;
    glyout = new QGridLayout;
    gb->setTitle("Поля ПЭ3");
    lbl = new s_tqLabel("\"Наименование\"");
    glyout->addWidget(lbl,0,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("pe3name");
    glyout->addWidget(le,0,1,1,1);
    lbl = new s_tqLabel("\"Примечание\"");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("pe3notes");
    glyout->addWidget(le,1,1,1,1);
    glyout->setColumnStretch(0,30);
    glyout->setColumnStretch(1,70);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    s_tqCheckBox *chb;
    s_tqComboBox *cb;
    gb=new s_tqGroupBox;
    glyout = new QGridLayout;
    gb->setTitle("Характеристики");
    for (int i=0; i<5; i++)
    {
        lbl = new s_tqLabel("");
        lbl->setObjectName("par"+QString::number(i)+"lbl");
        glyout->addWidget(lbl,i,0,1,1);
        le = new s_tqLineEdit;
        le->setObjectName("par"+QString::number(i)+"le");
        glyout->addWidget(le,i,1,1,1);
        cb = new s_tqComboBox;
        cb->setObjectName("par"+QString::number(i)+"cb");
        glyout->addWidget(cb,i,2,1,1);
        chb = new s_tqCheckBox;
        chb->setText("Учёт в наименовании");
        chb->setObjectName("par"+QString::number(i)+"chb");
        glyout->addWidget(chb,i,3,1,1);
    }
    lbl = new s_tqLabel("Мин. раб. температура");
    glyout->addWidget(lbl,6,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mintemple");
    glyout->addWidget(le,6,1,1,3);
    lbl = new s_tqLabel("Макс. раб. температура");
    glyout->addWidget(lbl,7,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("maxtemple");
    glyout->addWidget(le,7,1,1,3);
    lbl = new s_tqLabel("Корпус компонента");
    glyout->addWidget(lbl,8,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("packagele");
    glyout->addWidget(le,8,1,1,3);
    chb = new s_tqCheckBox;
    chb->setText("Компонент планарный (SMD)");
    chb->setObjectName("issmdchb");
    glyout->addWidget(chb,9,1,1,3);

    gb->setLayout(glyout);
    lyout->addWidget(gb);
    cp->setLayout(lyout);

    lyout = new QVBoxLayout;
    gb = new s_tqGroupBox;
    gb->setTitle("Дополнительные параметры");
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Точность");
    glyout->addWidget(lbl,0,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("accuracyle");
    glyout->addWidget(le,0,1,1,3);
    lbl = new s_tqLabel("Макс. рассеив. мощность");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("maxpowerle");
    glyout->addWidget(le,1,1,1,3);
    lbl = new s_tqLabel("ТКС (ТКЕ)");
    glyout->addWidget(lbl,2,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("tkcle");
    glyout->addWidget(le,2,1,1,3);
    lbl = new s_tqLabel("Маркировка корпуса");
    glyout->addWidget(lbl,3,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("markingle");
    glyout->addWidget(le,3,1,1,3);
    chb = new s_tqCheckBox;
    chb->setText("Компонент выпускается");
    chb->setObjectName("isactivechb");
    glyout->addWidget(chb,4,1,1,3);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Описание компонента");
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Файл описания компонента");
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    cw->Setup("2.15..PDF Documents (*_pdf)");
    cw->setObjectName("dsheetcw");
    glyout->addWidget(cw,0,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Модель");
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Файл описания модели");
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    cw->Setup("2.15..All files (*_*)");
    cw->setObjectName("mdlfilecw");
    glyout->addWidget(cw,0,1,1,1);
    lbl = new s_tqLabel("Описание модели");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdldescle");
    glyout->addWidget(le,1,1,1,1);
    lbl = new s_tqLabel("Имя модели");
    glyout->addWidget(lbl,2,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdlnamele");
    glyout->addWidget(le,2,1,1,1);
    lbl = new s_tqLabel("Параметры модели");
    glyout->addWidget(lbl,3,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdlparsle");
    glyout->addWidget(le,3,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);
        //    connect (ManufCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (PartNumberLE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (Par1LE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (isNeedToAccAccuracyInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));
        //    connect (isNeedToAccVoltageInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));

    cp2->setLayout(lyout);
}

void cmp_maindialog::FillAltDialog(QStringList vl)
{

}

void cmp_maindialog::AddManuf()
{
    //    qa_AddManufDialog.AddNewItem();
    //    qa_AddManufDialog.exec();
    /*    QStringList tmpList;
        tmpList.clear();
        QSqlQuery mw_ent_get_manuf (pc.ent);
    // 0.4b    mw_ent_get_manuf.exec("SELECT manuf FROM manuf;");
        mw_ent_get_manuf.exec("SELECT manuf FROM manuf WHERE `deleted`=0;");
        while (mw_ent_get_manuf.next())
        {
            tmpList << mw_ent_get_manuf.value(0).toString();
        }
        CompManufModel->setStringList(tmpList);
        CompManufModel->sort(0, Qt::AscendingOrder); // 0.4b
        ManufCB->setModel(CompManufModel);
        ManufCB->setCurrentText(pc.InterchangeString);*/
}

void cmp_maindialog::WriteAndClose()
{

}

void cmp_maindialog::emiterror(int er1, int er2)
{
    er1 += ER_CMPMAIN;
    emit error(er1,er2);
}
