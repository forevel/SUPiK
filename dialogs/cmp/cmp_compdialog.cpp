#include "cmp_compdialog.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "../../models/proxymodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_colortabwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"
#include "cmp_maindialog.h"
#include "cmp_newsubsection.h"

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
    CompTbles = "";
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
    pb->setIcon(QIcon(":/res/newcaty.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewSubsection()));
    pb->setToolTip("Создать новый раздел");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewItem()));
    pb->setToolTip("Создать новый компонент");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
//    lyout->addLayout(hlyout);
    s_tqLabel *lbl = new s_tqLabel("Компоненты");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);

    lyout->addLayout(hlyout);
    s_tqTableView *MainTV = new s_tqTableView;
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    MainTV->horizontalHeader()->setVisible(true);
    MainTV->verticalHeader()->setVisible(false);
    s_ncmodel *mainmodel = new s_ncmodel;
    ProxyModel *mainproxymodel = new ProxyModel;
    mainmodel->setup(CompLetter+"Компоненты_описание_сокращ");
    if (mainmodel->result)
    {
        WARNMSG("");
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
    ProxyModel *slaveproxymodel = new ProxyModel;
    slaveproxymodel->setSourceModel(slavemodel);
    SlaveTV->setModel(slaveproxymodel);
    SlaveTV->setObjectName("stv");
    SlaveTV->horizontalHeader()->setVisible(true);
    SlaveTV->verticalHeader()->setVisible(false);
    SlaveTV->setSelectionMode(QAbstractItemView::NoSelection);
    SlaveTV->setSortingEnabled(true);
    SlaveTV->sortByColumn(0, Qt::AscendingOrder);
    connect(SlaveTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SlaveItemChoosed(QModelIndex)));
    SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(SlaveTV,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(SlaveContextMenu(QPoint)));
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
        DBGMSG;
        return;
    }
    QString tmps = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    QStringList fl = QStringList() << "Наименование";
    CompTble = tmps.toInt();
    tmps = QString::number(CompTble); // убираем старшие незначащие нули
    QStringList sl;
    QString table = CompLetter+"Компоненты_описание_полн";
    QString field = "ИД";
    tfl.valuesbyfield(table,fl,field,tmps, sl); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
    if (tfl.result != TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    // теперь надо вытащить в slavemodel все компоненты из выбранной таблицы
    fl = QStringList() << "id" << "PartNumber" << "Manufacturer";
    CompTbles = sl.at(0);
    slavemodel->setupraw(CompDb,CompTbles,fl,"id"); // строим таблицу с сортировкой по ИД
    if (slavemodel->result)
        MessageBox2::information(this, "Внимание", "Проблемы при построении таблицы "+CompTbles);
    tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

void cmp_compdialog::EditItem()
{
    SlaveItemChoosed(QModelIndex());
}

void cmp_compdialog::SlaveItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    if (CompTble == 0) // не была задана таблица компонентов (раздел)
    {
        MessageBox2::information(this, "Внимание", "Не выбран раздел в левой части");
        return;
    }
    StartCompDialog(CompIDs, CMPMODE_ED);
    CheckNkAndAdd(CompIDs.toInt());
}

void cmp_compdialog::StartCompDialog(QString Id, int Mode, bool ByExisting)
{
    cmp_maindialog *dlg = new cmp_maindialog(Mode);
    dlg->SetupUI(CompType,CompTble,Id.toInt());
    if (ByExisting)
        dlg->SetID();
    connect(dlg,SIGNAL(cancelled()),this,SLOT(EditCancelled()));
    dlg->exec();
}

void cmp_compdialog::EditCancelled()
{
    Cancelled = true;
}

// --------------------------------------
// Добавляем новый элемент
// --------------------------------------

void cmp_compdialog::AddNewItem()
{
    // В CompDb содержится выбранная БД, соответствующая типу компонентов
    if ((CompDb == 0) || (CompTble == 0))
    {
        MessageBox2::information(this, "Внимание", "Не выбран раздел в левой части");
        return;
    }
    QStringList fl = QStringList() << "Наименование";
    QString tmps = QString::number(CompTble); // убираем старшие незначащие нули
    QStringList sl;
    QString table = CompLetter+"Компоненты_описание_полн";
    QString field = "ИД";
    tfl.valuesbyfield(table,fl,field,tmps, sl); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    int CompID = sqlc.GetNextFreeIndexSimple(CompDb, sl.at(0)); // ищем первый свободный ИД
    if (sqlc.result)
    {
        WARNMSG("");
        return;
    }
    Cancelled = false;
    StartCompDialog(QString::number(CompID), CMPMODE_NEW);
    // теперь добавим в перечень номенклатуры, если такового ещё нет
    if (!Cancelled)
        CheckNkAndAdd(CompID);
    MainItemChoosed(QModelIndex());
}

void cmp_compdialog::AddNewOnExistingItem()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    Cancelled = false;
    StartCompDialog(CompIDs,CMPMODE_EX,true); // создаём на базе компонента CompIDs компонент с новым индексом
    if (!Cancelled)
        CheckNkAndAdd(CompIDs.toInt());
    MainItemChoosed(QModelIndex());
}

void cmp_compdialog::DeleteItem()
{
    if (!(MessageBox2::question(this, "Удалить элемент", "Вы уверены, что хотите удалить элемент?")))
        return;
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    sqlc.DeleteFromDB(CompDb,CompTbles,"id",CompIDs);
    if (sqlc.result)
    {
        WARNMSG("");
        return;
    }
    else
    {
        MessageBox2::information(this, "Успешно", "Удалено успешно!");
        MainItemChoosed(QModelIndex());
    }
}

// проверка элемента на существование в БД номенклатуры и его добавление

void cmp_compdialog::CheckNkAndAdd(int id)
{
    QStringList ElementSl = QStringList() << "" << "Элемент в БД altium" << "Элемент в БД solidworks" << "Элемент в БД schemagee" << "Элемент в БД конструктивов" << "Элемент в БД устройств";
    QString ElementString = ElementSl.at(CompType);
    // сначала берём наименование и производителя из соответствующей БД компонентов
    QStringList fl = QStringList() << "PartNumber" << "Manufacturer";
    QStringList vl = sqlc.GetValuesFromTableByIDSimple(CompDb,CompTbles,fl,QString::number(id));
    if (sqlc.result) // нет такого элемента или ошибка в БД
    {
        WARNMSG("");
        return;
    }
    // теперь надо взять индекс производителя
    fl = QStringList() << "ИД";
    QStringList cmpvl;
    QString table = "Производители_сокращ";
    QString field = "Наименование";
    QString value;
    if (vl.size()>1)
    {
        value = vl.at(1);
        tfl.valuesbyfield(table,fl,field,value, cmpvl); // в cmpvl один нулевой элемент - индекс производителя
    }
    else
    {
        WARNMSG("");
        return;
    }
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    // теперь ищем в БД номенклатуры такой уже элемент
    fl = QStringList() << "ИД" << ElementString;
    QStringList cmpfl = QStringList() << "Наименование" << "Производитель";
    cmpvl.insert(0, vl.at(0)); // вставка перед производителем наименования компонента
    table = "Номенклатура_полн";
    QStringList nkidsl;
    tfl.valuesbyfields(table,fl,cmpfl,cmpvl, nkidsl);
    QString tmps;
    QString ManufId = cmpvl.at(1);
    if ((tfl.result == TFRESULT_ERROR) || (nkidsl.isEmpty())) // нет такого или ошибка
    {
        table = "Номенклатура карантин_полн";
        tfl.valuesbyfields(table,fl,cmpfl,cmpvl, nkidsl);
        if (nkidsl.isEmpty())
        {
            // создаём новый элемент в БД карантинной номенклатуры
            QString newID;
            table = "Номенклатура карантин_полн";
            tfl.insert(table, newID);
            // найдём сначала описание категории в таблице описания для данной БД
            fl = QStringList() << "Описание";
            tmps = QString::number(CompTble); // убираем старшие незначащие нули
            QStringList sl;
            table = CompLetter+"Компоненты_описание_полн";
            field = "ИД";
            tfl.valuesbyfield(table,fl,field,tmps, sl); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("");
                return;
            }
            // теперь возьмём ИД в таблице категорий
            fl = QStringList() << "ИД";
            if (!sl.isEmpty())
                tmps = sl.at(0);
            else
            {
                WARNMSG("");
                return;
            }
            table = "Категории_сокращ";
            field = "Наименование";
            tfl.valuesbyfield(table,fl,field,tmps, sl);
            if ((tfl.result == TFRESULT_ERROR) || (sl.isEmpty()))
            {
                WARNMSG("");
                return;
            }
            fl = QStringList() << "ИД" << "Наименование" << "Категория" << "Производитель" << ElementString;
            vl.insert(1,sl.at(0)); // vl уже содержит PartNumber (0) и Manufacturer (1), вставляем ИД категории
            vl.append(QString::number(id)); // добавляем ИД компонента по его БД
            vl.insert(0, newID);
            vl.replace(3, ManufId); // меняем наименование производителя на его ИД
            table = "Номенклатура карантин_полн";
            tfl.idtois(table, fl, vl);
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("");
                return;
            }
        }
    }
    else
    {
        if (nkidsl.size()<2)
        {
            WARNMSG("");
            return;
        }
        // проверяем, есть ли у данного элемента в БД ссылка на данный компонент (может, уже было создано ранее для другого раздела)
        if (!nkidsl.at(1).isEmpty()) // ссылка есть
        {
            if (nkidsl.at(1) == QString::number(id)) // проверяем, ссылка на тот же компонент?
                // есть уже точно такая же запись, ничего не делаем и выходим
                return;
            else if (!(MessageBox2::question(this, "Запись найдена", "В БД номенклатуры есть такой элемент,\nно с другой ссылкой ("+nkidsl.at(1)+"). Перезаписать?")))
                return;
        }
        // нет ссылки или она неправильная, записываем её методом обновления
        ElementSl.removeFirst(); // убираем первый пустой элемент, т.к. он был создан только для корректных индексов и уже не нужен
        // 1. Взять все значения, где ИД = nkidsl.at(0)
        QStringList sl = QStringList() << "Наименование" << "Категория" << "Производитель" << ElementSl;
        QStringList vl;
        table = "Номенклатура_полн";
        field = "ИД";
        value = nkidsl.at(0);
        tfl.valuesbyfield(table,sl,field,value,vl);
        if ((tfl.result == TFRESULT_ERROR) || (vl.size()<8))
        {
            WARNMSG("");
            return;
        }
        // 2. Обновить значение ElementString до QString::number(id)
        vl.replace(sl.indexOf(ElementString), QString::number(id));
        // 3. Отправить все значения обратно в таблицу
        sl.insert(0, "ИД");
        vl.insert(0, nkidsl.at(0));
        tfl.idtois(table,sl,vl);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
            return;
        }
    }
}

// добавление нового раздела

void cmp_compdialog::AddNewSubsection()
{
    cmp_newsubsection *dlg = new cmp_newsubsection(CompType);
    dlg->exec();
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


void cmp_compdialog::SlaveContextMenu(QPoint)
{
    QAction *ChangeDataChild;
    ChangeDataChild = new QAction ("Изменить элемент", this);
    ChangeDataChild->setSeparator(false);
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(EditItem()));
    QAction *AddNewByExisting = new QAction ("Создать на основе...", this);
    AddNewByExisting->setSeparator(false);
    connect(AddNewByExisting, SIGNAL(triggered()), this, SLOT(AddNewOnExistingItem()));
    QAction *DeleteAction = new QAction("Удалить элемент", this);
    DeleteAction->setSeparator(false);
    connect (DeleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    if (pc.access & 0x2492) // права на изменение
    {
        ContextMenu->addAction(AddNewByExisting);
        ContextMenu->addAction(ChangeDataChild);
    }
    if (pc.access & 0x4924) // права на удаление
        ContextMenu->addAction(DeleteAction);
    ContextMenu->exec(QCursor::pos()); // если есть права на удаление, на изменение тоже должны быть
}
