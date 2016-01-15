#include "dir_maindialog.h"
#include "dir_adddialog.h"
#include "../s_2cdialog.h"
#include "../../gen/s_sql.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../models/s_duniversal.h"
#include "../../models/griddelegate.h"
#include "../../models/s_ntmodel.h"
#include "../../models/s_ncmodel.h"

#include <QApplication>
#include <QPainter>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSortFilterProxyModel>

dir_maindialog::dir_maindialog(QString tble, QWidget *parent) :
    QDialog(parent)
{
    this->tble = tble;
    setAttribute(Qt::WA_DeleteOnClose);
    firstShow = true;
    MainTVIsTree = false;
    SlaveTVIsTree = false;
    IsQuarantine = false;
    twodb = false;
    isNewID = false;
    SetupUI();
}

void dir_maindialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/dirPB-grn.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddDirDialog()));
    pb->setToolTip("Создать новый справочник");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
/*    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/dirPB-red.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(DeleteData()));
    pb->setToolTip("Удалить справочник");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5); */
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel(tble);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    TreeView *SlaveTV = new TreeView;
    s_tqTableView *MainTV = new s_tqTableView;
    MainTableModel = new s_ncmodel;
    SlaveTreeModel = new TreeModel;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
    SlaveTV->setModel(SlaveTreeModel);
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    MainTableModel->setup(tble+"_сокращ");
    if (MainTableModel->result)
    {
        DBGMSG(PublicClass::ER_DIRMAIN,__LINE__,"Ошибка при построении таблицы "+tble);
        QApplication::restoreOverrideCursor();
        return;
    }
    MainTV->setModel(MainTableModel);
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    GridDelegate *MainDelegate = new GridDelegate;
    MainTV->setItemDelegate(gridItemDelegate);
    SlaveTV->setItemDelegate(MainDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(mainContextMenu(QPoint)));
    connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));
    connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
    connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditItem()));

    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    rlyout->addWidget(SlaveTV);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
}

void dir_maindialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/directory.jpg"));

    e->accept();
}

void dir_maindialog::showDirDialog(QModelIndex idx)
{
    Q_UNUSED(idx);
    QString tmpString = getMainIndex(1);
    if (!tmpString.isEmpty())
        ShowSlaveTree(tmpString);
}

// ############################################ SLOTS ####################################################

// отображение подчинённого дерева в правой части

void dir_maindialog::ShowSlaveTree(QString str)
{
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        DBGMSG(PublicClass::ER_DIRMAIN,__LINE__);
        return;
    }
    QStringList fields, values;
    int res;
    SlaveTable = str; // для функций удаления и добавления необходимо знать имя текущей таблицы
    if (str.contains("карантин", Qt::CaseInsensitive))
        IsQuarantine = true;
    else
        IsQuarantine = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    fields << "Наименование" << "Родительский справочник" << "Права доступа";
    values = tfl.valuesbyfield(tble+"_полн",fields,"Наименование",str);
    if (!tfl.result)
    {
        if (values.at(2).toUInt(0,16) & pc.access)
        {
            Tables.clear();
            QStringList sl;
            if (!values.at(1).isEmpty()) // есть родительские категории у справочника
            {
                Tables << values.at(1) << values.at(0);
                sl << values.at(1)+"_сокращ" << values.at(0)+"_сокращ";
            }
            else
            {
                Tables << values.at(0);
                sl << values.at(0) + "_сокращ";
            }
            res = SlaveTreeModel->Setup(sl);
            SlaveTV->resizeColumnsToContents();
            SlaveTVAccess = values.at(2).toLongLong(0, 16);
        }
        else
            DIRMER("Недостаточно прав для работы со справочником!");
    }
    else
        DIRMWARN;
    SlaveTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

QString dir_maindialog::getMainIndex(int column)
{
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        DIRMDBG;
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

QString dir_maindialog::getSlaveIndex(int column)
{
    QString tmpString;
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        DIRMDBG;
        return QString();
    }
    tmpString = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    return tmpString;
}

void dir_maindialog::EditItem()
{
    QString tmpString = getSlaveIndex(0);
    if (!tmpString.isEmpty())
        EditItem(tmpString);
    else
        DIRMWARN;
}

void dir_maindialog::EditItem(QString str)
{
    QString tmps = getMainIndex(1);
    if (tmps.isEmpty())
    {
        DIRMWARN;
        return;
    }
    QStringList sl = str.split(".");
    QString Table, Id;
    if (sl.size()>1)
    {
        Table = Tables.at(sl.at(0).toInt());
        Id = QString::number(sl.at(1).toInt());
    }
    else
    {
        Table = Tables.at(0);
        Id = QString::number(str.toInt());
    }
    s_2cdialog *newdialog = new s_2cdialog(tmps+":"+Id);
    int Mode = (isNewID) ? MODE_EDITNEW : MODE_EDIT;
    newdialog->setup(Table+"_полн",Mode,Id);
    if (newdialog->result)
        DIRMWARN;
    else
        newdialog->exec();
    ShowSlaveTree(SlaveTable);
}

void dir_maindialog::AddNew()
{
    isNewID = true;
    QString newID = tfl.insert(Tables.last()+"_полн"); // добавление элементов разрешается только в крайнюю таблицу
    QString tmpString = getSlaveIndex(0);
    QStringList fl, vl;
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        DIRMDBG;
        return QString();
    }
    TreeModel *mdl = static_cast<TreeModel *>(SlaveTV->model());
    switch (mdl->TreeType)
    {
    case TreeModel::TT_SIMPLE: // простое дерево
        fl << "ИД" << "ИД_а";
        vl << newID << tmpString;
        break;
    default: // TT_TYPE1, TT_TYPE2, TT_TABLE // простая таблица или деревья из таблиц
        fl << "ИД";
        vl << newID;
        break;
    }
    tfl.idtois(Tables.last()+"_полн",fl,vl);

    if ((!tmpString.isEmpty()) && (SlaveTVIsTree) && (!twodb)) // если из двух таблиц, то создавать элемент надо в подчиннённой таблице
    {
        fl << "ИД" << "ИД_а";
        vl << newID << tmpString;
    }
    else if (twodb)
    {
        QString ParentID = getSlaveIndex(0); // берём ИД "родительского" элемента
        QString SlaveHeader;
        QSqlQuery GetTableFields(sqlc.GetDB("sup"));
        GetTableFields.exec("SELECT `header` FROM `tablefields` WHERE `tablename`=\"" + SlaveTable + "_полн\" AND `links` LIKE \"%" \
                            + SlaveParentTableName + "%\";");
        if (GetTableFields.isActive())
        {
            GetTableFields.next();
            SlaveHeader = GetTableFields.value(0).toString();
            if (!SlaveHeader.isEmpty())
            {
                fl = QStringList() << "ИД" << SlaveHeader;
                vl = QStringList() << newID << ParentID;
                tfl.idtois(SlaveTable+"_полн", fl, vl);
                if (tfl.result)
                {
                    DIRMWARN;
                    return;
                }
            }
            else
            {
                DIRMWARN;
                return;
            }
        }
        else
        {
            DIRMWARN;
            return;
        }
    }
    EditItem(newID);
    isNewID = false;
}

void dir_maindialog::DeleteData()
{
    QMessageBox msgBox;
    msgBox.setText("Вы уверены, что хотите удалить элемент?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int res = msgBox.exec();
    if (res == QMessageBox::Cancel)
        return;
    DeleteDataUnconditional(QString::number(getSlaveIndex(0).toLongLong(0,10)));
}

void dir_maindialog::DeleteDataUnconditional(QString id)
{
    tfl.remove(SlaveTable+"_полн", id);
    if (tfl.result)
    {
        DIRMWARN;
        return;
    }
    else
    {
        DIRMINFO("Удалено успешно!");
        showDirDialog();
    }
}

void dir_maindialog::DeleteDir()
{
    QMessageBox msgBox;
    msgBox.setText("Вы уверены, что хотите удалить элемент?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int res = msgBox.exec();
    if (res == QMessageBox::Cancel)
        return;
    DeleteDataUnconditional(QString::number(getSlaveIndex(0).toLongLong(0,10)));
}

void dir_maindialog::AddDirDialog()
{
    dir_adddialog *AddDialog = new dir_adddialog(false, tble); // no update. В tble передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
    AddDialog->exec();
}

void dir_maindialog::EditDirDialog()
{
    QString tmpString = getMainIndex(1);
    dir_adddialog *EditDialog = new dir_adddialog(true, tble, tmpString+"_полн"); // В tble передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
    EditDialog->exec();
}

// ########################################## CONTEXTS ###################################################

// контекстное меню основного дерева

void dir_maindialog::mainContextMenu(QPoint)
{
    QAction *OpenAction = new QAction("Открыть", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(showDirDialog()));
    QAction *EditAction = new QAction("Редактировать справочник", this);
    connect (EditAction, SIGNAL(triggered()), this, SLOT(EditDirDialog()));
    QAction *AddAction = new QAction("Добавить справочник", this);
    connect (AddAction, SIGNAL(triggered()), this, SLOT(AddDirDialog()));

    QMenu *SystemContextMenu = new QMenu;
    SystemContextMenu->addAction(OpenAction);
    if (pc.access & 0x0002)
    {
        SystemContextMenu->addAction(EditAction);
        SystemContextMenu->addAction(AddAction);
    }
    SystemContextMenu->exec(QCursor::pos());
}

// контекстное меню дочернего дерева

void dir_maindialog::SystemSlaveContextMenu(QPoint)
{
    QAction *ChangeDataChild;
    ChangeDataChild = new QAction ("Изменить элемент", this);
    ChangeDataChild->setSeparator(false);
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(EditItem()));
    QAction *AddData = new QAction ("Добавить элемент", this);
    AddData->setSeparator(false);
    connect(AddData, SIGNAL(triggered()), this, SLOT(AddNew()));
    QAction *DeleteAction = new QAction("Удалить элемент", this);
    DeleteAction->setSeparator(false);
    connect (DeleteAction, SIGNAL(triggered()), this, SLOT(DeleteData()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    if (SlaveTVAccess & pc.access & 0x2492) // права на изменение
    {
        ContextMenu->addAction(AddData);
        ContextMenu->addAction(ChangeDataChild);
    }
    if (SlaveTVAccess & pc.access & 0x4924) // права на удаление
        ContextMenu->addAction(DeleteAction);
    ContextMenu->exec(QCursor::pos()); // если есть права на удаление, на изменение тоже должны быть
}
