#include "cmp_maindialog.h"
#include <QTabWidget>
#include <QPainter>
#include <QStringListModel>
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../s_2cdialog.h"
#include "../messagebox.h"
#include <QVBoxLayout>
#include <QFont>

cmp_maindialog::cmp_maindialog(int Mode, QWidget *parent) : QDialog(parent)
{
    CompMode = Mode;
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowContextHelpButtonHint);
    Changed = PartNumberCreatorEnabled = ChangeEnabled = false;
    RevNotes = 0;
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

    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Создано (изменено) ");
    hlyout->addWidget(lbl,1);
    le = new s_tqLineEdit;
    le->setEnabled(false);
    le->setObjectName("creatorle");
    hlyout->addWidget(le,1);
    le = new s_tqLineEdit;
    le->setEnabled(false);
    le->setObjectName("modifydatele");
    hlyout->addWidget(le,1);
    hlyout->addStretch(3);
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
    connect(pb,SIGNAL(clicked()),this,SLOT(CancelAndClose()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);
    setLayout(lyout);
    SetLEData("creatorle",pc.Pers);
    SetLEData("modifydatele",pc.DateTime);
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
    QStringList fl = QStringList() << "Наименование" << "Описание" << "Путь к файлам";
    QStringList tblesl;
    QString table, idfield;
    table = sl.at(CompType)+"Компоненты_описание_полн";
    idfield = "ИД";
    tfl.valuesbyfield(table,fl,idfield,QString::number(CompTable), tblesl);
    if ((tfl.result == TFRESULT_ERROR) || (tblesl.size() < 3))
    {
        WARNMSG("");
        return;
    }
    this->CompTble = tblesl.at(0);
    this->CompId = QString::number(CompID);
    this->CompType = QString::number(CompType);
    this->CompUrlPrefix = tblesl.at(2);
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("section");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    le->setText(sectsl.at(CompType));
    le = this->findChild<s_tqLineEdit *>("subsection");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    le->setText(tblesl.at(1));
    le = this->findChild<s_tqLineEdit *>("id");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    le->setText(CompId);
    fl.clear();
    switch (CompType)
    {
    case CTYPE_ALT:
    {
        SetAltDialog();
        fl << "Library Ref" << "Footprint Ref" << "Sim Description" << "Sim File" << \
              "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
              "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
              "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
              "Nominal" << "Unit" << "par4" << "par5";
        QStringList vl = sqlc.GetValuesFromTableByField(CompDb,CompTble,fl,"id",CompId);
        if ((sqlc.result) || (vl.size() == 0)); // новый элемент, ещё нет в БД
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
    ChangeEnabled = true;
}

void cmp_maindialog::SetAltDialog()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqWidget *cp = this->findChild<s_tqWidget *>("cp1");
    if (cp == 0)
    {
        DBGMSG;
        return;
    }
    s_tqWidget *cp2 = this->findChild<s_tqWidget *>("cp2");
    if (cp2 == 0)
    {
        DBGMSG;
        return;
    }
    s_tqGroupBox *gb = new s_tqGroupBox;
    gb->setTitle("Компонент");
    QFont font;
    font.setBold(true);
    gb->setFont(font);
    QGridLayout *glyout = new QGridLayout;
    // добавляем скрытое поле prefix
    s_tqLabel *lbl = new s_tqLabel("Префикс");
    lbl->setObjectName("prefixl");
    lbl->setVisible(false);
    glyout->addWidget(lbl,0,0,1,1);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("prefixle");
    le->setVisible(false);
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,0,1,1,2);
    lbl = new s_tqLabel("Наименование");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("partnumber");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,1,1,1,2);
    lbl = new s_tqLabel("Производитель");
    glyout->addWidget(lbl,2,0,1,1);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup("2.2..Производители_сокращ.Наименование");
    cw->setObjectName("manufacturer");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SomethingChanged()));
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(EnablePartNumberCreator(QVariant)));
    glyout->addWidget(cw,2,1,1,1);
    s_tqPushButton *pb = new s_tqPushButton(QString("Добавить"));
    connect(pb,SIGNAL(clicked()),this,SLOT(AddManuf()));
    glyout->addWidget(pb,2,2,1,1);
    glyout->setColumnStretch(0, 0);
    glyout->setColumnStretch(1, 1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Библиотеки");
    gb->setFont(font);
    lbl = new s_tqLabel("УГО (символ)");
    glyout = new QGridLayout;
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SomethingChanged()));
    cw->setObjectName("libref");
/*    int i = 0;
    QStringList tmpsl = CompTble.split("_", QString::KeepEmptyParts);
    QString PathString = "";
    QString tmps = tmpsl.last();
    while ((tmpsl.value(i, "") != "") && (tmpsl.value(i, "") != tmps))
    {
        PathString += "/";
        PathString += tmpsl.value(i++);
    }
    tmps = pc.PathToLibs + "Symbols" + PathString + "/" + CompTble + ".SchLib"; */
    QString tmps = pc.PathToLibs + "/Symbols" + CompUrlPrefix + CompTble + ".SchLib";
    cw->Setup("2.17.."+tmps+"."+pc.symfind);
    glyout->addWidget(cw,0,1,1,1);
    lbl = new s_tqLabel("Посадочное место");
    glyout->addWidget(lbl,1,0,1,1);
    cw = new s_tqChooseWidget(true);
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SomethingChanged()));
    cw->setObjectName("footref");
//    tmps = pc.PathToLibs + "Footprints" + PathString + "/" + CompTble + ".PcbLib";
    tmps = pc.PathToLibs + "/Footprints" + CompUrlPrefix + CompTble + ".PcbLib";
    cw->Setup("2.17.."+tmps+"."+pc.footfind);
    glyout->addWidget(cw,1,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb=new s_tqGroupBox;
    glyout = new QGridLayout;
    gb->setTitle("Поля ПЭ3");
    gb->setFont(font);
    lbl = new s_tqLabel("\"Наименование\"");
    glyout->addWidget(lbl,0,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("pe3name");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,0,1,1,1);
    lbl = new s_tqLabel("\"Примечание\"");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("pe3notes");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,1,1,1,1);
    glyout->setColumnStretch(0,0);
    glyout->setColumnStretch(1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    s_tqCheckBox *chb;
    s_tqComboBox *cb;
    gb=new s_tqGroupBox;
    gb->setFont(font);
    glyout = new QGridLayout;
    gb->setTitle("Характеристики");
    for (int i=0; i<5; i++)
    {
        lbl = new s_tqLabel("");
        lbl->setObjectName("par"+QString::number(i)+"lbl");
        glyout->addWidget(lbl,i,0,1,1);
        le = new s_tqLineEdit;
        le->setObjectName("par"+QString::number(i)+"le");
        connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
        glyout->addWidget(le,i,1,1,1);
        cb = new s_tqComboBox;
        cb->setObjectName("par"+QString::number(i)+"cb");
        glyout->addWidget(cb,i,2,1,1);
    }
    lbl = new s_tqLabel("Мин. раб. температура");
    glyout->addWidget(lbl,6,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mintemple");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,6,1,1,3);
    lbl = new s_tqLabel("Макс. раб. температура");
    glyout->addWidget(lbl,7,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("maxtemple");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,7,1,1,3);
    lbl = new s_tqLabel("Корпус компонента");
    glyout->addWidget(lbl,8,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("packagele");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,8,1,1,3);
    QHBoxLayout *hlyout = new QHBoxLayout;
    chb = new s_tqCheckBox;
    chb->setText("Компонент планарный (SMD)");
    chb->setObjectName("issmdchb");
    connect(chb,SIGNAL(toggled(bool)),this,SLOT(SomethingChanged()));
    chb->setLayoutDirection(Qt::RightToLeft);
    hlyout->addWidget(chb);
    chb = new s_tqCheckBox;
    chb->setText("Компонент выпускается");
    chb->setObjectName("isactivechb");
    connect(chb,SIGNAL(toggled(bool)),this,SLOT(SomethingChanged()));
    chb->setLayoutDirection(Qt::RightToLeft);
    hlyout->addWidget(chb);
    glyout->addLayout(hlyout,9,0,1,4);
    glyout->setColumnMinimumWidth(2, 60);
    gb->setLayout(glyout);
    lyout->addWidget(gb);
    lyout->addStretch(1);
    cp->setLayout(lyout);

    lyout = new QVBoxLayout;
    gb = new s_tqGroupBox;
    gb->setTitle("Дополнительные параметры");
    gb->setFont(font);
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Точность");
    glyout->addWidget(lbl,0,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("accuracyle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,0,1,1,3);
    lbl = new s_tqLabel("Макс. рассеив. мощность");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("maxpowerle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,1,1,1,3);
    lbl = new s_tqLabel("ТКС (ТКЕ)");
    glyout->addWidget(lbl,2,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("tkcle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,2,1,1,3);
    lbl = new s_tqLabel("Маркировка корпуса");
    glyout->addWidget(lbl,3,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("markingle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,3,1,1,3);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Описание компонента");
    gb->setFont(font);
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Файл описания компонента");
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    cw->Setup("2.15..PDF Documents (*_pdf)");
    cw->setObjectName("dsheetcw");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SomethingChanged()));
    glyout->addWidget(cw,0,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Модель");
    gb->setFont(font);
    glyout = new QGridLayout;
    lbl = new s_tqLabel("Файл описания модели");
    glyout->addWidget(lbl,0,0,1,1);
    cw = new s_tqChooseWidget(true);
    cw->Setup("2.15..All files (*_*)");
    cw->setObjectName("mdlfilecw");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SomethingChanged()));
    glyout->addWidget(cw,0,1,1,1);
    lbl = new s_tqLabel("Описание модели");
    glyout->addWidget(lbl,1,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdldescle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,1,1,1,1);
    lbl = new s_tqLabel("Имя модели");
    glyout->addWidget(lbl,2,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdlnamele");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,2,1,1,1);
    lbl = new s_tqLabel("Параметры модели");
    glyout->addWidget(lbl,3,0,1,1);
    le = new s_tqLineEdit;
    le->setObjectName("mdlparsle");
    connect(le,SIGNAL(textEdited(QString)),this,SLOT(SomethingChanged()));
    glyout->addWidget(le,3,1,1,1);
    gb->setLayout(glyout);
    lyout->addWidget(gb);
    lyout->addStretch(1);
    cp2->setLayout(lyout);
    SetUnitsAndPars();
}

// включаем сборщик поля PartNumber, если переданный производитель = "НКП"

void cmp_maindialog::EnablePartNumberCreator(QVariant Manufacturer)
{
    if (Manufacturer.toString() == "НКП")
    {
        if (!PartNumberCreatorEnabled)
        {
            SetPrefixesVisible(true);
            ConnectPartNumberCreatorLE("prefixle");
            QString tmps = (ChBData("issmdchb") == "1") ? "Чип" : "";
            if (tmps.isEmpty())
            {
                tmps += (CompTble == "capasitors") ? "Конд." : "";
                tmps += (CompTble == "resistors") ? "Рез." : "";
            }
            else
            {
                tmps += (CompTble == "capasitors") ? " конд." : "";
                tmps += (CompTble == "resistors") ? " рез." : "";
            }
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("prefixle");
            if (le == 0)
            {
                DBGMSG;
                return;
            }
            le->setText(tmps);
            for (int i=0; i<5; i++)
            {
                ConnectPartNumberCreatorLE("par"+QString::number(i)+"le");
                ConnectPartNumberCreatorCB("par"+QString::number(i)+"cb");
            }
            ConnectPartNumberCreatorLE("accuracyle");
            ConnectPartNumberCreatorLE("packagele");
            PartNumberCreatorEnabled = true;
        }
    }
    else
    {
        if (PartNumberCreatorEnabled)
        {
            SetPrefixesVisible(false);
            PartNumberCreatorEnabled = false;
            ClearHandles();
        }
    }
}

void cmp_maindialog::ConnectPartNumberCreatorLE(QString lename)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    QMetaObject::Connection *handle = new QMetaObject::Connection;
    *handle = connect(le, SIGNAL(textChanged(QString)), this, SLOT(PartNumberCreator()));
    handles.append(handle);
}

void cmp_maindialog::ConnectPartNumberCreatorCB(QString cbname)
{
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSG;
        return;
    }
    QMetaObject::Connection *handle = new QMetaObject::Connection;
    *handle = connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(PartNumberCreator()));
    handles.append(handle);
}

// отображение/скрытие полей префикса

void cmp_maindialog::SetPrefixesVisible(bool isVisible)
{
    s_tqLabel *lbl = this->findChild<s_tqLabel *>("prefixl");
    if (lbl == 0)
    {
        DBGMSG;
        return;
    }
    lbl->setVisible(isVisible);
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("prefixle");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    le->setVisible(isVisible);
}

void cmp_maindialog::ClearHandles()
{
    while (!handles.isEmpty())
    {
        QMetaObject::Connection *handle = handles.at(0);
        disconnect(*handle);
        handles.removeFirst();
    }
}

// формирователь поля PartNumber из других полей

void cmp_maindialog::PartNumberCreator()
{
    QString tmps2;
    QString tmps = LEData("prefixle") + " ";
    for (int i=0; i<5; i++)
    {
        tmps2 = LEData("par"+QString::number(i)+"le");
        if (!tmps2.isEmpty())
        {
            tmps += tmps2;
            tmps += " " + CBData("par"+QString::number(i)+"cb");
            tmps += ",";
        }
    }
    tmps2 = LEData("accuracyle");
    if (!tmps2.isEmpty())
        tmps += tmps2;
    tmps2 = LEData("packagele");
    if (!tmps2.isEmpty())
        tmps += ",тип " + tmps2;
    SetLEData("partnumber",tmps);
    return;
}

// установить для данного CompType требуемые наименования параметров в par<i>lbl и единицы измерения этих параметров в par<i>cb

void cmp_maindialog::SetUnitsAndPars()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("subsection");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    QString SubSection = le->text(); // взяли описание подраздела ("Конденсаторы")
    // теперь надо взять все par<i>name и par<i>unitset из ent.parameters, где parameters=SubSection
    QStringList fl;
    for (int i=1; i<6; i++)
        fl << "par"+QString::number(i)+"name" << "par"+QString::number(i)+"unitset";
    QStringList vl = sqlc.GetValuesFromTableByField("ent","parameters",fl,"parameters",SubSection);
    if (sqlc.result)
        return;
    // распихиваем взятые данные по соответствующим виджетам
    for (int i=0; i<5; i++)
    {
        if (i < vl.size())
        {
            if (!vl.at(i*2).isEmpty()) // если параметр есть
            {
                s_tqLabel *lbl = this->findChild<s_tqLabel *>("par"+QString::number(i)+"lbl");
                if (lbl == 0)
                    return;
                lbl->setText(vl.at(i*2));
                QStringListModel *cbmdl = new QStringListModel;
                QStringList cbfl;
                QString table = "Единицы измерения_сокращ";
                QString field = "Наименование";
                QString ida = "ИД_а";
                QString idavalue = vl.at(i*2+1);
                tfl.GetValuesByColumnAndField(table, field, ida, idavalue, cbfl);
                cbmdl->setStringList(cbfl);
                s_tqComboBox *cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"cb");
                if (cb == 0)
                    return;
                cb->setModel(cbmdl);
                cb->setCurrentIndex(0);
            }
            else
            {
                s_tqLabel *lbl = this->findChild<s_tqLabel *>("par"+QString::number(i)+"lbl");
                if (lbl != 0)
                    lbl->setVisible(false);
                s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("par"+QString::number(i)+"le");
                if (le != 0)
                    le->setVisible(false);
                s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("par"+QString::number(i)+"chb");
                if (chb != 0)
                    chb->setVisible(false);
                s_tqComboBox *cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"cb");
                if (cb != 0)
                    cb->setVisible(false);
            }
        }
    }
}

void cmp_maindialog::SetID()
{
    CompId = QString::number(sqlc.GetNextFreeIndexSimple(CompDb, CompTble)); // ищем первый свободный ИД
    if (sqlc.result)
    {
        WARNMSG("");
        return;
    }
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("id");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    le->setText(CompId);
}

void cmp_maindialog::FillAltDialog(QStringList vl)
{
    SetCWData("libref",vl.at(0));
    SetCWData("footref",vl.at(1));
    SetLEData("mdldescle",vl.at(2));
    SetCWData("mdlfilecw",vl.at(3));
    SetLEData("mdlnamele",vl.at(4));
    SetLEData("mdlparsle",vl.at(5));
    SetCWData("manufacturer",vl.at(6));
    SetLEData("partnumber",vl.at(7));
    SetLEData("packagele",vl.at(8));
    SetLEData("markingle",vl.at(9));
    SetParLE(0,vl.at(10));
    SetParLE(1,vl.at(11));
    SetLEData("accuracyle",vl.at(12));
    SetLEData("mintemple",vl.at(13));
    SetLEData("maxtemple",vl.at(14));
    SetLEData("maxpowerle",vl.at(15));
    SetLEData("tkcle",vl.at(16));
    SetParLE(2,vl.at(17));
    QString tmps = vl.at(18);
    tmps.replace("\\","/");
    SetCWData("dsheetcw",tmps);
    RevNotes = vl.at(19).toInt(); // RevNotes = 19
    RevNotes++; // на всякий случай прибавляем редакцию, вдруг обновим?
    SetChBData("isactivechb",(vl.at(20) == "0")?"1":"0"); // текстовая инверсия
    SetLEData("pe3name",vl.at(21));
    SetLEData("pe3notes",vl.at(22));
    QString ModifyDate = vl.at(23);
    QDateTime MDate = QDateTime::fromString(ModifyDate, "yyyy-MM-ddThh:mm:ss");
    ModifyDate = MDate.toString("dd-MM-yyyy hh:mm:ss");
    SetLEData("modifydatele",ModifyDate);
    QString Pers = sqlc.GetValueFromTableByField("sup","personel","personel","idpersonel",vl.at(24));
    SetLEData("creatorle",Pers);
    // Prefix = 25
    SetChBData("issmdchb",vl.at(26));
    // Nominal = 27
    // Unit = 28
    SetParLE(3,vl.at(29));
    SetParLE(4,vl.at(30));
    EnablePartNumberCreator(vl.at(6)); // принудительно вызываем включение формирователя поля PartNumber, если вдруг производитель оказался "НКП"
                                        // в конце - т.к. prefix формируется так же на основании isSMD
}

// запись в поле par<i>LE значения ParValue с разбиением на само значение и единицы измерения

void cmp_maindialog::SetParLE(int ParNum, QString ParValue)
{
    QStringList tmpsl = ParValue.split(" ");
    if (!tmpsl.isEmpty())
        SetLEData("par"+QString::number(ParNum)+"le", tmpsl.at(0)); // записали значение параметра
    if (tmpsl.size() > 1)
        SetCBData("par"+QString::number(ParNum)+"cb", tmpsl.at(1)); // записали значение в поле единиц измерения
}

QStringList cmp_maindialog::GetAltData()
{
    QStringList vl;
    vl.append(CWData("libref"));
    vl.append(CWData("footref"));
    vl.append(LEData("mdldescle"));
    vl.append(CWData("mdlfilecw"));
    vl.append(LEData("mdlnamele"));
    vl.append(LEData("mdlparsle"));
    vl.append(CWData("manufacturer"));
    vl.append(LEData("partnumber"));
    vl.append(LEData("packagele"));
    vl.append(LEData("markingle"));
    vl.append(ParLE(0));
    vl.append(ParLE(1));
    vl.append(LEData("accuracyle"));
    vl.append(LEData("mintemple"));
    vl.append(LEData("maxtemple"));
    vl.append(LEData("maxpowerle"));
    vl.append(LEData("tkcle"));
    vl.append(ParLE(2));
    QString tmps = CWData("dsheetcw");
    tmps.replace("/","\\\\");
    vl.append(tmps);
    vl.append(QString::number(RevNotes)); // RevNotes = 19
    vl.append((ChBData("isactivechb") == "0")?"1":"0"); // текстовая инверсия
    vl.append(LEData("pe3name"));
    vl.append(LEData("pe3notes"));
    vl.append(pc.DateTime);
    vl.append(QString::number(pc.idPers));
    vl.append(""); // Prefix = 25
    vl.append(ChBData("issmdchb"));
    vl.append(LEData("par0le")); // Nominal = 27
    QStringList sl;
    QString table = "Единицы измерения_полн";
    QStringList idf = QStringList("ИД");
    QString cmpfield = "Наименование";
    tfl.valuesbyfield(table,idf,cmpfield,CBData("par0le"), sl);
    if (sl.size() > 0)
        vl.append(sl.at(0)); // Unit = 28
    else
        vl.append("");
    vl.append(ParLE(3));
    vl.append(ParLE(4));
    return vl;
}

// формирование из par<i>LE и соответствующего поля в единицах измерения общей записи

QString cmp_maindialog::ParLE(int ParNum)
{
    QString tmps = LEData("par"+QString::number(ParNum)+"le"); // взяли значение из поля
    QString tmps2 = CBData("par"+QString::number(ParNum)+"cb"); // взяли значение из поля единиц измерения
    return tmps+" "+tmps2;
}

void cmp_maindialog::SetCWData(QString cwname, QVariant data)
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>(cwname);
    if (cw != 0)
    {
        PublicClass::ValueStruct vl;
        vl.Type = VS_STRING;
        vl.Value = data.toString();
        cw->SetData(vl);
    }
}

QString cmp_maindialog::CWData(QString cwname)
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>(cwname);
    if (cw != 0)
    {
        PublicClass::ValueStruct vl = cw->Data();
        return vl.Value;
    }
    else
        return QString();
}

void cmp_maindialog::SetLEData(QString lename, QVariant data)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>(lename);
    if (le != 0)
        le->setText(data.toString());
}

QString cmp_maindialog::LEData(QString lename)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>(lename);
    if (le != 0)
        return le->text();
    else
        return QString();
}

void cmp_maindialog::SetChBData(QString chbname, QVariant data)
{
    s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>(chbname);
    if (chb != 0)
        chb->setChecked(data.toBool());
}

QString cmp_maindialog::ChBData(QString chbname)
{
    s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>(chbname);
    if (chb != 0)
        return (chb->isChecked()) ? "1" : "0";
    else
        return QString();
}

void cmp_maindialog::SetCBData(QString cbname, QVariant data)
{
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>(cbname);
    if (cb != 0)
        cb->setCurrentText(data.toString());
}

QString cmp_maindialog::CBData(QString cbname)
{
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>(cbname);
    if (cb != 0)
        return (cb->currentText());
    else
        return QString();
}

void cmp_maindialog::AddManuf()
{
    QString newID, tmps;
    tmps = "Производители_полн";
    tfl.Insert(tmps, newID);
    s_2cdialog *newdialog = new s_2cdialog("Производители:добавить");
    newdialog->setup("Производители_полн",MODE_EDITNEW,newID);
    if (newdialog->result)
        WARNMSG("");
    else
        newdialog->exec();
}

void cmp_maindialog::CancelAndClose()
{
    if (Changed)
    {
        if (!(MessageBox2::question(this, "Выйти?", "Данные были изменены\nВсё равно выйти?")))
            return;
    }
    emit cancelled();
    this->close();
}

void cmp_maindialog::WriteAndClose()
{
    QStringList fl, vl;
    switch (CompType.toInt())
    {
    case CTYPE_ALT:
    {
        fl << "Library Ref" << "Footprint Ref" << "Sim Description" << "Sim File" << \
              "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
              "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
              "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
              "Nominal" << "Unit" << "par4" << "par5";
        vl = GetAltData();
        fl << "Library Path" << "Footprint Path";
        QString LPath = pc.PathToLibs + "/Symbols" + CompUrlPrefix + CompTble + ".SchLib";
        LPath.replace("/","\\\\");
        QString FPath = pc.PathToLibs + "/Footprints" + CompUrlPrefix + CompTble + ".PcbLib";
        FPath.replace("/","\\\\");
        vl << LPath << FPath;
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
    if (CompMode == CMPMODE_ED)
        sqlc.UpdateValuesInTable("alt",CompTble,fl,vl,"id",CompId);
    else
        sqlc.InsertValuesSimple("alt",CompTble,fl,vl);
    if (sqlc.result)
    {
        WARNMSG("");
        return;
    }
    MessageBox2::information(this, "Успешно", "Записано успешно!");
    this->close();
}

void cmp_maindialog::SomethingChanged()
{
    if (ChangeEnabled)
        Changed = true;
}
