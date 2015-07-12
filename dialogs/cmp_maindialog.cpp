#include "cmp_maindialog.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_tqlabel.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"

#include <QVBoxLayout>
#include <QFont>

cmp_maindialog::cmp_maindialog(QWidget *parent) : QDialog(parent)
{

}

cmp_maindialog::~cmp_maindialog()
{

}

void cmp_maindialog::SetupUI(int CompType, int CompTable, int CompID)
{
/*    QStringList fl = QStringList() << "Наименование";
    CompTble = tmps.toInt();
    tmps = QString::number(CompTble); // убираем старшие незначащие нули
    QStringList sl = tfl.valuesbyfield(CompLetter+"Компоненты_описание_полн",fl,"ИД",tmps); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
    if (tfl.result)
    {
        emit error(ER_COMP+tfl.result, 0x12);
        return;
    }
    // теперь надо вытащить в slavemodel все компоненты из выбранной таблицы
    fl = QStringList() << "id" << "PartNumber" << "Manufacturer";
*/
    /*
        s_tqWidget *cp1 = new s_tqWidget;
        s_tqWidget *cp2 = new s_tqWidget;
        s_tqWidget *cp3 = new s_tqWidget;
        ctw->addTab(cp1, "Текстовые");
        ctw->addTab(cp2, "Основные");
        ctw->addTab(cp3, "Дополнительные");


        s_tqGroupBox *gb = new s_tqGroupBox;
        gb->setTitle("Компонент");
        lbl = new s_tqLabel("Производитель");
        s_tqWidget *wdgt = new s_tqWidget;
    /*    PartNumberL = new s_tqLabel("Наименование");
        PartNumberL->setFont(fontB);
        PartNumberLE = new s_tqLineEdit;
        AddManufPB = new s_tqPushButton(QString("Добавить"));

        CompGBLayout = new QGridLayout;
        CompGBLayout->addWidget(ManufL, 0, 0);
        CompGBLayout->addWidget(ManufCB, 0, 1);
        CompGBLayout->addWidget(AddManufPB, 0, 2);
        CompGBLayout->addWidget(PartNumberL, 1, 0);
        CompGBLayout->addWidget(PartNumberLE, 1, 1, 1, 2);
        CompGBLayout->setColumnStretch(0, 40);
        CompGBLayout->setColumnStretch(1, 60);
        CompGB->setLayout(CompGBLayout);

        LibGB = new s_tqGroupBox;
        LibGB->setTitle("Библиотеки");
        LibRefL = new s_tqLabel("УГО (символ)");
        LibRefL->setFont(fontB);
        FootPrintL = new s_tqLabel("Посадочное место");
        FootPrintL->setFont(fontB);
        LibRefCB = new s_tqComboBox;
        FootPrintCB = new s_tqComboBox;
    //    FootPrintCB->addItem("123", QVariant(123));
    //    FootPrintCB->addItem("234", QVariant(234));

        LibGBLayout = new QGridLayout;
        LibGBLayout->addWidget(LibRefL, 0, 0);
        LibGBLayout->addWidget(LibRefCB, 0, 1);
        LibGBLayout->addWidget(FootPrintL, 1, 0);
        LibGBLayout->addWidget(FootPrintCB, 1, 1);
        LibGBLayout->setColumnStretch(0, 40);
        LibGBLayout->setColumnStretch(1, 60);
        LibGB->setLayout(LibGBLayout);

        PE3GB = new s_tqGroupBox;
        PE3GB->setTitle("Поля перечня элементов");
        PE3GB->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        PEDescL = new s_tqLabel("\"Наименование\"");
        PEDescL->setFont(fontB);
        PEDescLE = new s_tqLineEdit;
        PENotesL = new s_tqLabel("\"Примечание\"");
        PENotesLE = new s_tqLineEdit;

        PE3GBLayout = new QGridLayout;
        PE3GBLayout->addWidget(PEDescL, 0, 0);
        PE3GBLayout->addWidget(PEDescLE, 0, 1);
        PE3GBLayout->addWidget(PENotesL, 1, 0);
        PE3GBLayout->addWidget(PENotesLE, 1, 1);
        PE3GBLayout->setColumnStretch(0, 40);
        PE3GBLayout->setColumnStretch(1, 60);
        PE3GB->setLayout(PE3GBLayout);

        TechGB = new s_tqGroupBox;
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
}
