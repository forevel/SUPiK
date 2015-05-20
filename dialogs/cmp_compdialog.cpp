#include "cmp_compdialog.h"
#include "sys_acceptexist.h"
#include <QTextCodec>
#include <QStringListModel>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTableView>
#include <QPainter>
#include <QPaintEvent>
#include <QSizePolicy>

// --------------------------------------
// Конструктор
// --------------------------------------

cmp_compdialog::cmp_compdialog(QWidget *parent) : QDialog(parent)
{
//    SlaveTVModel = new s_gSqlQueryModel;
    LibRefModel = new QStringListModel;
    FootRefModel = new QStringListModel;
    CompManufModel = new QStringListModel;
    CompUnitsModel = new QStringListModel;
    s_aemodel = new QStringListModel;
    SomethingChanged = false;
    RevNotes = 0;
    curUnit = 0;

    SetupUI();
//    connect (ManufCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetSomethingChanged()));
//    connect (PartNumberLE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
//    connect (Par1LE, SIGNAL(textChanged(QString)), this, SLOT(SetSomethingChanged()));
//    connect (isNeedToAccAccuracyInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));
//    connect (isNeedToAccVoltageInNameCB, SIGNAL(clicked()), this, SLOT(VoltageOrAccuracyAccIsChecked()));
}

// --------------------------------------
// Деструктор
// --------------------------------------

cmp_compdialog::~cmp_compdialog()
{
    delete s_aemodel;
    delete CompUnitsModel;
    delete CompManufModel;
    delete FootRefModel;
    delete LibRefModel;
//    delete SlaveTVModel;
}

void cmp_compdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/CompWallpaper.jpg"));

    event->accept();
}

// Настройка интерфейса

void cmp_compdialog::SetupUI()
{
    QFont fontB, font15;

    font15.setPointSize(15);
    fontB.setBold(true);

    MainTW = new s_tqTreeWidget;
    MainTW->header()->setVisible(false);
    MainTW->setColumnCount(2);
    MainTW->setIndentation(20);
    MainTW->setContextMenuPolicy(Qt::CustomContextMenu);
    MainTW->hideColumn(1);
    s_tqFrame *line = new s_tqFrame;
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setMaximumWidth(2);
    SlaveTV = new s_tqTableView;
    TreeFrame = new s_tqFrame;

    TreeFrameLayout = new QHBoxLayout;
    TreeFrameLayout->addWidget(MainTW, 10);
    TreeFrameLayout->addWidget(line);
    TreeFrameLayout->addWidget(SlaveTV, 30);
    TreeFrame->setLayout(TreeFrameLayout);
    MainLayout = new QGridLayout;
    MainLayout->addWidget(TreeFrame);
    setLayout(MainLayout);

    CompGB = new s_tqGroupBox;
    CompGB->setTitle("Компонент");
    ManufL = new s_tqLabel("Производитель");
    ManufL->setFont(fontB);
    ManufCB = new s_tqComboBox;
    ManufCB->setMaxVisibleItems(7);
    ManufCB->setInsertPolicy(QComboBox::InsertAtBottom);
    PartNumberL = new s_tqLabel("Наименование");
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

    CompLayout = new QVBoxLayout;
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

//    TreeFrame->setVisible(false);
//    MainLayout->replaceWidget(TreeFrame, CompFrame, Qt::FindDirectChildrenOnly);

}

// --------------------------------------
// Добавляем новый элемент
// --------------------------------------

void cmp_compdialog::AddNewItem()
{
    GetNextId();
    SectionLE->setText(pc.dbs[pc.dbs_index].alias);
    FillNullDialog();
    Action=INSERT;
    RevNotes = 1;
    idCompLE->setText(QString::number(pc.idRecord));
    SomethingChanged = false;
}

void cmp_compdialog::GetNextId()
{
    QSqlQuery get_next_id = QSqlQuery(pc.alt);
    get_next_id.exec("SELECT MAX(id) FROM " + pc.dbs[pc.dbs_index].dbs + ";");
    get_next_id.next();
    pc.idRecord = get_next_id.value(0).toInt(0) + 1;
}

// --------------------------------------
// Заполняем элементы диалогового окна (комбо-боксы)
// --------------------------------------

void cmp_compdialog::FillNullDialog()
{
    QStringList tmpList;
    QString tmpString;
    int i = 0;

    ClearDialog();

    // заполняем библиотеки
    tmpList = pc.dbs[pc.dbs_index].dbs.split("_", QString::KeepEmptyParts);

    PathString = "";

    tmpString = tmpList.last();
    while ((tmpList.value(i, "") != "") && (tmpList.value(i, "") != tmpString))
    {
        PathString += "/";
        PathString += tmpList.value(i++);
    }

    tmpString = pc.PathToLibs + "/Symbols" + PathString + "/" + pc.dbs[pc.dbs_index].dbs + ".SchLib";

    LibRefModel->setStringList(GetListFromFile(tmpString, pc.symfind));
    LibRefCB->setModel(LibRefModel);
    LibRefCB->setCurrentIndex(0);

    tmpString = pc.PathToLibs + "/Footprints/" + PathString + "/" + pc.dbs[pc.dbs_index].dbs + ".PcbLib";

    SetParNames();
    UpdateUnitsCombobox();
    UpdateManufCombobox();

    ManufCB->setCurrentIndex(0);

    FootRefModel->setStringList(GetListFromFile(tmpString, pc.footfind));
    FootPrintCB->setModel(FootRefModel);
    FootPrintCB->setCurrentIndex(0);
}

// --------------------------------------
// Заполняем элементы диалогового окна (комбо-боксы) данными из БД
// --------------------------------------

void cmp_compdialog::FillDialog (QString PartNumber)
{
    QSqlQuery get_data_from_db(pc.alt);

    QString tmpString = "SELECT `id`, `Library Ref`, `Footprint Ref`,"\
            "`Sim Description`,`Sim File`,`Sim Model Name`,`Sim Parameters`,"\
            "`Manufacturer`,`PartNumber`,`Package`,`Marking`,`NominalValue`,"\
            "`NominalVoltage`,`Tolerance`,`OpTemperaturen`,`OpTemperaturem`,"\
            "`Pmax`,`TC`,`prefix`,`HelpURL`,`RevNotes`,"\
            "`Discontinued`,`Description`,`Notes`,`isSMD`,`Nominal`,`Unit`,`Par3` FROM " \
            + pc.dbs[pc.dbs_index].dbs + " WHERE PartNumber=\"" \
            + PartNumber + "\";";
    get_data_from_db.exec(tmpString);
    get_data_from_db.next();
    if (get_data_from_db.isValid());
    else
    {
        QMessageBox::warning(this,"Внимание!",\
// 0.4                             "Запись в БД не найдена!",
                             get_data_from_db.lastError().text(), \
                             QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }


    // поле id в поле id
    tmpString = get_data_from_db.value(0).toString();
    pc.idRecord = tmpString.toInt(0);

    SectionLE->setText(pc.dbs[pc.dbs_index].alias);
    FillNullDialog();
    Action=INSERT;
    idCompLE->setText(QString::number(pc.idRecord));

    // поле LibRef в комбо-бокс LibRefCB
    tmpString=get_data_from_db.value(1).toString();
    if (LibRefCB->findText(tmpString) == -1)
    {
        QMessageBox::warning(this,"Внимание!",\
                             "Не найдено УГО в библиотеке!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    LibRefCB->setCurrentText(tmpString);

    // поле FootRef в комбо-бокс FootPrintCB
    tmpString=get_data_from_db.value(2).toString();
    if (FootPrintCB->findText(tmpString) == -1)
    {
        QMessageBox::warning(this,"Внимание!",\
                             "Не найден футпринт в библиотеке!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    FootPrintCB->setCurrentText(tmpString);

    pc.CompModelData.MDescLE = get_data_from_db.value(3).toString(); // 0.4b
    pc.CompModelData.MFileLE = get_data_from_db.value(4).toString(); // 0.4b
    pc.CompModelData.MNameCB = get_data_from_db.value(5).toString(); // 0.4b
    pc.CompModelData.MParLE = get_data_from_db.value(6).toString(); // 0.4b

    tmpString=get_data_from_db.value(7).toString();
    if (ManufCB->findText(tmpString) == -1)
    {
        QMessageBox::warning(this,"Внимание!",\
                             "Не найден производитель!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    ManufCB->setCurrentText(tmpString);

    if (ManufCB->currentText() == "НКП")
    {
        isNeedToAccAccuracyInNameCB->setEnabled(true);
        isNeedToAccVoltageInNameCB->setEnabled(true);
    }
    else
    {
        isNeedToAccAccuracyInNameCB->setEnabled(false);
        isNeedToAccVoltageInNameCB->setEnabled(false);
    }

    TypeLE->setText(get_data_from_db.value(9).toString());
    MarkLE->setText(get_data_from_db.value(10).toString());
    tmpString = get_data_from_db.value(11).toString(); // 0.34-aj
    Par1LE->setText(tmpString.split(" ").value(0)); // 0.34-aj
    Par2LE->setText(get_data_from_db.value(12).toString());
    AccuracyLE->setText(get_data_from_db.value(13).toString());
    MinOpTLE->setText(get_data_from_db.value(14).toString());
    MaxOpTLE->setText(get_data_from_db.value(15).toString());
    MaxPowerLE->setText(get_data_from_db.value(16).toString());
    TKCLE->setText(get_data_from_db.value(17).toString());
    PrefixLE->setText(get_data_from_db.value(18).toString());
    DSheetLE->setText(get_data_from_db.value(19).toString());
    RevNotes = get_data_from_db.value(20).toInt(0);
    if (get_data_from_db.value(21).toString() == "0")
        isActiveCB->setChecked(true);
    else
        isActiveCB->setChecked(false);
    PEDescLE->setText(get_data_from_db.value(22).toString());
    PENotesLE->setText(get_data_from_db.value(23).toString());

    if (get_data_from_db.value(24).toString() == "0")
        isSMDCB->setChecked(false); // 0.4 false <=> true
    else
        isSMDCB->setChecked(true);

    if (get_data_from_db.value(25).toString() != "")
        Par1LE->setText(get_data_from_db.value(25).toString());

    QSqlQuery get_units (pc.ent);
    get_units.exec("SELECT `units` FROM `units` WHERE `idunits`=" + get_data_from_db.value(26).toString() + ";");
    get_units.next();
    if (get_units.isValid())
        UnitsCB->setCurrentText(get_units.value(0).toString());
    else
        UnitsCB->setCurrentText(tmpString.split(" ").value(1));

    Par3LE->setText(get_data_from_db.value(27).toString());

    PartNumberLE->setText(get_data_from_db.value(8).toString());

    SomethingChanged = false;
}

// --------------------------------------
// Очистка диалога
// --------------------------------------

void cmp_compdialog::ClearDialog ()
{
    LibRefCB->setCurrentIndex(-1);
    FootPrintCB->setCurrentIndex(-1);

    pc.CompModelData.MDescLE = ""; // 0.4b
    pc.CompModelData.MFileLE = ""; // 0.4b
    pc.CompModelData.MNameCB = ""; // 0.4b
    pc.CompModelData.MParLE = ""; // 0.4b

    ManufCB->setCurrentIndex(-1);

    PartNumberLE->setText("");
    TypeLE->setText("");
    MarkLE->setText("");
    Par1LE->setText("");
    Par2LE->setText("");
    AccuracyLE->setText("");
    MinOpTLE->setText("");
    MaxOpTLE->setText("");
    MaxPowerLE->setText("");
    TKCLE->setText("");
    PrefixLE->setText("");
    DSheetLE->setText("");
    isActiveCB->setChecked(false);
    PEDescLE->setText("");
    PENotesLE->setText("");
    isSMDCB->setChecked(false);
    UnitsCB->clear();
    Par3LE->setText("");
}

// --------------------------------------
// Считываем информацию об элементах в библиотеке по шаблону
// --------------------------------------

QStringList cmp_compdialog::GetListFromFile (QString filename, QByteArray &StringToFind)
{
    QStringList tmpList;
    QString tmpString;
    char *tmpChar;
    int filepos = 0;

    tmpList.clear();
    QFile file;
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Warning!",\
                              "Невозможно открыть файл библиотеки!",\
                              QMessageBox::Ok, QMessageBox::NoButton);
        return QStringList();
    }

    pc.data = file.readAll();
    while ((filepos = pc.data.indexOf(StringToFind, filepos)) != -1)
    {
        tmpChar = pc.data.data();
        tmpChar += filepos;
        while (*tmpChar)
        {
            if (tmpChar[0] == '=')
            {
                tmpChar++;
                filepos++;
                tmpString = "";
                while ((tmpChar[0]) && (tmpChar[0] != '|'))
                {
                    tmpString += QString::fromLocal8Bit(tmpChar,1);
                    tmpChar++;
                    filepos++;
                }
                break;
            }
            tmpChar++;
            filepos++;
        }
        tmpList << tmpString;
    }
    file.close();
    return tmpList;
}

// --------------------------------------
// Отменили работу с диалогом
// --------------------------------------

void cmp_compdialog::DeclinePBClicked()
{
    this->close();
}

// --------------------------------------
// Захотели записать и закрыть
// --------------------------------------

void cmp_compdialog::AcceptAndClosePBClicked()
{
    if (SomethingChanged && (Action == UPDATE))
    {
        QMessageBox tmpMB;
        tmpMB.setText("Изменить текущую запись или создать новую?");
//        tmpMB.setInformativeText("Записать их?");
        QPushButton *tmpOkPB = tmpMB.addButton("Изменить", QMessageBox::AcceptRole);
        QPushButton *tmpCancelPB = tmpMB.addButton("Создать", QMessageBox::RejectRole);
        tmpMB.exec();
        if (tmpMB.clickedButton() == tmpOkPB)
            Action = UPDATE;
        else if (tmpMB.clickedButton() == tmpCancelPB)
        {
            GetNextId();
            idCompLE->setText(QString::number(pc.idRecord));
            Action = INSERT;
        }
        else
            return;
    }
    if (CheckAndAdd())
        this->close();
}

void cmp_compdialog::SetSomethingChanged ()
{
    SomethingChanged = true;
}

// --------------------------------------
// Проверить содержимое диалогового окна
// --------------------------------------

bool cmp_compdialog::CheckAndAdd()
{
    QString tmpString;
    QStringList tmpList;

    if (PartNumberLE->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning!",\
                             "Поле \"Наименование\"\n не может быть пустым!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    if (PEDescLE->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning!",\
                             "Поле \"Наименование\"\n не может быть пустым!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    // проверка на наличие в БД уже такой записи
    if (Action == INSERT)
    {
        QSqlQuery get_filter(pc.alt);
        get_filter.exec("SELECT PartNumber FROM " + pc.dbs[pc.dbs_index].dbs + \
                        " WHERE PartNumber=\"" + PartNumberLE->text() + "\";");
        get_filter.next();
        if (get_filter.isValid())
        {
            QMessageBox::warning(this,"Warning!",\
                                 "Такая запись уже имеется в БД!",\
                                 QMessageBox::Ok, QMessageBox::NoButton);
            return false;
        }

        get_filter.exec("SELECT PartNumber FROM " + pc.dbs[pc.dbs_index].dbs + \
                        " WHERE PartNumber LIKE '%" + PartNumberLE->text().left(3) + "%';");
        while (get_filter.next())
        {
            tmpList << get_filter.value(0).toString();
        }

        s_aemodel->setStringList(tmpList);
        if (!(s_aemodel->rowCount()));
        else
        {
            sys_acceptexist *s_ae = new sys_acceptexist;
            s_ae->ADTV->setModel(s_aemodel);
            s_ae->ADTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
            s_ae->ADTV->resizeColumnsToContents();
            s_ae->ADTV->resizeRowsToContents();
            if (s_ae->exec() == QDialog::Accepted);
            else return false;
        }
    }
    // если всё в порядке, пишем в базу новую запись
    QString SymPath=pc.PathToLibs + "/Symbols" + PathString + "/" + pc.dbs[pc.dbs_index].dbs + \
            ".SchLib";
    QString FootPath=pc.PathToLibs + "/Footprints" + PathString + "/" + pc.dbs[pc.dbs_index].dbs + \
            ".PcbLib";

    QSqlQuery get_units (pc.ent);

    get_units.exec("SELECT `idunits` FROM `units` WHERE `units`=\"" + UnitsCB->currentText() + "\";");
    if (get_units.next())
        curUnit = get_units.value(0).toInt(0);
    else
        curUnit = 0;

    if (Action == INSERT)
    {
        RevNotes = 1;

        if (DSheetLE->text().mid(0,1) == "\\") // преобразование слэшей в пути к DSheet в нужные символы
            tmpString = DSheetLE->text().replace("\\","\\\\");
        else
            tmpString = DSheetLE->text().replace("/","\\\\");

        tmpString="INSERT INTO " + pc.dbs[pc.dbs_index].dbs + \
                " (`Library Ref`,`Library Path`,`Footprint Ref`,`Footprint Path`,"\
                "`Sim Description`,`Sim File`,`Sim Model Name`,`Sim Parameters`,"\
                "`Manufacturer`,`PartNumber`,`Package`,`Marking`,`NominalValue`,"\
                "`NominalVoltage`,`Tolerance`,`OpTemperaturen`,`OpTemperaturem`,"\
                "`Pmax`,`TC`,`Creator`,`Modify Date`,`prefix`,`HelpURL`,`RevNotes`,"\
                "`Discontinued`,`Description`,`Notes`,`isSMD`,`Nominal`,`Unit`,`Par3`,`deleted`) VALUES (\""\
                + LibRefCB->currentText() + "\",\"" \
                + SymPath.replace("/","\\\\") + "\",\"" \
                + FootPrintCB->currentText() + "\",\"" \
                + FootPath.replace("/","\\\\") + "\",\"" \
                + pc.CompModelData.MDescLE + "\",\"" \
                + pc.CompModelData.MFileLE + "\",\"" \
                + pc.CompModelData.MNameCB + "\",\"" \
                + pc.CompModelData.MParLE + "\",\"" \
                + ManufCB->currentText() + "\",\"" \
                + PartNumberLE->text() + "\",\"" \
                + TypeLE->text() + "\",\"" \
                + MarkLE->text() + "\",\"" \
                + Par1LE->text() + " " + UnitsCB->currentText() + "\",\"" \
                + Par2LE->text() + "\",\"" \
                + AccuracyLE->text() + "\",\"" \
                + MinOpTLE->text() + "\",\"" \
                + MaxOpTLE->text() + "\",\"" \
                + MaxPowerLE->text() + "\",\"" \
                + TKCLE->text() + "\"," \
                + QString::number(pc.idPers) + ",\"" \
                + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\",\"" \
                + PrefixLE->text() + "\",\"" \
                + tmpString + "\"," \
                + QString::number(RevNotes) + ",\"" \
                + QString::number(static_cast<int>(!isActiveCB->isChecked())) + "\",\"" \
                + PEDescLE->text() + "\",\"" \

                + PENotesLE->text() + "\"," \
                + QString::number(static_cast<int>(isSMDCB->isChecked())) + ",\"" \
                + Par1LE->text() + "\",\"" \
                + QString::number(curUnit) + "\",\"" \
                + Par3LE->text() + "\",0);";

    }
    if (Action == UPDATE)
    {
        RevNotes++;

        if (DSheetLE->text().mid(0,1) == "\\") // преобразование слэшей в пути к DSheet в нужные символы
            tmpString = DSheetLE->text().replace("\\","\\\\");
        else
            tmpString = DSheetLE->text().replace("/","\\\\");

        tmpString = "UPDATE " + pc.dbs[pc.dbs_index].dbs + \
                + " SET `Library Ref`=\"" \
                + LibRefCB->currentText() + "\",`Library Path`=\"" \
                + SymPath.replace("/","\\\\") + "\",`Footprint Ref`=\"" \
                + FootPrintCB->currentText() + "\",`Footprint Path`=\"" \
                + FootPath.replace("/","\\\\") + "\",`Sim Description`=\"" \
/* 0.4b                + MDescLE->text() + "\",`Sim File`=\"" \
                + MFileLE->text() + "\",`Sim Model Name`=\"" \
                + MNameCB->currentText() + "\",`Sim Parameters`=\"" \
                + MParLE->text() + "\",`Manufacturer`=\"" \ */
// 0.4b {{{
                + pc.CompModelData.MDescLE + "\",`Sim File`=\"" \
                + pc.CompModelData.MFileLE + "\",`Sim Model Name`=\"" \
                + pc.CompModelData.MNameCB + "\",`Sim Parameters`=\"" \
                + pc.CompModelData.MParLE + "\",`Manufacturer`=\"" \
// }}} 0.4b
                + ManufCB->currentText() + "\",`PartNumber`=\"" \
                + PartNumberLE->text() + "\",`Package`=\"" \
                + TypeLE->text() + "\",`Marking`=\"" \
                + MarkLE->text() + "\",`NominalValue`=\"" \
                + Par1LE->text() + " " + UnitsCB->currentText() + "\",`NominalVoltage`=\"" \
                + Par2LE->text() + "\",`Tolerance`=\"" \
                + AccuracyLE->text() + "\",`OpTemperaturen`=\"" \
                + MinOpTLE->text() + "\",`OpTemperaturem`=\"" \
                + MaxOpTLE->text() + "\",`Pmax`=\"" \
                + MaxPowerLE->text() + "\",`TC`=\"" \
// 0.4                + TKCLE->text() + "\",`Creator`="
// 0.4                + QString::number(pc.idPers) + ",`Modify Date`=\""
                + TKCLE->text() + "\",`Modify Date`=\"" \
                + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +"\",`prefix`=\"" \
                + PrefixLE->text() + "\",`HelpURL`=\"" \
                + tmpString + "\",`RevNotes`=" \
                + QString::number(RevNotes) + ",`Discontinued`=\"" \
                + QString::number(static_cast<int>(!isActiveCB->isChecked())) + "\",`Description`=\"" \
                + PEDescLE->text() + "\",`Notes`=\"" \

                + PENotesLE->text() + "\",`deleted`=0,`isSMD`=\"" \
// 0.4               + QString::number(static_cast<int>(!isSMDCB->isChecked())) + "\",`Nominal`=\""
                + QString::number(static_cast<int>(isSMDCB->isChecked())) + "\",`Nominal`=\"" \
                + Par1LE->text() + "\",`Unit`=\"" \
                + QString::number(curUnit) + "\",`Par3`=\"" \
                + Par3LE->text() + "\" WHERE id=" + idCompLE->text() + ";";


    }
    QSqlQuery insert_comp (pc.alt);
    insert_comp.exec(tmpString);
    // проверка успешности записи в БД
    if (insert_comp.isActive())
    {
        QMessageBox::information(this,"Warning!",\
                             "Записано успешно",\
                             QMessageBox::Ok, QMessageBox::NoButton);
        if (Action == INSERT)
        {
            if (QMessageBox::question(this,\
                                  "Записать в БД номенклатуры?", \
                                  "Вы хотите добавить компонент в БД номенклатуры?", \
                                  QMessageBox::Yes|QMessageBox::No, \
                                  QMessageBox::Yes) == QMessageBox::Yes)
            {
                // Вызвать метод из НК добавления нового элемента
//                qa_AddNkDialog.AddNewItem();
                QSqlQuery get_altium_sections (pc.ent);
                get_altium_sections.exec("SELECT `dbs`,`alias` FROM altium;");
                while ((get_altium_sections.next()) \
                       && (get_altium_sections.value(0).toString() != pc.dbs[pc.dbs_index].dbs));

                if (get_altium_sections.value(1).isValid())
                {
//                    qa_AddNkDialog.SectionCB->setCurrentText("alt." + get_altium_sections.value(0).toString());
                }
                else
                {
                    QMessageBox::warning(this,"Warning!",\
                                         "Не найден раздел БД altium!",\
                                         QMessageBox::Ok, QMessageBox::NoButton);
                    return false;
                }

/*//                qa_AddNkDialog.ManufCB->setCurrentText(ManufCB->currentText());
//                qa_AddNkDialog.PartNumberLE->setText(PartNumberLE->text());
//                qa_AddNkDialog.idSectLE->setText(idCompLE->text());
//                qa_AddNkDialog.idSldwLE->setText("0");
//                qa_AddNkDialog.idSmgeLE->setText("0");
//                QList<QTreeWidgetItem *>NkSearchIndex = qa_AddNkDialog.NkTree->findItems(get_altium_sections.value(1).toString(), \
//                                      Qt::MatchFlags (Qt::MatchWrap | Qt::MatchRecursive), 1);
//                if (!NkSearchIndex.isEmpty())
//                {
//                    qa_AddNkDialog.NkTree->expandAll();
//                    qa_AddNkDialog.NkTree->setCurrentItem(NkSearchIndex.first());
//                }

//                qa_AddNkDialog.exec(); */
            }
        }
        return true;
    }
    else
    {
        QMessageBox::warning(this,"Warning!",\
                             insert_comp.lastError().text(),\
                             QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
}


// --------------------------------------
// Захотели выбрать файл описания
// --------------------------------------

void cmp_compdialog::DSheetPBClicked()
{
    QString DSheedbs = QFileDialog::getOpenFileName(this, \
                                      "Выберите файл описания", \
                                      pc.PathToLibs + "//DSheet//",
                                      "PDF Files (*.pdf)");
    DSheetLE->setText(DSheedbs);
}

void cmp_compdialog::AddManufPBClicked()
{
//    qa_AddManufDialog.AddNewItem();
//    qa_AddManufDialog.exec();
    UpdateManufCombobox();
}

void cmp_compdialog::UpdateManufCombobox ()
{
    QStringList tmpList;
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
    ManufCB->setCurrentText(pc.InterchangeString);
}

void cmp_compdialog::ManufCBIndexChanged(const QString &arg1)
{
    if (arg1 == "НКП")
    {
        isNeedToAccAccuracyInNameCB->setEnabled(true);
        isNeedToAccVoltageInNameCB->setEnabled(true);
        PrefixLE->setVisible(true); // 0.4b
        PrefixL->setVisible(true);
        PartNumberLE->setEnabled(false);
        PEDescLE->setEnabled(false);
        UpdatePartNumber();
        ConnectPartNumber();
    }
    else
    {
        isNeedToAccAccuracyInNameCB->setEnabled(false);
        isNeedToAccVoltageInNameCB->setEnabled(false);
        PrefixL->setVisible(false); // 0.4b
        PrefixLE->setVisible(false);
        PartNumberLE->setEnabled(true);
        PEDescLE->setEnabled(true);
        DisconnectPartNumber();
    }
}

void cmp_compdialog::SetParNames()
{
    QSqlQuery get_parnames(pc.sup);

}

void cmp_compdialog::UpdateUnitsCombobox()
{
//    int i;

//    CompUnitsModel->setStringList(pc.KList);

/*    for (i = 0; i < NUM_LISTS; i++)
    {
        if (SectionLE->text() == pc.unitlist[i].section)
            CompUnitsModel->setStringList(*(pc.unitlist[i].list));
    } */

//    UnitsCB->setModel(CompUnitsModel);
//    UnitsCB->setCurrentIndex(0);
}


bool cmp_compdialog::ConnectPartNumber()
{
    handle1 = connect(PrefixLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle2 = connect(Par1LE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle3 = connect(AccuracyLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle4 = connect(TypeLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle5 = connect(MaxPowerLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle6 = connect(Par2LE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle7 = connect(UnitsCB, SIGNAL(currentTextChanged(QString)) , this, SLOT(UpdatePartNumber()));
    bool result = handle1 && handle2 && handle3 && handle4 && handle5 && handle6 && handle7;
    return result;
}

void cmp_compdialog::UpdatePartNumber()
{
    QString tmpString;
    if (SectionLE->text() == "Конденсаторы")
    {
        tmpString = PrefixLE->text() + " Конд. " + Par1LE->text() + " " + UnitsCB->currentText() + ",";
        if (isNeedToAccVoltageInNameCB->isChecked())
            tmpString += Par2LE->text() + ",";
        if (isNeedToAccAccuracyInNameCB->isChecked())
            tmpString += AccuracyLE->text() + ",";
        tmpString += "тип " + TypeLE->text();
    }
    else if (SectionLE->text() == "Резисторы")
    {

        tmpString = PrefixLE->text() + " Рез. " + MaxPowerLE->text() + "-" \
                + Par1LE->text() + " " + UnitsCB->currentText() + "-";
        if (isNeedToAccAccuracyInNameCB->isChecked())
            tmpString += AccuracyLE->text() + ",";
        tmpString += "тип " + TypeLE->text();
    }
    else
        tmpString = PrefixLE->text();
    PartNumberLE->setText(tmpString);
    PEDescLE->setText(tmpString);
}

bool cmp_compdialog::DisconnectPartNumber()
{
    if (handle1)
        if (!disconnect(handle1)) return false;
    if (handle2)
        if (!disconnect(handle2)) return false;
    if (handle3)
        if (!disconnect(handle3)) return false;
    if (handle4)
        if (!disconnect(handle4)) return false;
    if (handle5)
        if (!disconnect(handle5)) return false;
    if (handle6) // 0.34-ah
        if (!disconnect(handle6)) return false;
    if (handle7) // 0.4
        if (!disconnect(handle7)) return false; // 0.4
    return true;
}

void cmp_compdialog::DeletePBClicked()
{
    QString tmpString;
    if (pc.access & 0x07)
    {
//        qtdd.ManufLE->setText(ManufCB->currentText());
//        qtdd.PartNumberLE->setText(PartNumberLE->text());
//        qtdd.isAccepted = false;
//        qtdd.exec();
//        if (qtdd.isAccepted == true)
//        {
            QSqlQuery delete_comp_from_db (pc.alt);

            tmpString = "UPDATE " + pc.dbs[pc.dbs_index].dbs + " SET `deleted`=1 WHERE `Manufacturer`=\"" + ManufCB->currentText() \
                    + "\" AND `PartNumber`=\"" + PartNumberLE->text() + "\";";
            delete_comp_from_db.exec(tmpString);
            if (delete_comp_from_db.isActive())
            {
                QMessageBox::information(this,"Warning!",\
                                     "Удалено успешно",\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(this,"Ошибка при удалении!",\
                                     delete_comp_from_db.lastError().text(),\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            }
//        }
    }
    else
    {
        QMessageBox::warning(this,"Warning!",\
                             "Недостаточно привилегий для выполнения действия!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    this->close();
}

void cmp_compdialog::VoltageOrAccuracyAccIsChecked()
{
    if (ManufCB->currentText() == "НКП")
        UpdatePartNumber();
}

void cmp_compdialog::ModelParPBClicked()
{
//    qa_EditCompModelDialog.FillDialog();
//    qa_EditCompModelDialog.exec();
}
