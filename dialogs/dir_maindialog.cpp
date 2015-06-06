#include "dir_maindialog.h"
#include "dir_adddialog.h"
#include "../gen/s_sql.h"
#include "s_2cdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqsplitter.h"
#include "sys_acceptexist.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "s_sqlfieldsdialog.h"

#include <QApplication>
#include <QPainter>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSortFilterProxyModel>

dir_maindialog::dir_maindialog(QWidget *parent) :
    QDialog(parent)
{
    firstShow = true;
    MainTVIsTree = false;
    SlaveTVIsTree = false;
    IsQuarantine = false;
    SetupUI();
}

void dir_maindialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Справочники");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    s_tqTreeView *SlaveTV = new s_tqTreeView;
    s_tqTableView *MainTV = new s_tqTableView;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
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
    rlyout->addWidget(SlaveTV);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
}

void dir_maindialog::showEvent(QShowEvent *e)
{
    if (firstShow)
    {
        setDirTree();
        firstShow = false;
    }
    e->accept();
}

void dir_maindialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/directory.jpg"));

    e->accept();
}

void dir_maindialog::setDirTree()
{
    s_ncmodel *MainTableModel = new s_ncmodel;
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        ShowErMsg(ER_DIRMAIN+0x01);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(mainContextMenu(QPoint)));
    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));

    int res = MainTableModel->setup("Справочники_сокращ");
    if (res)
    {
        ShowErMsg(res+ER_DIRMAIN+0x04);
        QApplication::restoreOverrideCursor();
        return;
    }
    MainTV->setModel(MainTableModel);
    connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
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
    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        ShowErMsg(ER_DIRMAIN+0x11);
        return;
    }
    QStringList fields, values;
    int i, res;
    tble = str; // для функций удаления и добавления необходимо знать имя текущей таблицы
    if (str.contains("карантин", Qt::CaseInsensitive))
        IsQuarantine = true;
    else
        IsQuarantine = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    fields << "dirlist" << "access";
    values = sqlc.getvaluesfromtablebyfield(pc.sup, "dirlist", fields, "dirlist", str);
    if (!sqlc.result)
    {
        if (values.at(1).toUInt(0,16) & pc.access)
        {
            s_ntmodel *SlaveTreeModel = new s_ntmodel;
            if (values.at(0).contains("Номенклатура", Qt::CaseSensitive))
                res = SlaveTreeModel->Setup("Категории_сокращ",values.at(0)+"_сокращ");
            else
                res = SlaveTreeModel->Setup(values.at(0) + "_сокращ");
            if (res == ER_NTMODEL) // это не дерево
            {
                s_ncmodel *SlaveTableModel = new s_ncmodel;
                int res = SlaveTableModel->setup(values.at(0) + "_сокращ");
                if (res)
                {
                    ShowErMsg(res+0x14+ER_DIRMAIN);
                    QApplication::restoreOverrideCursor();
                    return;
                }
                SlaveTV->setModel(SlaveTableModel);
                SlaveTVIsTree = false;
            }
            else
            {
                SlaveTV->setModel(SlaveTreeModel);
                SlaveTVIsTree = true;
                connect(SlaveTV, SIGNAL(expanded(QModelIndex)), SlaveTreeModel, SLOT(addExpandedIndex(QModelIndex)));
                connect(SlaveTV, SIGNAL(collapsed(QModelIndex)), SlaveTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
            }
            SlaveTV->header()->setDefaultAlignment(Qt::AlignCenter);
            SlaveTV->header()->setVisible(true);
            SlaveTV->setIndentation(2);
            SlaveTV->setAnimated(false);
            s_duniversal *gridItemDelegate = new s_duniversal;
            SlaveTV->setItemDelegate(gridItemDelegate);
            for (i = 0; i < SlaveTV->header()->count(); i++)
                SlaveTV->resizeColumnToContents(i);
            SlaveTVAccess = values.at(1).toLongLong(0, 16);
            SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
            disconnect(SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
            connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
            disconnect(SlaveTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
            connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditItem(QModelIndex)));
        }
        else
            QMessageBox::warning(this, "warning", "Недостаточно прав для работы со справочником!");
    }
    else
        QMessageBox::warning(this, "warning", "Не найдена ссылка на таблицу справочника!", QMessageBox::Ok, QMessageBox::NoButton);
    QApplication::restoreOverrideCursor();
//    adjustSize();
}

QString dir_maindialog::getMainIndex(int column)
{
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        ShowErMsg(ER_DIRMAIN+0x31);
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

QString dir_maindialog::getSlaveIndex(int column)
{
    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        ShowErMsg(ER_DIRMAIN+0x41);
        return QString();
    }
    QString tmpString = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, SlaveTV->model()->parent(SlaveTV->currentIndex())).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void dir_maindialog::EditItem(QModelIndex index)
{
    Q_UNUSED(index);
    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        ShowErMsg(ER_DIRMAIN+0x41);
        return;
    }
    if (SlaveTVIsTree)
    {
        s_ntmodel *mdl = static_cast<s_ntmodel *>(SlaveTV->model());
        if (mdl->data(index,Qt::ForegroundRole).value<QColor>() != mdl->colors[0])
            return; // для родителей запрещено иметь дополнительные поля
    }
    QString tmpString = getSlaveIndex(0);
    if (!tmpString.isEmpty())
        EditItem(tmpString);
    else
        ShowErMsg(ER_DIRMAIN+0x44);
}

void dir_maindialog::EditItem(QString str)
{
//    int res;
    QString tmps = getMainIndex(1);
    if (tmps.isEmpty())
    {
        ShowErMsg(ER_DIRMAIN+0x51);
        return;
    }
    s_2cdialog *newdialog = new s_2cdialog;
    newdialog->setup(tmps+"_полная", MODE_EDIT, str, "Справочники:"+tmps, "", IsQuarantine);
//    if (IsQuarantine)
//        newdialog->IsQuarantine = true;
//    newdialog->Mode = MODE_EDIT;
    if (newdialog->result)
    {
//        newdialog->close();
        ShowErMsg(ER_DIRMAIN+newdialog->result+0x54);
    }
    else
        newdialog->exec();
//    if (!(res = newdialog->setup(tmps+"_полная", str)))
//        newdialog->exec();
//    else
//        ShowErMsg(ER_DIRMAIN+0x54);
}

void dir_maindialog::AddNew()
{
    QString newID = tfl.insert(tble+"_полная");
    EditItem(newID);
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
    QString tmpString = QString::number(getSlaveIndex(0).toLongLong(0,10));
    res = tfl.remove(tble, tmpString);
    if (res)
        QMessageBox::warning(this, "warning!", "Невозможно удалить элемент №"+tmpString);
    else
    {
        QMessageBox::warning(this, "warning!", "Удалено успешно!");
        showDirDialog();
    }
}

void dir_maindialog::AddDirDialog()
{
    dir_adddialog *AddDialog = new dir_adddialog(false); // no update
    if (AddDialog->result)
    {
        delete AddDialog;
        return;
    }
    AddDialog->exec();
}

void dir_maindialog::EditDirDialog()
{
    QString tmpString = getMainIndex(1);
    dir_adddialog *EditDialog = new dir_adddialog(true, tmpString);
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

void dir_maindialog::ShowErMsg(int ermsg)
{
    QMessageBox::warning(this, "warning!", "Ошибка 0x" + QString::number(ermsg,16));
}
