#include "../inc/dir_maindialog.h"
#include "../inc/dir_adddialog.h"
#include "../inc/s_sql.h"
#include "../inc/s_2cdialog.h"

#include <QApplication>
#include <QPainter>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

dir_maindialog::dir_maindialog(QWidget *parent) :
    QDialog(parent)
{
    firstShow = true;
    SetupUI();
}

void dir_maindialog::SetupUI()
{
    MainLayout = new QVBoxLayout;
    MainFrameLayout = new QHBoxLayout;
    SlaveTV = new s_tqtreeview;
    MainTV = new s_tqtreeview;
    gridItemDelegate = new s_duniversal;
    MainFrame = new s_tqFrame;
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setMaximumWidth(2);
    MainFrameLayout->addWidget(MainTV, 10);
    MainFrameLayout->addWidget(line, 1);
    MainFrameLayout->addWidget(MainFrame, 25);
    MainL = new s_tqLabel;
    MainL->setText("Справочники");
    QFont font;
    font.setPointSize(15);
    MainL->setFont(font);
    MainLayout->addWidget(MainL, 0);
    MainLayout->setAlignment(MainL, Qt::AlignRight);
    MainLayout->addLayout (MainFrameLayout);
    setLayout(MainLayout);
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
    painter.drawPixmap(rect(), QPixmap(":/pic/Pic/directory.jpg"));

    e->accept();
}

void dir_maindialog::setDirTree()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
//    if ((pc.access & 0x01) || (pc.access & 0x40)) // sysread || altread
//    {
        MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
        disconnect(MainTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
        connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), \
                 this, SLOT(mainContextMenu(QPoint)));
        disconnect(MainTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
        connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));

        if (MainTreeModel == (void*)0) delete MainTreeModel;
        MainTreeModel = new s_aitemmodel(pc.sup, "dirlist", QSqlDatabase(), "", false); // если второй таблицы нет, в tble2 должна содержаться пустая строка (требование к таблице dirlist)
        QItemSelectionModel *m = MainTV->selectionModel();
        MainTreeModel->isEditable = false;
        MainTV->setModel(MainTreeModel);
        delete m;
        connect(MainTV, SIGNAL(expanded(QModelIndex)), MainTreeModel, SLOT(addExpandedIndex(QModelIndex)));
        connect(MainTV, SIGNAL(collapsed(QModelIndex)), MainTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
        connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(showDirDialog(QModelIndex)));
        MainTV->header()->setVisible(false);
        MainTV->setIndentation(2);
        MainTV->setAnimated(false);
        MainTV->setItemDelegate(gridItemDelegate);
        for (int i = 0; i < MainTV->header()->count(); i++)
            MainTV->resizeColumnToContents(i);
    QApplication::restoreOverrideCursor();
}

void dir_maindialog::showDirDialog()
{
    if (MainTV->model()->rowCount(MainTV->currentIndex()) != 0)
        setMainTVExpanded(MainTV->currentIndex());
    else
    {
        MainFrameLayout->replaceWidget(MainFrame, SlaveTV, Qt::FindDirectChildrenOnly);
        ShowSlaveTree(getMainIndex(1));
    }
}

void dir_maindialog::showDirDialog(QModelIndex idx)
{
    MainTV->setCurrentIndex(idx);
    showDirDialog();
}

// ############################################ SLOTS ####################################################

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
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(ChangeAdditionalFields()));
    QAction *AddDataChild;
    AddDataChild = new QAction ("Добавить элемент", this);
    AddDataChild->setSeparator(false);
    connect(AddDataChild, SIGNAL(triggered()), this, SLOT(AddDataChild()));
    QAction *AddSubDataChild;
    AddSubDataChild = new QAction ("Добавить субэлемент", this);
    AddSubDataChild->setSeparator(false);
    if (SlaveTreeModel->isTree)
    {
        connect(AddSubDataChild, SIGNAL(triggered()), this, SLOT(AddSubDataChild()));
        AddSubDataChild->setEnabled(true);
    }
    else
        AddSubDataChild->setEnabled(false);
    QAction *DeleteAction = new QAction("Удалить элемент", this);
    DeleteAction->setSeparator(false);
    connect (DeleteAction, SIGNAL(triggered()), this, SLOT(DeleteData()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    if (SlaveTVAccess & pc.access & 0x2492) // права на изменение
    {
        ContextMenu->addAction(AddDataChild);
        ContextMenu->addAction(AddSubDataChild);
        ContextMenu->addAction(ChangeDataChild);
    }
    if (SlaveTVAccess & pc.access & 0x4924) // права на удаление
        ContextMenu->addAction(DeleteAction);
    ContextMenu->exec(QCursor::pos()); // если есть права на удаление, на изменение тоже должны быть
}

// отображение подчинённого дерева в правой части

void dir_maindialog::ShowSlaveTree(QString str)
{
    QStringList fields, values;
    int i;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    fields << "pc" << "access";
    values = sqlc.getvaluesfromtablebyfield(pc.sup, "dirlist", fields, "dirlist", str);
    if (!sqlc.result)
    {
        tble = values.at(0);
        db = sqlc.getdb(tble.left(3));
        if (!db.isValid())
        {
            QMessageBox::warning(this, "warning!", "Нет такой базы данных: "+tble.left(3));
            QApplication::restoreOverrideCursor();
            return;
        }
        tble = tble.right(tble.size()-4);
        if (SlaveTreeModel == (void*)0) delete SlaveTreeModel;
        SlaveTreeModel = new s_aitemmodel(db, tble, QSqlDatabase(), "", false); // если второй таблицы нет, в tble2 должна содержаться пустая строка (требование к таблице dirlist)
        if (SlaveTreeModel->result)
        {
            QMessageBox::warning(this, "warning!", "Проблема №" + QString::number(SlaveTreeModel->result));
            QApplication::restoreOverrideCursor();
            return;
        }
        QItemSelectionModel *m = SlaveTV->selectionModel();
        SlaveTreeModel->isEditable = false;
        SlaveTV->setModel(SlaveTreeModel);
        delete m;
        SlaveTV->header()->setDefaultAlignment(Qt::AlignCenter);
        SlaveTV->header()->setVisible(true);
        SlaveTV->setIndentation(2);
        SlaveTV->setAnimated(false);
        SlaveTV->setItemDelegate(gridItemDelegate);
        for (i = 0; i < SlaveTV->header()->count(); i++)
            SlaveTV->resizeColumnToContents(i);
        SlaveTVAccess = values.at(1).toLongLong(0, 16);
        SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
        disconnect(SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
        connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
        disconnect(SlaveTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
        connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeAdditionalFields(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(expanded(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(expanded(QModelIndex)), SlaveTreeModel, SLOT(addExpandedIndex(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(collapsed(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(collapsed(QModelIndex)), SlaveTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(clicked(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(clicked(QModelIndex)), this, SLOT(setSlaveTVExpanded(QModelIndex)));
    }
    else
        QMessageBox::warning(this, "warning", "Не найдена ссылка на таблицу справочника!", QMessageBox::Ok, QMessageBox::NoButton);
    QApplication::restoreOverrideCursor();
}

// обработка раскрывания корней дерева

void dir_maindialog::setMainTVExpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (MainTV->isExpanded(index))
            MainTV->setExpanded(index, false);
        else
            MainTV->setExpanded(index, true);
    }
    for (int i = MainTV->header()->count(); i >= 0; --i)
        MainTV->resizeColumnToContents(i);
}

// обработка раскрывания корней дочернего дерева

void dir_maindialog::setSlaveTVExpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (SlaveTV->isExpanded(index))
            SlaveTV->setExpanded(index, false);
        else
            SlaveTV->setExpanded(index, true);
    }
    for (int i = SlaveTV->header()->count(); i >= 0; --i)
        SlaveTV->resizeColumnToContents(i);
}

QString dir_maindialog::getMainIndex(int column)
{
    QModelIndex index = MainTV->model()->index(MainTV->currentIndex().row(), column, MainTV->model()->parent(MainTV->currentIndex()));
    QString tmpString = index.data(Qt::DisplayRole).toString();
    if (!column)
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

QString dir_maindialog::getSlaveIndex(int column)
{
    QModelIndex index = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, SlaveTV->model()->parent(SlaveTV->currentIndex()));
    QString tmpString = index.data(Qt::DisplayRole).toString();
    if (!column)
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void dir_maindialog::ChangeAdditionalFields(QModelIndex index)
{
    if (SlaveTV->model()->rowCount(SlaveTV->currentIndex()) != 0) // для родителей запрещено иметь дополнительные поля
        setSlaveTVExpanded(SlaveTV->currentIndex());
    else
    {
        SlaveTV->setCurrentIndex(index);
        ChangeAdditionalFields();
    }
}

void dir_maindialog::ChangeAdditionalFields()
{
    ChangeAdditionalFields(getSlaveIndex(0));
}

void dir_maindialog::ChangeAdditionalFields(QString str)
{
    int res;
    s_2cdialog *newdialog = new s_2cdialog("Справочники:"+getMainIndex(1));
    if (!(res = newdialog->setup(getMainIndex(1)+"_полная", str)))
    {
        //newdialog->setModal(true);
        newdialog->exec();
    }
    else
    {
        delete newdialog;
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(res),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
/*    sqldialog = new s_sqlfieldsdialog;
    if (!(res = sqldialog->SetupUI(db, tble, str, "Справочник:"+getMainIndex(1)))) // берём ИД текущего элемента
    {
        connect(sqldialog, SIGNAL(accepted()), this, SLOT(showDirDialog()));
        sqldialog->exec();
    }
    else
    {
        delete sqldialog;
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(res),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    } */
}

void dir_maindialog::AddDataChild()
{
    AddChild(QString::number(0), "");
}

void dir_maindialog::AddSubDataChild()
{
    AddChild(QString::number(getSlaveIndex(0).toLongLong(0,10)), "");
}

void dir_maindialog::AddChild(QString alias, QString str)
{
    QString newID = QString::number(sqlc.getnextfreeindex(db, tble));
    ChangeAdditionalFields("0");
}

void dir_maindialog::DeleteData()
{
    QMessageBox msgBox;
    msgBox.setText("Вы уверены, что хотите удалить элемент?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel)
        return;
    QString tmpString = QString::number(getSlaveIndex(0).toLongLong(0,10));
    QStringList tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, tble, "id"+tble, "idalias", tmpString);
    while (!tmpStringList.isEmpty())
    {
        int res = sqlc.deletefromdb(db, tble, "id"+tble, tmpStringList.at(0));
        if (res)
            QMessageBox::warning(this, "warning!", "Невозможно удалить элемент №"+tmpString);
        tmpStringList.removeAt(0);
    }
    int res = sqlc.deletefromdb(db, tble, "id"+tble, tmpString);
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
    QSqlDatabase db = sqlc.getdb("sup");
    QString dirlink = sqlc.getvaluefromtablebyfield(db, "dirlist", "pc", "dirlist", tmpString);
    dir_adddialog *EditDialog = new dir_adddialog(true, dirlink);
    EditDialog->exec();
}
