#include "dir_maindialog.h"
#include "dir_adddialog.h"
#include "../gen/twocoldialog.h"
#include "../../gen/s_sql.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../models/griddelegate.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../gen/messagebox.h"

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
    MainTable = tble;
    setAttribute(Qt::WA_DeleteOnClose);
    IsQuarantine = false;
    isNewID = false;
    SetupUI();
}

void dir_maindialog::SetupUI()
{
    QVBoxLayout *mlyout = new QVBoxLayout;
    s_tqStackedWidget *cw = new s_tqStackedWidget;
    cw->setObjectName("cw");
    s_tqWidget *wdgt = new s_tqWidget;
//    wdgt->setAttribute(Qt::WA_DeleteOnClose,true);
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
    s_tqLabel *lbl = new s_tqLabel(MainTable);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    TreeView *SlaveTV = new TreeView;
    TreeView *MainTV = new TreeView;
    MainTableModel = new TreeModel;
    SlaveTreeModel = new TreeModel;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
    SlaveTV->setModel(SlaveTreeModel);
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (MainTableModel->Setup(MainTable+"_сокращ", true))
    {
        ERMSG("Ошибка при построении таблицы "+MainTable);
        QApplication::restoreOverrideCursor();
        return;
    }
    MainTV->setModel(MainTableModel);
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    GridDelegate *MainDelegate = new GridDelegate;
    MainTV->setItemDelegate(MainDelegate);
    SlaveTV->setItemDelegate(MainDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(mainContextMenu(QPoint)));
    connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(ShowSlave(QModelIndex)));
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
    wdgt->setLayout(lyout);
    cw->addWidget(wdgt);
    mlyout->addWidget(cw);
    setLayout(mlyout);
}

void dir_maindialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/directory.png"));

    e->accept();
}

void dir_maindialog::ShowSlave(QModelIndex idx)
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
        DBGMSG;
        return;
    }
    QStringList fields, values;
    int res;
    if (str.contains("карантин", Qt::CaseInsensitive))
        IsQuarantine = true;
    else
        IsQuarantine = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    fields << "Наименование" << "Родительский справочник" << "Права доступа";
    QString table = MainTable+"_полн";
    QString cmpfield = "Наименование";
    tfl.valuesbyfield(table,fields,cmpfield,str, values);
    if (tfl.result != TFRESULT_ERROR)
    {
        if (values.size() < 3)
        {
            WARNMSG("");
            return;
        }
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
            MessageBox2::error(this, "Ошибка", "Недостаточно прав для работы со справочником!");
    }
    else
        WARNMSG("");
    SlaveTV->resizeRowsToContents();
//     SlaveProxyModel->sort(1, Qt::AscendingOrder);
    QApplication::restoreOverrideCursor();
}

QString dir_maindialog::getMainIndex(int column)
{
    QString tmpString;
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    if (MainTV == 0)
    {
        DBGMSG;
        return QString();
    }
    if (column == 0)
    {
        QStringList tmpStringList = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString().split(".");
        if (tmpStringList.size() > 1)
            tmpString = tmpStringList.at(1);
    }
    else
        tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    tmpString.remove(QChar(0xFFFF));
    return tmpString;
}

QString dir_maindialog::getSlaveIndex(int column)
{
    QString tmpString;
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        DBGMSG;
        return QString();
    }
    if (column == 0)
    {
        QStringList tmpStringList = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString().split(".");
        if (tmpStringList.size() > 1)
            tmpString = tmpStringList.at(1);
    }
    else
        tmpString = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    tmpString.remove(QChar(0xFFFF));
    return tmpString;
}

void dir_maindialog::RefreshSlaveTV()
{
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV != 0)
    {
        TreeModel *mdl = static_cast<TreeModel *>(SlaveTV->model());
        mdl->Refresh();
    }
    SlaveTV->resizeColumnsToContents();
    SlaveTV->resizeRowsToContents();
}

void dir_maindialog::EditItem()
{
    QString tmpString = getSlaveIndex(0);
    if (!tmpString.isEmpty())
        EditItem(tmpString);
    else
        WARNMSG("");
}

void dir_maindialog::EditItem(QString str)
{
    if (!(pc.access & 0xAAAA)) // нет прав на запись
    {
        MessageBox2::error(this, "Ошибка", "Нет прав на изменение справочника");
        return;
    }
    QString tmps = getMainIndex(1);
    if (tmps.isEmpty())
    {
        WARNMSG("");
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
    QString SecondPart = getSlaveIndex(1);
    s_tqStackedWidget *cw = this->findChild<s_tqStackedWidget *>("cw");
    if (cw == 0)
    {
        WARNMSG("");
        return;
    }
    TwoColDialog *newdialog = new TwoColDialog(tmps+":"+SecondPart);
    int Mode = (isNewID) ? MODE_EDITNEW : MODE_EDIT;
    newdialog->setup(Table+"_полн",Mode,Id,IsQuarantine);
    if (newdialog->result)
    {
        WARNMSG("");
        return;
    }
    cw->addWidget(newdialog);
    cw->setCurrentWidget(newdialog);
    cw->repaint();
//    RefreshSlaveTV();
}

void dir_maindialog::AddNew()
{
    isNewID = true;
    QString newID;
    QString table = Tables.last()+"_полн";
    tfl.Insert(table, newID); // добавление элементов разрешается только в крайнюю таблицу
    if (tfl.result == TFRESULT_ERROR)
    {
        DBGMSG;
        return;
    }
    QString tmpString = getSlaveIndex(0);
    QStringList fl, vl;
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        DBGMSG;
        return;
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
    tfl.Update(table,fl,vl);
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
    QString tmpString = getSlaveIndex(0);
    long Num = tmpString.toLongLong(0, 10);
    DeleteDataUnconditional(QString::number(Num));
}

void dir_maindialog::DeleteDataUnconditional(QString id)
{
    QString table = Tables.last()+"_полн";
    tfl.Remove(table, id);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    else
    {
        MessageBox2::information(this, "Успешно", "Удалено успешно!");
        RefreshSlaveTV();
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
    if (pc.access & ACC_SYS_WR)
    {
        dir_adddialog *AddDialog = new dir_adddialog(false, MainTable); // no update. В MainTable передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
        AddDialog->exec();
    }
    else
        MessageBox2::error(this, "Ошибка", "Недостаточно прав для продолжения");
}

void dir_maindialog::EditDirDialog()
{
    if (pc.access & ACC_SYS_WR)
    {
        QString tmpString = getMainIndex(1);
        dir_adddialog *EditDialog = new dir_adddialog(true, MainTable, tmpString+"_полн"); // В MainTable передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
        EditDialog->exec();
    }
    else
        MessageBox2::error(this, "Ошибка", "Недостаточно прав для продолжения");
}

// ########################################## CONTEXTS ###################################################

// контекстное меню основного дерева

void dir_maindialog::mainContextMenu(QPoint)
{
    QAction *OpenAction = new QAction("Открыть", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(ShowSlave()));
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
