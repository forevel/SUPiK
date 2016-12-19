#include "sys_persdialog.h"
#include "../../models/treemodel.h"
#include "../../widgets/treeview.h"
#include "../../models/griddelegate.h"
#include "../s_2cdialog.h"
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
//    QStringList PersPswFields = QStringList() << "idperspsw" << "idpersonel";
    int res = treemodel->Setup("Пароли_сокращ", true);
    if (res)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    }
    tv->setModel(treemodel);
    GridDelegate *gridItemDelegate = new GridDelegate;
    tv->setItemDelegate(gridItemDelegate);
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Context(QPoint)));
    connect (tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangePers(QModelIndex)));
    lyout->addWidget(tv);
    setLayout(lyout);
    tv->resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}

void sys_persdialog::ChangePers(QModelIndex idx)
{
    Q_UNUSED(idx);
    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QModelIndex index = tv->model()->index(tv->currentIndex().row(), 0, tv->model()->parent(tv->currentIndex()));

    QString tmpString = index.data(Qt::DisplayRole).toString();
    pc.ConvertId(true, tmpString);
    s_2cdialog *newdialog = new s_2cdialog("");
    newdialog->SetupRaw("sup", "perspsw", MODE_EDIT_RAW, tmpString);
    if (!newdialog->result)
    {
        newdialog->setModal(true);
        newdialog->exec();
//        UpdateTree();
    }
    else
    {
        WARNMSG("");
        return;
    }
}
