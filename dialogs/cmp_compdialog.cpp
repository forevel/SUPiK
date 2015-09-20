#include "cmp_compdialog.h"
//#include <QStringListModel>
#include <QAction>
#include <QMenu>
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
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewItem()));
    pb->setToolTip("Создать новый компонент");
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
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
    CompTbles = sl.at(0);
    slavemodel->setupraw(CompDb,CompTbles,fl,"id"); // строим таблицу с сортировкой по ИД
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
        emit error(ER_COMP, 0x21);
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    if (CompTble == 0) // не была задана таблица компонентов (раздел)
    {
        QMessageBox::warning(this,"Предупреждение","Не выбран раздел в левой части");
        return;
    }
    StartCompDialog(CompIDs);
}

void cmp_compdialog::StartCompDialog(QString Id, bool ByExisting)
{
    cmp_maindialog *dlg = new cmp_maindialog;
    dlg->SetupUI(CompType,CompTble,Id.toInt());
    if (ByExisting)
        dlg->SetID();
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
    StartCompDialog(QString::number(CompID));
    // теперь добавим в перечень номенклатуры, если такового ещё нет
    CheckNkAndAdd(CompID);
}

void cmp_compdialog::AddNewOnExistingItem()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        emit error(ER_COMP, 0x32);
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    StartCompDialog(CompIDs,true); // создаём на базе компонента CompIDs компонент с новым индексом
    CheckNkAndAdd(CompIDs.toInt());
}

void cmp_compdialog::DeleteItem()
{
    if (QMessageBox::question(this, "Удалить элемент", "Вы уверены, что хотите удалить элемент?", QMessageBox::Yes|QMessageBox::No,\
                          QMessageBox::No) == QMessageBox::No)
        return;
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        emit error(ER_COMP, 0x21);
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    sqlc.deletefromdb(sqlc.getdb(CompDb),CompTbles,"id",CompIDs);
    if (sqlc.result)
    {
        emit error(ER_DIRMAIN+sqlc.result, 0x35);
        return;
    }
    else
    {
        QMessageBox::warning(this, "warning!", "Удалено успешно!");
        MainItemChoosed(QModelIndex());
    }
}

// проверка элемента на существование в БД номенклатуры и его добавление

void cmp_compdialog::CheckNkAndAdd(int id)
{
    QStringList sl = QStringList() << "" << "Элемент в БД altium" << "Элемент в БД solidworks" << "Элемент в БД schemagee" << "Элемент в БД конструктивов" << "Элемент в БД устройств";
    QString ElementString = sl.at(CompType);
    // сначала берём наименование и производителя из соответствующей БД компонентов
    QStringList fl = QStringList() << "PartNumber" << "Manufacturer";
    QStringList vl = sqlc.getvaluesfromtablebyid(sqlc.getdb(CompDb),CompTbles,fl,QString::number(id));
    if (sqlc.result) // нет такого элемента или ошибка в БД
    {
        emit error(ER_COMP, 0x41);
        return;
    }
    // теперь надо взять индекс производителя
    QStringList tmpfl = QStringList() << "ИД";
    QStringList cmpvl;
    if (vl.size()>1)
        cmpvl = tfl.valuesbyfield("Производители_сокращ",tmpfl,"Наименование",vl.at(1)); // в cmpvl один нулевой элемент - индекс производителя
    else
    {
        emit error (ER_COMP, 0x42);
        return;
    }
    if (tfl.result)
    {
        emit error (ER_COMP, 0x43);
        return;
    }
    // теперь ищем в БД номенклатуры такой уже элемент
    fl << QStringList() << "ИД" << ElementString;
    tmpfl = QStringList() << "Наименование" << "Производитель";
    cmpvl.insert(0, vl.at(0)); // вставка перед производителем наименования компонента
    QStringList nkidsl = tfl.valuesbyfields("Номенклатура_полн",fl,tmpfl,cmpvl);
    if (tfl.result) // нет такого или ошибка
    {
        // создаём новый элемент в БД номенклатуры
        tfl.insert("Номенклатура_полн");
        // найдём ИД в таблице категорий
        QStringList letsl = QStringList() << "" << "А" << "З" << "Э" << "К" << "У";
        fl = QStringList() << "Описание";
        QStringList tblesl = tfl.valuesbyfield(letsl.at(CompType)+"Компоненты_описание_сокращ",fl,"ИД",QString::number(CompTable));

        fl = QStringList() << "Наименование" << "Категория" << "Производитель" << ElementString;
        vl.insert(1,); // vl уже содержит PartNumber (0) и Manufacturer (1)
    }
    else
    {
        // проверяем, есть ли у данного элемента в БД ссылка на данный компонент (может, уже было создано ранее для другого раздела)
        if (nkidsl.at(1).isEmpty())
        {
            // нет ссылки, записываем её методом обновления
        }
        else
        {
            if (nkidsl.at(1) == QString::number(id))
            {
                // есть уже точно такая же запись, ничего не делаем и выходим
                return;
            }
            else
            {
                if (QMessageBox::question(this, "Запись найдена", "В БД номенклатуры есть такой элемент,\nно с другой ссылкой ("+nkidsl.at(1)+"). Перезаписать?", \
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
                    return;

            }
        }
    }
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

void cmp_compdialog::emiterror(int er1, int er2)
{
    er1 += ER_COMP;
    emit error(er1,er2);
}
