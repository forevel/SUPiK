#include "sysdireditor.h"
#include "../../widgets/s_tqtableview.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../models/s_duniversal.h"
#include "../../models/s_ncmodel.h"
#include "../s_2cdialog.h"
#include "../messagebox.h"

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
    s_tqTableView *tv = new s_tqTableView;
    s_ncmodel *mdl = new s_ncmodel;
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
    s_tqTableView *tv = this->findChild<s_tqTableView *>("tv");
    if (tv == 0)
    {
        SYSDDBG;
        return;
    }
    s_ncmodel *mdl = static_cast<s_ncmodel *>(tv->model());
    if (mdl == 0)
    {
        SYSDDBG;
        return;
    }
    mdl->setup("Справочники_сокращ");
    if (mdl->result)
    {
        SYSDWARN;
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
    s_tqTableView *tv = this->findChild<s_tqTableView *>("tv");
    if (tv == 0)
    {
        SYSDWARN;
        return;
    }
    QString tmpString = tv->model()->index(tv->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    tmpString = QString::number(tmpString.toInt(0));
    if (tmpString.isEmpty())
    {
        SYSDWARN;
        return;
    }
    s_2cdialog *newdialog = new s_2cdialog("Справочники:"+tmpString);
    newdialog->setup("Справочники_полн",MODE_EDIT,tmpString);
    if (newdialog->result)
        SYSDWARN;
    else
        newdialog->exec();
    FillTable();
}

void SysDirEditor::DeleteDir()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("tv");
    if (tv == 0)
    {
        SYSDDBG;
        return;
    }
    QString dirname = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString();
    if (!(MessageBox2::question(this, "Уверены?", "Вы уверены, что хотите удалить все сведения о справочнике?")))
        return;
    QStringList fl = QStringList() << "dirlist";
    QStringList vl = QStringList() << dirname;
    sqlc.RealDeleteFromDB("sup","dirlist",fl,vl);
    if (sqlc.result)
    {
        SYSDWARN;
        return;
    }
    SYSDINFO("Удалено успешно!");
    FillTable();
}
