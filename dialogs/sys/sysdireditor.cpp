#include "sysdireditor.h"
#include "../../widgets/treeview.h"
#include "../../widgets/wd_func.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../models/s_duniversal.h"
#include "../../models/treemodel.h"
#include "../gen/twocoldialog.h"
#include "../gen/messagebox.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QAction>
#include <QMenu>

SysDirEditor::SysDirEditor(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    SetupUI();
}

void SysDirEditor::SetupUI()
{
    TreeView *tv = new TreeView;
    TreeModel *mdl = new TreeModel;
    tv->setModel(mdl);
    tv->setObjectName("tv");
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tv,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TvMenu(QPoint)));
    connect(tv,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(EditDir()));
    s_duniversal *gridItemDelegate = new s_duniversal;
    tv->setItemDelegate(gridItemDelegate);
    QVBoxLayout *lyout = new QVBoxLayout;
    lyout->addWidget(tv);
    setLayout(lyout);
    FillTable();
}

void SysDirEditor::FillTable()
{
    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    TreeModel *mdl = static_cast<TreeModel *>(tv->model());
    if (mdl == 0)
    {
        DBGMSG;
        return;
    }

    if (mdl->Setup("Справочники_сокращ"))
    {
        WARNMSG("");
        return;
    }
    tv->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tv->horizontalHeader()->setVisible(true);
    tv->verticalHeader()->setVisible(false);
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
}

void SysDirEditor::TvMenu(QPoint p)
{
    Q_UNUSED(p);
    QAction *ChangeAction;
    ChangeAction = new QAction ("Изменить элемент", this);
    ChangeAction->setSeparator(false);
    connect(ChangeAction, SIGNAL(triggered()), this, SLOT(EditDir()));
    QAction *DeleteAction = new QAction("Удалить элемент", this);
    DeleteAction->setSeparator(false);
    connect (DeleteAction, SIGNAL(triggered()), this, SLOT(DeleteDir()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->addAction(ChangeAction);
    ContextMenu->addAction(DeleteAction);
    ContextMenu->exec(QCursor::pos());
}

void SysDirEditor::EditDir()
{
/*    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        WARNMSG("");
        return;
    }
    QString tmpString;
    QStringList tmpStringList = tv->model()->index(tv->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString().split("."); */
    QString tmpString = WDFunc::TVField(this, "tv", 0, true);
/*    QStringList tmpStringList = tmpString.split(".");
    if (tmpStringList.size() > 1)
        tmpString = tmpStringList.at(1);
    tmpString = QString::number(tmpString.toInt(0)); */
    if (tmpString.isEmpty())
    {
        WARNMSG("");
        return;
    }
    TwoColDialog *newdialog = new TwoColDialog("Справочники:"+tmpString);
    newdialog->setup("Справочники_полн",MODE_EDIT,tmpString);
    if (newdialog->result)
        WARNMSG("");
    else
        newdialog->exec();
    FillTable();
}

void SysDirEditor::DeleteDir()
{
/*    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString dirname = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString(); */
    QString dirname = WDFunc::TVField(this, "tv", 1);
    if (!(MessageBox2::question(this, "Уверены?", "Вы уверены, что хотите удалить все сведения о справочнике?")))
        return;
    QStringList fl = QStringList() << "dirlist";
    QStringList vl = QStringList() << dirname;
    sqlc.RealDeleteFromDB("sup","dirlist",fl,vl);
    if (sqlc.result)
    {
        WARNMSG("");
        return;
    }
    MessageBox2::information(this, "Успешно", "Удалено успешно!");
    FillTable();
}
