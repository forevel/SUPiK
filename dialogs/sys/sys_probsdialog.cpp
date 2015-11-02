#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>

#include "sys_probsdialog.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqtableview.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"

sys_probsdialog::sys_probsdialog(QWidget *parent) :
    QDialog(parent)
{
    slmodel = new QStringListModel;
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void sys_probsdialog::SetupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    s_tqTableView *mainTV = new s_tqTableView(false);
    mainTV->setObjectName("mainTV");
    mainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    disconnect(mainTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
    connect (mainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(mainTVcontextmenu(QPoint)));
    disconnect(mainTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
    connect (mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(removeProb(QModelIndex)));
    mainLayout->addWidget(mainTV, 100);
    s_tqPushButton *pb = new s_tqPushButton("Обновить");
    pb->setIcon(QIcon(":/res/refresh.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(refresh()));
    mainLayout->addWidget(pb, 0);
    setLayout(mainLayout);
    updatemainTV();
}

void sys_probsdialog::mainTVcontextmenu(QPoint)
{
    QAction *OpenAction = new QAction("Устранить проблему", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(removeProb()));

    QMenu *SystemContextMenu = new QMenu;
    SystemContextMenu->addAction(OpenAction);
    SystemContextMenu->exec(QCursor::pos());
}

void sys_probsdialog::removeProb(QModelIndex index)
{
    Q_UNUSED(index);
/*    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    QStringList probchunks;
    probchunks = tv->model()->data(index, Qt::DisplayRole).toString().split(" "); // делим сообщение на части, используя разделитель - пробел
    if (probchunks.at(0) == "Невозможно")
    {

    }
    else if (probchunks.at(0) == "Отсутствует")
    {
        field = probchunks.at(2); // "Отсутствует поле <поле> в таблице <таблица>
        tble = probchunks.at(5);
        if (tble == "полей") // "Отсутствует поле " + tables.at(j) + " или набор полей alias+idalias в таблице " + tables.at(j))
        {
            db = sqlc.GetDBByTableName(field);
            tble = field;
            dlg = new QDialog;
            dlg->setObjectName("iddlg");
            QHBoxLayout *lyt = new QHBoxLayout(dlg);
            s_tqLabel *lbl = new s_tqLabel("Имя поля:", dlg);
            s_tqComboBox *cb = new s_tqComboBox(dlg);
            QStringList tmpsl;
            tmpsl << field << "alias+idalias";
            slmodel->setStringList(tmpsl);
            cb->setModel(slmodel);
            cb->setObjectName("idcb");
            cb->setMinimumWidth(120);
            s_tqPushButton *pbo = new s_tqPushButton("Ага",dlg);
            s_tqPushButton *pbc = new s_tqPushButton("Неа",dlg);
            lyt->addWidget(lbl);
            lyt->addWidget(cb, 100);
            lyt->addWidget(pbo);
            lyt->addWidget(pbc);
            dlg->setLayout(lyt);
            dlg->setModal(true);
            connect(pbc, SIGNAL(clicked()), dlg, SLOT(close()));
            connect(pbo, SIGNAL(clicked()), this, SLOT(addcol()));
            dlg->exec();
            return;
        }
        else
        {
            db = sqlc.GetDBByTableName(tble);
            if (field == "deleted")
                sqlc.AddColumn(db, tble, field, "0");
            else
                sqlc.AddColumn(db, tble, field);
            if (sqlc.result)
                QMessageBox::warning(this,"warning!","Ошибка при добавлении поля "+field+" в таблицу "+tble+"!");
        }
        refresh();
        QMessageBox::information(this,"Ура!","Добавлено поле "+field+" в таблицу "+tble+"!");
        updatemainTV();
        emit updateprobsnumber();
    }
    else if (probchunks.at(0) == "Добавлены")
    {
        tble = probchunks.at(5);
        if (tble == "номенклатуры")
            tble = "qnk";
        else if (tble == "Altium")
            tble = "qaltium";
        else if (tble == "Schemagee")
            tble = "qschemagee";
    } */
}

void sys_probsdialog::removeProb()
{
/*    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    QModelIndex index = tv->currentIndex();
    removeProb(index);*/
}

void sys_probsdialog::updatemainTV()
{
/*    int i;
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    if (mainmodel == (void*)0) delete mainmodel;
    QList<int> setlist;
    pc.fillallprob();
    for (i = 0; i < pc.allprobs.size(); i++)
    {
        if (pc.allprobs.at(i).mid(0, 1) == "!")
        {
            setlist << 1;
            pc.allprobs.replace(i, pc.allprobs.at(i).right(pc.allprobs.at(i).size()-1));
        }
        else if (pc.allprobs.at(i).mid(0, 1) == "@")
        {
            setlist << 2;
            pc.allprobs.replace(i, pc.allprobs.at(i).right(pc.allprobs.at(i).size()-1));
        }
        else
            setlist << 4;
    }
    QList<QStringList> tmpsl;
    tmpsl.clear();
    tmpsl.append(pc.allprobs);
//    mainmodel = new s_ncmodel(tmpsl, setlist);
    mainmodel->isEditable = false;
    tv->setModel(mainmodel);
    tv->resizeColumnsToContents(); */
}

void sys_probsdialog::refresh()
{
/*    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    // обновление событий
    pc.UpdateInProgress = true; // признак текущего обновления списка сообщений, чтобы не было лишних сообщений
    pc.DBCheck();
    pc.notify = 0;
    pc.minutetest();
    pc.Acknowledged = true; // чтобы при обновлении не выскакивало лишнее сообщение
    pc.NewNotifyHasArrived = false;
    pc.UpdateInProgress = false;
    QApplication::restoreOverrideCursor(); */
}

void sys_probsdialog::addcol()
{
/*    s_tqComboBox *cb = dlg->findChild<s_tqComboBox *>("idcb");
    db = sqlc.GetDBByTableName(tble);
    QString tmpString = cb->currentText();
    if (cb->currentText() != tble) // пишем alias & idalias
    {
        field = "alias";
        sqlc.AddColumn(db, tble, field);
        if (sqlc.result)
            QMessageBox::warning(this,"warning!","Ошибка при добавлении поля "+field+" в таблицу "+tble+"!");
        field = "idalias";
    }
    else
        field = cb->currentText();
    sqlc.AddColumn(db, tble, field);
    if (sqlc.result)
        QMessageBox::warning(this,"warning!","Ошибка при добавлении поля "+field+" в таблицу "+tble+"!");
    else
    {
        refresh();
        QMessageBox::information(this,"Ура!","Добавлено поле "+field+" в таблицу "+tble+"!");
        updatemainTV();
        emit updateprobsnumber();
    }
    dlg->close(); */
}
