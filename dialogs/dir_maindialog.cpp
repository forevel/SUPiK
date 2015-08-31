#include "dir_maindialog.h"
#include "dir_adddialog.h"
#include "s_2cdialog.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include "../gen/publicclass.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqsplitter.h"
#include "../models/s_duniversal.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"

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
    s_tqLabel *lbl = new s_tqLabel(tble);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    s_tqTreeView *SlaveTV = new s_tqTreeView;
    s_tqTableView *SlaveTbV = new s_tqTableView;
    s_tqTableView *MainTV = new s_tqTableView;
    MainTableModel = new s_ncmodel;
    SlaveTreeModel = new s_ntmodel;
    SlaveTableModel = new s_ncmodel;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
    SlaveTbV->setObjectName("SlaveTbV");
    SlaveTV->setModel(SlaveTreeModel);
    SlaveTbV->setModel(SlaveTableModel);
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    MainTableModel->setup(tble+"_сокращ");
    if (MainTableModel->result)
    {
        emit error(MainTableModel->result+ER_DIRMAIN,0x02);
        QApplication::restoreOverrideCursor();
        return;
    }
    MainTV->setModel(MainTableModel);
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    connect(gridItemDelegate,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
    MainTV->setItemDelegate(gridItemDelegate);
    SlaveTbV->setItemDelegate(gridItemDelegate);
    SlaveTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
    SlaveTbV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(mainContextMenu(QPoint)));
//    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));
    connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));
    connect(SlaveTV, SIGNAL(expanded(QModelIndex)), SlaveTreeModel, SLOT(addExpandedIndex(QModelIndex)));
    connect(SlaveTV, SIGNAL(collapsed(QModelIndex)), SlaveTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
    connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
    connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditItem(QModelIndex)));
    connect (SlaveTbV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
    connect (SlaveTbV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditItem(QModelIndex)));

    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    rlyout->addWidget(SlaveTV);
    rlyout->addWidget(SlaveTbV);
    SlaveTV->setVisible(false);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    s_tqPushButton *pb = new s_tqPushButton("Выход");
    connect(pb,SIGNAL(clicked()),this,SLOT(close()));
    lyout->addWidget(pb);
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
        emit error(ER_DIRMAIN,0x11);
        return;
    }
    s_tqTableView *SlaveTbV = this->findChild<s_tqTableView *>("SlaveTbV");
    if (SlaveTbV == 0)
    {
        emit error(ER_DIRMAIN,0x11);
        return;
    }
    QStringList fields, values;
    int i, res;
    slvtble = str; // для функций удаления и добавления необходимо знать имя текущей таблицы
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
            if (!values.at(1).isEmpty()) // есть родительские категории у справочника
            {
                twodb = true;
                res = SlaveTreeModel->Setup(values.at(1)+"_сокращ",values.at(0)+"_сокращ");
            }
            else
            {
                twodb = false;
                res = SlaveTreeModel->Setup(values.at(0) + "_сокращ");
            }
            if (res == ER_NTMODEL) // это не дерево
            {
                SlaveTableModel->setup(values.at(0) + "_сокращ");
                if (SlaveTableModel->result)
                {
                    emit error(SlaveTableModel->result+ER_DIRMAIN,0x12);
                    QApplication::restoreOverrideCursor();
                    return;
                }
                SlaveTVIsTree = false;
                SlaveTbV->setVisible(true);
                SlaveTV->setVisible(false);
                SlaveTbV->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
                SlaveTbV->horizontalHeader()->setVisible(true);
                SlaveTbV->verticalHeader()->setVisible(false);
                SlaveTbV->resizeColumnsToContents();
                SlaveTbV->resizeRowsToContents();
            }
            else
            {
                SlaveTVIsTree = true;
                SlaveTbV->setVisible(false);
                SlaveTV->setVisible(true);
                SlaveTV->header()->setDefaultAlignment(Qt::AlignCenter);
                SlaveTV->header()->setVisible(true);
                SlaveTV->setIndentation(2);
                SlaveTV->setAnimated(false);
                for (i = 0; i < SlaveTV->header()->count(); i++)
                    SlaveTV->resizeColumnToContents(i);
            }
            SlaveTVAccess = values.at(2).toLongLong(0, 16);
        }
        else
            QMessageBox::warning(this, "warning", "Недостаточно прав для работы со справочником!");
    }
    else
        emit error(tfl.result+ER_DIRMAIN, 0x13);
    QApplication::restoreOverrideCursor();
}

QString dir_maindialog::getMainIndex(int column)
{
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_DIRMAIN,0x21);
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
    if (SlaveTVIsTree)
    {
        s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
        if (SlaveTV == 0)
        {
            emit error(ER_DIRMAIN,0x31);
            return QString();
        }
        tmpString = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, SlaveTV->model()->parent(SlaveTV->currentIndex())).data(Qt::DisplayRole).toString();
    }
    else
    {
        s_tqTableView *SlaveTbV = this->findChild<s_tqTableView *>("SlaveTbV");
        if (SlaveTbV == 0)
        {
            emit error(ER_DIRMAIN,0x31);
            return QString();
        }
        tmpString = SlaveTbV->model()->index(SlaveTbV->currentIndex().row(),column,QModelIndex()).data(Qt::DisplayRole).toString();
    }
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void dir_maindialog::EditItem(QModelIndex index)
{
    Q_UNUSED(index);
    if (SlaveTVIsTree)
    {
        s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
        if (SlaveTV == 0)
        {
            emit error(ER_DIRMAIN,0x41);
            return;
        }
        s_ntmodel *mdl = static_cast<s_ntmodel *>(SlaveTV->model());
        if (twodb) // если имеем дело с двумя таблицами
        {
            if ((mdl->data(index,Qt::ForegroundRole).value<QColor>() != mdl->colors[0]) || \
                    (mdl->data(index,Qt::FontRole).value<QFont>() != mdl->fonts[0]))
                return; // для родителей запрещено иметь дополнительные поля
        }
        else
        {
            if ((mdl->data(index,Qt::ForegroundRole).value<QColor>() == mdl->colors[4]) && \
                    (mdl->data(index,Qt::FontRole).value<QFont>() == mdl->fonts[4]))
                return; // для родителей запрещено иметь дополнительные поля
        }
    }
    QString tmpString = getSlaveIndex(0);
    if (!tmpString.isEmpty())
        EditItem(tmpString);
    else
        emit error(ER_DIRMAIN,0x42);
}

void dir_maindialog::EditItem(QString str)
{
    QString tmps = getMainIndex(1);
    if (tmps.isEmpty())
    {
        emit error(ER_DIRMAIN,0x51);
        return;
    }
    s_2cdialog *newdialog = new s_2cdialog(tble+":"+tmps);
    int Mode = (isNewID) ? MODE_EDITNEW : MODE_EDIT;
    newdialog->setup(tmps+"_полн",Mode,str);
    if (newdialog->result)
        emit error(ER_DIRMAIN+newdialog->result,0x52);
    else
        newdialog->exec();
    ShowSlaveTree(slvtble);
}

void dir_maindialog::AddNew()
{
    isNewID = true;
    QString newID = tfl.insert(slvtble+"_полн");
    QString tmpString = getSlaveIndex(0);
    QStringList fl, vl;
    if ((!tmpString.isEmpty()) && (SlaveTVIsTree))
    {
        fl << "ИД" << "ИД_а";
        vl << newID << tmpString;
        tfl.idtois(slvtble+"_полн",fl,vl);
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
    tfl.remove(slvtble+"_полн", id);
    if (tfl.result)
    {
        emit error(ER_DIRMAIN+tfl.result, 0x61);
        return;
    }
    else
    {
        QMessageBox::warning(this, "warning!", "Удалено успешно!");
        showDirDialog();
    }
}

void dir_maindialog::AddDirDialog()
{
    dir_adddialog *AddDialog = new dir_adddialog(false, tble); // no update. В tble передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
    connect(AddDialog,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
    AddDialog->exec();
}

void dir_maindialog::EditDirDialog()
{
    QString tmpString = getMainIndex(1);
    dir_adddialog *EditDialog = new dir_adddialog(true, tble, tmpString+"_полн"); // В tble передаётся тип справочника ("Справочники", "Справочники системные" и т.д.)
    connect(EditDialog,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
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
