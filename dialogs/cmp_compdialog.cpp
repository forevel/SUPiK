#include "cmp_compdialog.h"
//#include <QStringListModel>
#include <QAction>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QSortFilterProxyModel>
#include "../models/s_duniversal.h"
#include "../widgets/s_tqgroupbox.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqcheckbox.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_colortabwidget.h"
#include "../widgets/s_tqwidget.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "cmp_maindialog.h"

// --------------------------------------
// Конструктор
// --------------------------------------

cmp_compdialog::cmp_compdialog(int type, QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QStringList sl = QStringList() << "" << "А" << "З" << "Э" << "К" << "У";
    QStringList dbsl = QStringList() << "" << "alt" << "sch" << "sol" << "con" << "dev";
    if (type > sl.size())
        return;
    CompDb = dbsl.at(type); // БД компонентов
    CompLetter = sl.at(type); // буква типа компонентов
    CompType = type;
    SomethingChanged = false;
    CompTble = 0;

    SetupUI();
}

// --------------------------------------
// Деструктор
// --------------------------------------

cmp_compdialog::~cmp_compdialog()
{
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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewItem()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    hlyout->addWidget(pb);
    hlyout->addSpacing(20);
    pb = new s_tqPushButton;

    hlyout->addStretch(300);
    lyout->addLayout(hlyout);
    s_tqLabel *lbl = new s_tqLabel("Компоненты");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);
    s_tqTableView *MainTV = new s_tqTableView;
    s_duniversal *gridItemDelegate = new s_duniversal;
    connect(gridItemDelegate,SIGNAL(error(int,int)),this,SLOT(emiterror(int,int)));
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    MainTV->horizontalHeader()->setVisible(true);
    MainTV->verticalHeader()->setVisible(false);
    s_ncmodel *mainmodel = new s_ncmodel;
    QSortFilterProxyModel *mainproxymodel = new QSortFilterProxyModel;
    mainmodel->setup(CompLetter+"Компоненты_описание_сокращ");
    if (mainmodel->result)
    {
        emit error(ER_COMP+mainmodel->result,0x01);
        QApplication::restoreOverrideCursor();
        return;
    }
    mainproxymodel->setSourceModel(mainmodel);
    MainTV->setModel(mainproxymodel);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(0, Qt::AscendingOrder);
    connect(MainTV,SIGNAL(clicked(QModelIndex)),this,SLOT(MainItemChoosed(QModelIndex)));
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;
    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    s_tqTableView *SlaveTV = new s_tqTableView;
    SlaveTV->setItemDelegate(gridItemDelegate);
    slavemodel = new s_ncmodel;
    QSortFilterProxyModel *slaveproxymodel = new QSortFilterProxyModel;
    slaveproxymodel->setSourceModel(slavemodel);
    SlaveTV->setModel(slaveproxymodel);
    SlaveTV->setObjectName("stv");
    SlaveTV->horizontalHeader()->setVisible(true);
    SlaveTV->verticalHeader()->setVisible(false);
    SlaveTV->setSelectionMode(QAbstractItemView::NoSelection);
    SlaveTV->setSortingEnabled(true);
    SlaveTV->sortByColumn(0, Qt::AscendingOrder);
    connect(SlaveTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SlaveItemChoosed(QModelIndex)));
    rlyout->addWidget(SlaveTV);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    right->setLayout(rlyout);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}

void cmp_compdialog::MainItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mtv");
    if (tv == 0)
    {
        emit error(ER_COMP, 0x11);
        return;
    }
    QString tmps = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    QStringList fl = QStringList() << "Наименование";
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
    slavemodel->setupraw(CompDb,sl.at(0),fl,"id"); // строим таблицу с сортировкой по ИД
    if (slavemodel->result)
        emit error(ER_COMP+slavemodel->result, 0x13);
    tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        emit error(ER_COMP, 0x14);
        return;
    }
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

void cmp_compdialog::test()
{
    emit error (0,0);
}

void cmp_compdialog::SlaveItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        emit error(ER_COMP, 0x21);
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    int CompID = CompIDs.toInt();
    if (CompTble == 0) // не была задана таблица компонентов (раздел)
    {
        QMessageBox::warning(this,"Предупреждение","Не выбран раздел в левой части");
        return;
    }
    cmp_maindialog *dlg = new cmp_maindialog;
    dlg->SetupUI(CompType,CompTble,CompID);
    connect(dlg,SIGNAL(error(int,int)),this,SLOT(emiterror(int,int)));
    dlg->exec();
}

// --------------------------------------
// Добавляем новый элемент
// --------------------------------------

void cmp_compdialog::AddNewItem()
{
    // В CompDb содержится выбранная БД, соответствующая типу компонентов
    if ((CompDb == 0) || (CompTble == 0))
    {
        QMessageBox::warning(this,"Предупреждение","Не выбран раздел в левой части");
        return;
    }
    QStringList fl = QStringList() << "Наименование";
    QString tmps = QString::number(CompTble); // убираем старшие незначащие нули
    QStringList sl = tfl.valuesbyfield(CompLetter+"Компоненты_описание_полн",fl,"ИД",tmps); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
    if (tfl.result)
    {
        emit error(ER_COMP+tfl.result, 0x31);
        return;
    }
    int CompID = sqlc.getnextfreeindexsimple(sqlc.getdb(CompDb), sl.at(0)); // ищем первый свободный ИД
    if (sqlc.result)
    {
        emit error(ER_COMP+sqlc.result, 0x32);
        return;
    }
    cmp_maindialog *dlg = new cmp_maindialog;
    dlg->SetupUI(CompType,CompTble,CompID);
    connect(dlg,SIGNAL(error(int,int)),this,SLOT(emiterror(int,int)));
    dlg->exec();
}

// --------------------------------------
// Заполняем элементы диалогового окна (комбо-боксы) данными из БД
// --------------------------------------

void cmp_compdialog::FillDialog (QString PartNumber)
{
/*    QSqlQuery get_data_from_db(pc.alt);

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

    SomethingChanged = false;*/
}

// --------------------------------------
// Очистка диалога
// --------------------------------------

void cmp_compdialog::ClearDialog ()
{
/*    LibRefCB->setCurrentIndex(-1);
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
    Par3LE->setText("");*/
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
/*    if (SomethingChanged && (Action == UPDATE))
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
        this->close();*/
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
/*    QString tmpString;
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
/*                + pc.CompModelData.MDescLE + "\",`Sim File`=\"" \
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

/*                qa_AddNkDialog.ManufCB->setCurrentText(ManufCB->currentText());
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
  /*          }
        }
        return true;
    }
    else
    {
        QMessageBox::warning(this,"Warning!",\
                             insert_comp.lastError().text(),\
                             QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }*/
    return true;
}


// --------------------------------------
// Захотели выбрать файл описания
// --------------------------------------

void cmp_compdialog::DSheetPBClicked()
{
/*    QString DSheedbs = QFileDialog::getOpenFileName(this, \
                                      "Выберите файл описания", \
                                      pc.PathToLibs + "//DSheet//",
                                      "PDF Files (*.pdf)");
    DSheetLE->setText(DSheedbs);*/
}

void cmp_compdialog::AddManufPBClicked()
{
//    qa_AddManufDialog.AddNewItem();
//    qa_AddManufDialog.exec();
    UpdateManufCombobox();
}

void cmp_compdialog::UpdateManufCombobox ()
{
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

void cmp_compdialog::ManufCBIndexChanged(const QString &arg1)
{
/*    if (arg1 == "НКП")
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
    }*/
}

void cmp_compdialog::SetParNames()
{
/*    QSqlQuery get_parnames(pc.sup);*/

}

bool cmp_compdialog::ConnectPartNumber()
{
/*    handle1 = connect(PrefixLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle2 = connect(Par1LE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle3 = connect(AccuracyLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle4 = connect(TypeLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle5 = connect(MaxPowerLE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle6 = connect(Par2LE, SIGNAL(textChanged(QString)), this, SLOT(UpdatePartNumber()));
    handle7 = connect(UnitsCB, SIGNAL(currentTextChanged(QString)) , this, SLOT(UpdatePartNumber()));
    bool result = handle1 && handle2 && handle3 && handle4 && handle5 && handle6 && handle7;
    return result;*/
    return true;
}

void cmp_compdialog::UpdatePartNumber()
{
/*    QString tmpString;
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
    PEDescLE->setText(tmpString);*/
}

bool cmp_compdialog::DisconnectPartNumber()
{
/*    if (handle1)
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
    return true;*/
    return true;
}

void cmp_compdialog::DeletePBClicked()
{
/*    QString tmpString;
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
    this->close();*/
}

void cmp_compdialog::VoltageOrAccuracyAccIsChecked()
{
 /*   if (ManufCB->currentText() == "НКП")
        UpdatePartNumber();*/
}

void cmp_compdialog::ModelParPBClicked()
{
//    qa_EditCompModelDialog.FillDialog();
//    qa_EditCompModelDialog.exec();
}

void cmp_compdialog::emiterror(int er1, int er2)
{
    er1 += ER_COMP;
    emit error(er1,er2);
}
