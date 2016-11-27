#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>
#include <QTimer>
#include <QPainter>

#include "sys_probsdialog.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/treeview.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"

SysProblemsDialog::SysProblemsDialog(QWidget *parent) :
    QDialog(parent)
{
    QTimer *tmr = new QTimer;
    tmr->setInterval(2000);
    connect(tmr,SIGNAL(timeout()),this,SLOT(UpdateProblems()));
    slmodel = new QStringListModel;
    mainmodel = new TreeModel;
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    tmr->start();
}

void SysProblemsDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2WallPaper.png"));
    e->accept();
}

void SysProblemsDialog::SetupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Список проблем");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    mainLayout->addLayout(hlyout);
    TreeView *mainTV = new TreeView(TreeView::TV_PLAIN);
    mainTV->setObjectName("mainTV");
    mainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (mainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(MainTvContextMenu(QPoint)));
    connect (mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(RemoveProb()));
    mainLayout->addWidget(mainTV, 100);
/*    s_tqPushButton *pb = new s_tqPushButton("Обновить");
    pb->setIcon(QIcon(":/res/refresh.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(refresh()));
    mainLayout->addWidget(pb, 100); */
    setLayout(mainLayout);
    mainmodel->ClearModel();
    mainmodel->insertColumns(0, 1, QModelIndex());
    mainTV->setModel(mainmodel);
    InitialModelFill();
}

void SysProblemsDialog::MainTvContextMenu(QPoint)
{
    QAction *OpenAction = new QAction("Устранить проблему", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(RemoveProb()));

    QMenu *SystemContextMenu = new QMenu;
    SystemContextMenu->addAction(OpenAction);
    SystemContextMenu->exec(QCursor::pos());
}



void SysProblemsDialog::RemoveProb()
{
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    QModelIndex index = tv->currentIndex();
/*    QStringList probchunks;
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
        UpdateMainTv();
        emit ProblemsNumberUpdated();
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

void SysProblemsDialog::InitialModelFill()
{
    QFont font = QFont("MS Sans Serif", -1, QFont::Normal);
    for (int i=0; i<pc.ProblemsList.size(); i++)
    {
        PublicClass::ProblemStruct pl = pc.ProblemsList.at(i);
        PublicClass::ValueStruct vl;
        vl.Type=VS_STRING;
        vl.Value = ProbStringCombiner(pl);
        QList<PublicClass::ValueStruct> lvl;
        lvl.append(vl);
        mainmodel->AddItemToTree(lvl);
        mainmodel->SetLastItem(PublicClass::ProblemForegroundColors()[pl.ProblemType], PublicClass::ProblemBackgroundColors()[pl.ProblemType], \
                               font, QIcon());
    }
    UpdateMainTv();
}

void SysProblemsDialog::NewProblemsDetected()
{
    QFont font = QFont("MS Sans Serif", -1, QFont::Normal);
    pc.EPLMutex.lock();
    while (!pc.ExchangeProblemsList.isEmpty())
    {
        PublicClass::ProblemStruct pl = pc.ExchangeProblemsList.takeFirst();
        pc.ProblemsList.append(pl); // перекидываем из временного хранилища проблем в постоянное
        PublicClass::ValueStruct vl;
        vl.Type=VS_STRING;
        vl.Value = ProbStringCombiner(pl);
        QList<PublicClass::ValueStruct> lvl;
        lvl.append(vl);
        mainmodel->AddItemToTree(lvl);
        mainmodel->SetLastItem(PublicClass::ProblemForegroundColors()[pl.ProblemType], PublicClass::ProblemBackgroundColors()[pl.ProblemType], \
                               font, QIcon());
    }
    pc.EPLMutex.unlock();
    emit ProblemsNumberUpdated();
    UpdateMainTv();
}

void SysProblemsDialog::UpdateMainTv()
{
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    tv->resizeColumnsToContents();
}

QString SysProblemsDialog::ProbStringCombiner(PublicClass::ProblemStruct vl)
{
    QString tmps;
    switch (vl.ProblemSubType)
    {
    case PublicClass::PST_FIELDMISSED:
    {
        tmps = "Отсутствует поле " + vl.ProblemField + " в таблице " + vl.ProblemTable;
        break;
    }
    case PublicClass::PST_FIELDEMPTY:
    {
        tmps = "Не заполнено поле " + vl.ProblemField + " в таблице " + vl.ProblemTable + " по индексу " + vl.ProblemId;
        break;
    }
    case PublicClass::PST_NOSUCHID:
    {
        tmps = "В таблице " + vl.ProblemTable + " поле " + vl.ProblemField + " в строке " + vl.ProblemId + "ссылается на несуществующий ИД";
        break;
    }
    case PublicClass::PST_INCORRECT:
    {

    }
    }
    return tmps;
}

void SysProblemsDialog::refresh()
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

void SysProblemsDialog::addcol()
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
        UpdateMainTv();
        emit ProblemsNumberUpdated();
    }
    dlg->close(); */
}

void SysProblemsDialog::UpdateProblems()
{
    if (!pc.ExchangeProblemsList.isEmpty())
        NewProblemsDetected();
}
