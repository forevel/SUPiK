#include "sys_persdialog.h"
#include "../../models/treemodel.h"
#include "../../widgets/treeview.h"
#include "../../models/griddelegate.h"
#include <QVBoxLayout>
#include <QApplication>

sys_persdialog::sys_persdialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    SetupUI();
}

void sys_persdialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    TreeView *tv = new TreeView;
    tv->setObjectName("tv");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    TreeModel *treemodel = new TreeModel;
    int res = treemodel->SetupRaw("sup", "perspsw", "idperspsw", "idpersonel");
    if (res)
    {
        QApplication::restoreOverrideCursor();
        SYSPWARN;
        return;
    }
    tv->setModel(treemodel);
    GridDelegate *gridItemDelegate = new GridDelegate;
    tv->setItemDelegate(gridItemDelegate);
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Context(QPoint)));
    connect (tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeFieldsSlot(QModelIndex)));
    lyout->addWidget(tv);
    setLayout(lyout);
    tv->resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}
