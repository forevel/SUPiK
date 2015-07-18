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
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"

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
    QStringList dbsl = QStringList() << "" << "alt" << "sch" << "sol" << "con" << "dev";
    this->CompDb = dbsl.at(CompType);
    QString tble = sqlc.getvaluefromtablebyfield(sqlc.getdb(CompDb),"description","description","iddescription",QString::number(CompTable)); // tble ~ capasitors
    if (sqlc.result)
    {
        emit error(ER_CMPMAIN+sqlc.result,0x01);
        return;
    }
    this->CompTble = tble;
    this->CompId = QString::number(CompID);
    QStringList fl;
    switch (CompType)
    {
    case CTYPE_ALT:
    {
        fl << "Library Ref" << "Library Path" << "Footprint Ref" << "Footprint Path" << "Sim Description" << "Sim File" << \
              "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
              "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
              "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
              "Nominal" << "Unit" << "par4" << "par5";
        QStringList vl = sqlc.getvaluesfromtablebyfield(sqlc.getdb(CompDb),CompTble,fl,"id",CompId);
        if (sqlc.result)
        {
            emit error(ER_CMPMAIN+sqlc.result,0x02);
            return;
        }
        SetAltDialog();
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

    gb=new s_tqGroupBox;
    glyout = new QGridLayout;
    gb->setTitle("Характеристики");

    /*        TechGB = new s_tqGroupBox;
        TechGB->setTitle("Характеристики");
        Par1L = new s_tqLabel;
        Par3L = new s_tqLabel;
        Par3L = new s_tqLabel;
        Par4L = new s_tqLabel;
        Par5L = new s_tqLabel;
        UnitsCB = new s_tqComboBox;
        UnitsCB->setEditable(false);
        AccuracyL = new s_tqLabel("Точность");
        MinOpTL = new s_tqLabel("Мин. раб. темп.");
        MaxOpTL = new s_tqLabel("Макс. раб. темп.");
        Par1LE = new s_tqLineEdit;
        Par3LE = new s_tqLineEdit;
        Par3LE = new s_tqLineEdit;
        Par4LE = new s_tqLineEdit;
        Par5LE = new s_tqLineEdit;
        AccuracyLE = new s_tqLineEdit;
        MinOpTLE = new s_tqLineEdit;
        MaxOpTLE = new s_tqLineEdit;
        MaxPowerLE = new s_tqLineEdit;
        MaxPowerL = new s_tqLabel("Макс. рассеив. мощность");
        MaxPowerL->sizePolicy().setHeightForWidth(true);
        TKCL = new s_tqLabel("ТКС (ТКЕ)");
        TKCLE = new s_tqLineEdit;
        isNeedToAccVoltageInNameCB = new s_tqCheckBox;
        isNeedToAccVoltageInNameL = new s_tqLabel("Учёт");
        isNeedToAccAccuracyInNameCB = new s_tqCheckBox;
        isNeedToAccAccuracyInNameL = new s_tqLabel("Учёт");

        TechGBLayout = new QGridLayout;
        TechGBLayout->addWidget(Par1L, 0, 0);

        /*    PackageGBLAyout = new QGridLayout;
        DSheetGBLayout = new QGridLayout;
        PackageGB = new s_tqGroupBox(qt_cmp_addcompdialog);
        PackageGB->setObjectName(QStringLiteral("PackageGB"));
        PackageGB->setGeometry(QRect(380, 320, 231, 81));
        QFont font4;
        font4.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font4.setBold(false);
        font4.setItalic(false);
        font4.setUnderline(false);
        font4.setWeight(50);
        font4.setStrikeOut(false);
        font4.setKerning(false);
        font4.setStyleStrategy(QFont::PreferDefault);
        PackageGB->setFont(font4);
        PackageGB->setStyleSheet(QLatin1String("color: rgb(170, 170, 0);\n"
    "background-color: rgb(200, 255, 196);"));
        PackageGB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        TypeL = new s_tqLabel(PackageGB);
        TypeL->setObjectName(QStringLiteral("TypeL"));
        TypeL->setGeometry(QRect(10, 20, 31, 16));
        TypeL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        MarkL = new s_tqLabel(PackageGB);
        MarkL->setObjectName(QStringLiteral("MarkL"));
        MarkL->setGeometry(QRect(10, 50, 61, 20));
        MarkL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        TypeLE = new s_tqLineEdit(PackageGB);
        TypeLE->setObjectName(QStringLiteral("TypeLE"));
        TypeLE->setGeometry(QRect(40, 20, 111, 20));
        TypeLE->setStyleSheet(QStringLiteral("color: rgb(85, 85, 0);"));
        MarkLE = new s_tqLineEdit(PackageGB);
        MarkLE->setObjectName(QStringLiteral("MarkLE"));
        MarkLE->setGeometry(QRect(110, 50, 111, 20));
        MarkLE->setStyleSheet(QStringLiteral("color: rgb(85, 85, 0);"));
        isSMDL = new s_tqLabel(PackageGB);
        isSMDL->setObjectName(QStringLiteral("isSMDL"));
        isSMDL->setGeometry(QRect(167, 20, 31, 16));
        isSMDL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        isSMDCB = new s_ts_tqCheckBox(PackageGB);
        isSMDCB->setObjectName(QStringLiteral("isSMDCB"));
        isSMDCB->setGeometry(QRect(200, 20, 21, 17));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(isSMDCB->sizePolicy().hasHeightForWidth());
        isSMDCB->setSizePolicy(sizePolicy);
        isSMDCB->setSizeIncrement(QSize(0, 0));
        isSMDCB->setBaseSize(QSize(0, 0));
        isSMDCB->setAutoFillBackground(false);
        isSMDCB->setChecked(false);
        DSheetGB = new s_tqGroupBox(qt_cmp_addcompdialog);
        DSheetGB->setObjectName(QStringLiteral("DSheetGB"));
        DSheetGB->setGeometry(QRect(10, 410, 601, 61));
        DSheetGB->setStyleSheet(QLatin1String("color: rgb(170, 170, 0);\n"
    "background-color: rgb(200, 255, 196);"));
        DSheetGB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        DSheetGB->setFlat(false);
        DSheetPB = new s_tqPushButton(DSheetGB);
        DSheetPB->setObjectName(QStringLiteral("DSheetPB"));
        DSheetPB->setGeometry(QRect(570, 20, 21, 20));
        DSheetL = new s_tqLabel(DSheetGB);
        DSheetL->setObjectName(QStringLiteral("DSheetL"));
        DSheetL->setGeometry(QRect(10, 17, 81, 31));
        DSheetL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        DSheetLE = new s_tqLabel(DSheetGB);
        DSheetLE->setObjectName(QStringLiteral("DSheetLE"));
        DSheetLE->setGeometry(QRect(100, 21, 461, 21));
        DSheetLE->setStyleSheet(QStringLiteral("color: rgb(85, 85, 0);"));
        DSheetLE->setFrameShape(QFrame::StyledPanel);
        AcceptAndClosePB = new s_tqPushButton(qt_cmp_addcompdialog);
        AcceptAndClosePB->setObjectName(QStringLiteral("AcceptAndClosePB"));
        AcceptAndClosePB->setGeometry(QRect(360, 560, 151, 23));
        AcceptAndClosePB->setStyleSheet(QStringLiteral("background-color: rgb(220, 220, 220);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/icon_zap.png"), QSize(), QIcon::Normal, QIcon::Off);
        AcceptAndClosePB->setIcon(icon1);
        AcceptAndClosePB->setAutoDefault(true);
        AcceptAndClosePB->setFlat(false);
        DeclinePB = new s_tqPushButton(qt_cmp_addcompdialog);
        DeclinePB->setObjectName(QStringLiteral("DeclinePB"));
        DeclinePB->setGeometry(QRect(520, 560, 91, 23));
        DeclinePB->setStyleSheet(QLatin1String("border-color: rgb(255, 0, 0);\n"
    "background-color: rgb(220, 220, 220);"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/res/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
        DeclinePB->setIcon(icon2);
        idCompLE = new s_tqLabel(qt_cmp_addcompdialog);
        idCompLE->setObjectName(QStringLiteral("idCompLE"));
        idCompLE->setEnabled(false);
        idCompLE->setGeometry(QRect(550, 12, 61, 21));
        idCompLE->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        idCompLE->setFrameShape(QFrame::WinPanel);
        idCompLE->setFrameShadow(QFrame::Sunken);
        idCompL = new s_tqLabel(qt_cmp_addcompdialog);
        idCompL->setObjectName(QStringLiteral("idCompL"));
        idCompL->setGeometry(QRect(530, 12, 21, 21));
        idCompL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        SectionL = new s_tqLabel(qt_cmp_addcompdialog);
        SectionL->setObjectName(QStringLiteral("SectionL"));
        SectionL->setGeometry(QRect(256, 10, 41, 21));
        SectionL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        SectionLE = new s_tqLabel(qt_cmp_addcompdialog);
        SectionLE->setObjectName(QStringLiteral("SectionLE"));
        SectionLE->setEnabled(false);
        SectionLE->setGeometry(QRect(300, 10, 171, 21));
        SectionLE->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        SectionLE->setFrameShape(QFrame::WinPanel);
        SectionLE->setFrameShadow(QFrame::Sunken);
        DeletePB = new s_tqPushButton(qt_cmp_addcompdialog);
        DeletePB->setObjectName(QStringLiteral("DeletePB"));
        DeletePB->setGeometry(QRect(10, 560, 81, 23));
        DeletePB->setStyleSheet(QStringLiteral("background-color: rgb(220, 220, 220);"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/res/Deldocy.png"), QSize(), QIcon::Normal, QIcon::Off);
        DeletePB->setIcon(icon3);
        DeletePB->setAutoDefault(true);
        DeletePB->setFlat(false);
        ModelParPB = new s_tqPushButton(qt_cmp_addcompdialog);
        ModelParPB->setObjectName(QStringLiteral("ModelParPB"));
        ModelParPB->setGeometry(QRect(140, 560, 171, 23));
        PrefixLE = new s_tqLineEdit(qt_cmp_addcompdialog);
        PrefixLE->setObjectName(QStringLiteral("PrefixLE"));
        PrefixLE->setEnabled(true);
        PrefixLE->setGeometry(QRect(180, 481, 431, 20));
        PrefixLE->setStyleSheet(QStringLiteral("color: rgb(85, 85, 0);"));
        PrefixL = new s_tqLabel(qt_cmp_addcompdialog);
        PrefixL->setObjectName(QStringLiteral("PrefixL"));
        PrefixL->setEnabled(true);
        PrefixL->setGeometry(QRect(120, 482, 51, 16));
        PrefixL->setFont(font3);
        PrefixL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        isActiveCB = new s_tqCheckBox(qt_cmp_addcompdialog);
        isActiveCB->setObjectName(QStringLiteral("isActiveCB"));
        isActiveCB->setGeometry(QRect(92, 481, 21, 20));
        sizePolicy.setHeightForWidth(isActiveCB->sizePolicy().hasHeightForWidth());
        isActiveCB->setSizePolicy(sizePolicy);
        isActiveCB->setSizeIncrement(QSize(0, 0));
        isActiveCB->setBaseSize(QSize(0, 0));
        isActiveCB->setAutoFillBackground(false);
        isActiveCB->setChecked(false);
        isActiveL = new s_tqLabel(qt_cmp_addcompdialog);
        isActiveL->setObjectName(QStringLiteral("isActiveL"));
        isActiveL->setGeometry(QRect(20, 479, 71, 23));
        isActiveL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        CategoryLE = new s_tqLineEdit(qt_cmp_addcompdialog);
        CategoryLE->setObjectName(QStringLiteral("CategoryLE"));
        CategoryLE->setEnabled(false);
        CategoryLE->setGeometry(QRect(89, 520, 491, 20));
        CategoryLE->setStyleSheet(QStringLiteral("color: rgb(85, 85, 0);"));
        CategoryL = new s_tqLabel(qt_cmp_addcompdialog);
        CategoryL->setObjectName(QStringLiteral("CategoryL"));
        CategoryL->setEnabled(true);
        CategoryL->setGeometry(QRect(20, 522, 61, 16));
        CategoryL->setFont(font3);
        CategoryL->setStyleSheet(QStringLiteral("color: rgb(85, 170, 0);"));
        CategoryPB = new s_tqPushButton(qt_cmp_addcompdialog);
        CategoryPB->setObjectName(QStringLiteral("CategoryPB"));
        CategoryPB->setGeometry(QRect(590, 520, 21, 20));
        QWidget::setTabOrder(ManufCB, AddManufPB);
        QWidget::setTabOrder(AddManufPB, PartNumberLE);
        QWidget::setTabOrder(PartNumberLE, LibRefCB);
        QWidget::setTabOrder(LibRefCB, FootPrintCB);
        QWidget::setTabOrder(FootPrintCB, PEDescLE);
        QWidget::setTabOrder(PEDescLE, PENotesLE);
        QWidget::setTabOrder(PENotesLE, Par1LE);
        QWidget::setTabOrder(Par1LE, UnitsCB);
        QWidget::setTabOrder(UnitsCB, Par2LE);
        QWidget::setTabOrder(Par2LE, isNeedToAccVoltageInNameCB);
        QWidget::setTabOrder(isNeedToAccVoltageInNameCB, Par3LE);
        QWidget::setTabOrder(Par3LE, AccuracyLE);
        QWidget::setTabOrder(AccuracyLE, isNeedToAccAccuracyInNameCB);
        QWidget::setTabOrder(isNeedToAccAccuracyInNameCB, MinOpTLE);
        QWidget::setTabOrder(MinOpTLE, MaxOpTLE);
        QWidget::setTabOrder(MaxOpTLE, MaxPowerLE);
        QWidget::setTabOrder(MaxPowerLE, TKCLE);
        QWidget::setTabOrder(TKCLE, TypeLE);
        QWidget::setTabOrder(TypeLE, isSMDCB);
        QWidget::setTabOrder(isSMDCB, MarkLE);
        QWidget::setTabOrder(MarkLE, DSheetPB);
        QWidget::setTabOrder(DSheetPB, isActiveCB);
        QWidget::setTabOrder(isActiveCB, PrefixLE);
        QWidget::setTabOrder(PrefixLE, DeletePB);
        QWidget::setTabOrder(DeletePB, AcceptAndClosePB);
        QWidget::setTabOrder(AcceptAndClosePB, DeclinePB); */

        //    connect (ManufCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (PartNumberLE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (Par1LE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
        //    connect (isNeedToAccAccuracyInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));
        //    connect (isNeedToAccVoltageInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));

    /*    CompLayout = new QVBoxLayout;
        LeftLayout = new QVBoxLayout;
        RightLayout = new QVBoxLayout;
        CenterLayout = new QHBoxLayout;
        AddCompL = new s_tqLabel("Компонент Altium");
        AddCompL->setFont(font15);
        CompLayout->addWidget(AddCompL, 1);
        CompLayout->setAlignment(AddCompL, Qt::AlignRight);
        CompLayout->addWidget(CompGB, 100);
        CompLayout->addWidget(LibGB, 100);
        CompLayout->addWidget(PE3GB, 100);
        CompFrame = new s_tqFrame;
        CompFrame->setLayout(CompLayout);
    */
    //    TreeFrame->setVisible(false);
    //    MainLayout->replaceWidget(TreeFrame, CompFrame, Qt::FindDirectChildrenOnly);
    cp->setLayout(lyout);
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

void cmp_maindialog::emiterror(int er1, int er2)
{
    er1 += ER_CMPMAIN;
    emit error(er1,er2);
}
