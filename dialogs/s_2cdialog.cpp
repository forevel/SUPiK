#include "s_2cdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqwidget.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"

#include <QSortFilterProxyModel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

// Диалог, состоящий из двух столбцов
// Предназначен для организации списков выбора либо для редактирования полей различных таблиц
// Списки выбора представляют собой таблицы (не деревья! для деревьев есть s_2ctdialog)
// Режим задаётся в Mode (MODE_CHOOSE для режима выбора, MODE_EDIT - для режима редактирования и MODE_EDITNEW для режима редактирования нового элемента)
// В режиме выбора пользователь имеет возможность только выбрать из предлагаемой таблицы одно значение (строку), и слот
// accepted() вернёт в сигнале "datachanged" значение нулевой колонки по выбранной строке
// В режиме редактирования диалог представляет в первой колонке имена полей, а во второй - значения, причём значения
// редактируются в зависимости от выбранного делегата. В этом случае слот accepted() осуществляет запись в базу новых значений
// В режиме MODE_EDITNEW при закрытии диалога удаляется новосозданный элемент, если была нажата кнопка отмены
// режим редактирования карантинных таблиц - особый, для него accepted() должен вызывать специальный обработчик

s_2cdialog::s_2cdialog(QString caption, QWidget *parent) :
    QDialog(parent)
{
    Cancelled = false;
    this->caption = caption;
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_2cdialog::setup(QString tble, int Mode, QString id, bool isQuarantine)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    this->tble.clear();
    this->tble.append(tble);
    this->Mode = Mode;
    this->Id = id;
    setupUI();
    if (MainModel->Setup(tble, id))
    {
        QApplication::restoreOverrideCursor();
        CD2WARN;
        return;
    }
    FillHeaderData();
    this->IsQuarantine = isQuarantine;
    result = 0;
    QApplication::restoreOverrideCursor();
}

void s_2cdialog::SetupRaw(QString db, QString tble, int Mode, QString id)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    this->tble.clear();
    this->tble.append(tble);
    this->Mode = Mode;
    this->Id = id;
    this->Db = db;
    setupUI();
    if (MainModel->SetupRaw(db, tble, id))
    {
        QApplication::restoreOverrideCursor();
        CD2WARN;
        return;
    }
    FillHeaderData();
    result = 0;
    QApplication::restoreOverrideCursor();
}

void s_2cdialog::SetupFile(QString Filename, QString StringToFind, QString str)
{
}

void s_2cdialog::setupUI()
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(cancelled()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setText(caption);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0, Qt::AlignRight);
    mainLayout->addLayout(hlyout);
    QHBoxLayout *pbLayout = new QHBoxLayout;
    s_tqTableView *mainTV = new s_tqTableView; // autoResize = true
    mainTV->setObjectName("mainTV");
    s_duniversal *uniDelegate = new s_duniversal;
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    pbOk->setIcon(QIcon(":/res/ok.png"));
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    pbCancel->setIcon(QIcon(":/res/cross.png"));
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    MainModel = new EditModel;
    connect(MainModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV(QModelIndex,QModelIndex)));
    mainTV->setModel(MainModel);
    mainTV->setSortingEnabled(true);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->verticalHeader()->setVisible(false);
    mainTV->horizontalHeader()->setVisible(false);
    mainTV->setItemDelegate(uniDelegate);
    mainTV->resizeColumnsToContents();
    mainTV->resizeRowsToContents();
    connect (mainTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accepted(QModelIndex)));
    if ((Mode == MODE_CHOOSE) || (Mode == MODE_CHOOSE_RAW))
    {
        QHBoxLayout *hlyout = new QHBoxLayout;
        s_tqPushButton *pb = new s_tqPushButton;
        pb->setIcon(QIcon(":/res/lupa.gif"));
        pb->setToolTip("Поиск");
        connect(pb,SIGNAL(clicked()),this,SLOT(ShowFilterLineEdit()));
        hlyout->addWidget(pb);
        pb = new s_tqPushButton;
        pb->setToolTip("Сбросить фильтр");
        pb->setIcon(QIcon(":/res/crossgray.png"));
        connect(pb,SIGNAL(clicked()),this,SLOT(Unfilter()));
        hlyout->addWidget(pb);
        pb = new s_tqPushButton;
        pb->setToolTip("Создать новый элемент");
        pb->setIcon(QIcon(":/res/newdocy.png"));
        connect(pb,SIGNAL(clicked()),this,SLOT(AddItem()));
        hlyout->addWidget(pb);
        hlyout->addStretch(1);
        mainLayout->addLayout(hlyout);
    }
    mainLayout->addWidget(mainTV);
    mainLayout->setAlignment(mainTV, Qt::AlignLeft);
    mainLayout->addStretch(100);
    mainLayout->addLayout(pbLayout);
    setLayout(mainLayout);
}

void s_2cdialog::AddItem()
{
    if (Mode == MODE_CHOOSE)
    {
        QString tmptble = tble.at(0);
        tmptble.remove("_полн");
        tmptble.remove("_сокращ");
        QString Caption = tmptble;
        tmptble.append("_полн");
        QString newID;
        tfl.insert(tmptble, newID);
        if (tfl.result == TFRESULT_ERROR)
        {
            CD2WARN;
            return;
        }
        tfl.idtois(tmptble,QStringList("ИД"),QStringList(newID)); // добавление полей idpers, deleted, date
        if (tfl.result != TFRESULT_ERROR)
        {
            s_2cdialog *newdialog = new s_2cdialog(Caption);
            newdialog->setup(tmptble, MODE_EDITNEW, newID);
            if (!newdialog->result)
            {
                newdialog->setModal(true);
                newdialog->exec();
                Update();
            }
            else
            {
                CD2WARN;
                return;
            }
        }
        else
            CD2WARN;
    }
    else if (Mode == MODE_CHOOSE_RAW)
    {
        QString newid = sqlc.InsertValuesToTable(Db, tble.at(0), QStringList(), QStringList()); // вставка новой пустой строки
        if (sqlc.result)
        {
            CD2WARN;
            return;
        }
        QStringList tmptablefields = QStringList() << "date" << "idpers";
        QStringList tmpvalues = QStringList() << pc.DateTime << QString::number(pc.idPers);
        sqlc.UpdateValuesInTable(Db, tble.at(0), tmptablefields, tmpvalues, "id"+tble.at(0), newid);
        if (sqlc.result)
        {
            CD2WARN;
            return;
        }
        s_2cdialog *newdialog = new s_2cdialog("");
        newdialog->SetupRaw(Db, tble.at(0), MODE_EDITNEW_RAW, newid);
        if (!newdialog->result)
        {
            newdialog->setModal(true);
            newdialog->exec();
            Update();
        }
        else
        {
            CD2WARN;
            return;
        }
    }
}

void s_2cdialog::Update()
{
/*    MainModel->Setup(tble.at(0));
    if (mainmodel->result)
    {
        CD2WARN;
        return;
    } */
}

void s_2cdialog::AddTable(QString tble)
{
/*    this->tble.append(tble);
    MainModel->Add(tble);
    if (MainModel->result)
    {
        CD2WARN;
        return;
    }
    result = 0;
    return; */
}

void s_2cdialog::resizemainTV(QModelIndex, QModelIndex)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    if (tv == 0)
    {
        CD2DBG;
        return;
    }
    tv->resizeColumnsToContents();
    int wdth = 0;
    for (int i = 0; i < tv->model()->columnCount(); i++)
        wdth += tv->columnWidth(i);
    tv->setMinimumWidth(wdth+10);
    int hgth = 0;
    for (int i = 0; i < tv->model()->rowCount(); i++)
        hgth += tv->rowHeight(i);
    int tmpi = QApplication::desktop()->availableGeometry().height()-350; // -350 - чтобы высота диалога не выходила за пределы видимой части экрана
    hgth = (hgth > tmpi) ? tmpi : hgth;
    tv->setMinimumHeight(hgth+10);
}

void s_2cdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void s_2cdialog::accepted(QModelIndex idx)
{
    Q_UNUSED(idx);
    if ((Mode == MODE_CHOOSE) || (Mode == MODE_CHOOSE_RAW))
        accepted();
}

void s_2cdialog::accepted()
{
    QStringList headers = MainModel->Headers();
    QStringList values = MainModel->Values();
    if ((Mode == MODE_EDIT) || (Mode == MODE_EDITNEW) || (Mode == MODE_CHOOSE))
    {
        QString oldtble, oldid, newid, tmps;
        int tmph;
        if (IsQuarantine)
        {
            oldtble = tble.at(0);
            QString newtble = oldtble;
            tmph = headers.indexOf("ИД");
            if (tmph != -1)
            oldid = values.at(tmph);
            newtble.remove(" карантин"); // убираем " карантин", т.к. пишем в некарантинную таблицу
            tfl.insert(newtble, newid);
            values.replace(tmph, newid); // создаём новую запись в некарантинной таблице
            tble.replace(0, newtble); // подготовка к следующему оператору
        }
        tmps = tble.at(0);
        tfl.idtoisv(tmps, headers, values);
        if (tfl.result == TFRESULT_ERROR)
        {
            CD2WARN;
            return;
        }
        CD2INFO("Записано успешно!");
        if (IsQuarantine)
        {
            tfl.remove(oldtble, oldid); // при успешной записи в некарантин, из карантина старую надо удалить
            if (tfl.result == TFRESULT_ERROR)
            {
                CD2WARN;
                return;
            }
        }
    }
    else // MODE_EDIT_RAW & MODE_EDITNEW_RAW & MODE_CHOOSE_RAW
    {
        sqlc.UpdateValuesInTable(Db, tble.at(0), headers, values, "id"+tble.at(0), Id);
        if (sqlc.result)
        {
            CD2WARN;
            return;
        }
        CD2INFO("Записано успешно!");
    }
    emit changeshasbeenMade(QString());
    this->close();
}

void s_2cdialog::cancelled()
{
    Cancelled = true;
    this->close();
}

void s_2cdialog::closeEvent(QCloseEvent *e)
{
    if (Cancelled && (Mode == MODE_EDITNEW)) // для ввода нового при нажатии отмены - удалить из БД текущую запись
    {
        QString tmps = tble.at(0);
        tfl.Delete(tmps, Id);
    }
    e->accept();
}

void s_2cdialog::FillHeaderData()
{
    if ((Mode == MODE_CHOOSE_RAW) || (Mode == MODE_EDITNEW_RAW) || (Mode == MODE_EDIT_RAW))
        return;
    int i;
    int ftype;
    for (i=0;i<MainModel->rowCount();i++)
    {
        QStringList tmpStringList = MainModel->data(MainModel->index(i,1,QModelIndex()),Qt::UserRole).toString().split(".");
        if (tmpStringList.size() > 1)
            ftype = tmpStringList.at(1).toInt();
        QString hdradd = MainModel->data(MainModel->index(i,0,QModelIndex()),Qt::DisplayRole).toString();
        switch (ftype)
        {
        case FW_ALLINK:
        case FW_LINK:
        {
            MainModel->setData(MainModel->index(i,0,QModelIndex()),QVariant(caption+":"+hdradd),EditModel::HeaderTextRole);
            break;
        }
        default:
            break;
        }
    }
}
