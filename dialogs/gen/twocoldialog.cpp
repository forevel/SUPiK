#include "twocoldialog.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"

#include <QSortFilterProxyModel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
//#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

// Диалог, состоящий из двух столбцов
// Предназначен для организации списков выбора либо для редактирования полей различных таблиц
// Списки выбора представляют собой таблицы или деревья
// Режим задаётся в Mode (MODE_CHOOSE для режима выбора, MODE_EDIT - для режима редактирования и MODE_EDITNEW для режима редактирования нового элемента)
// В режиме выбора пользователь имеет возможность только выбрать из предлагаемой таблицы одно значение (строку), и слот
// accepted() вернёт в сигнале "datachanged" значение нулевой колонки по выбранной строке
// В режиме редактирования диалог представляет в первой колонке имена полей, а во второй - значения, причём значения
// редактируются в зависимости от выбранного делегата. В этом случае слот accepted() осуществляет запись в базу новых значений
// В режиме MODE_EDITNEW при закрытии диалога удаляется новосозданный элемент, если была нажата кнопка отмены
// режим редактирования карантинных таблиц - особый, для него accepted() должен вызывать специальный обработчик

TwoColDialog::TwoColDialog(QString caption, QWidget *parent) :
    QDialog(parent)
{
    Cancelled = false;
    this->caption = caption;
    setAttribute(Qt::WA_DeleteOnClose);
}

int TwoColDialog::setup(QString tble, int Mode, QString id, bool isQuarantine)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    this->tble.clear();
    this->tble.append(tble);
    this->Mode = Mode;
    this->Id = id;
    MainModel = new EditModel;
    if ((MainModel->Setup(tble, id)) == RESULTBAD)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return RESULTBAD;
    }
    if (!setupUI())
        return RESULTBAD;
    FillHeaderData();
    this->IsQuarantine = isQuarantine;
    QApplication::restoreOverrideCursor();
    return RESULTOK;
}

int TwoColDialog::SetupRaw(QString db, QString tble, int Mode, QString id)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    this->tble.clear();
    this->tble.append(tble);
    this->Mode = Mode;
    this->Id = id;
    this->Db = db;
    MainModel = new EditModel;
    if ((MainModel->SetupRaw(db, tble, id)) == RESULTBAD)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return RESULTBAD;
    }
    if (!setupUI())
        return RESULTBAD;
    FillHeaderData();
    QApplication::restoreOverrideCursor();
    return RESULTOK;
}

int TwoColDialog::SetupFile(QString Filename, QString StringToFind, QString str)
{
    Q_UNUSED(Filename);
    Q_UNUSED(StringToFind);
    Q_UNUSED(str);
    return RESULTOK;
}

bool TwoColDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(cancelled()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel;
    if (caption.size() > MAX_CAPTION_SIZE)
    {
        caption.truncate(MAX_CAPTION_SIZE);
        caption.append("...");
    }
    lbl->setText(caption);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0, Qt::AlignRight);
    mainLayout->addLayout(hlyout);
    QHBoxLayout *pbLayout = new QHBoxLayout;
    TreeView *mainTV = new TreeView;
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
    mainTV->setModel(MainModel);
    mainTV->setSortingEnabled(true);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->setItemDelegate(uniDelegate);
    mainTV->setWordWrap(true);
    mainTV->setTextElideMode(Qt::ElideLeft);
//    mainTV->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mainTV->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
//    mainTV->resizeColumnsToContents();
    mainTV->resizeRowsToContents();
    mainLayout->addWidget(mainTV);
    mainLayout->addLayout(pbLayout);
    setLayout(mainLayout);
    connect(MainModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV()));
    return true;
}

void TwoColDialog::AddItem()
{
    if (Mode == MODE_CHOOSE)
    {
        QString tmptble = tble.at(0);
        tmptble.remove("_полн");
        tmptble.remove("_сокращ");
        QString Caption = tmptble;
        tmptble.append("_полн");
        QString newID;
        tfl.Insert(tmptble, newID);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
            return;
        }
        tfl.Update(tmptble,QStringList("ИД"),QStringList(newID)); // добавление полей idpers, deleted, date
        if (tfl.result != TFRESULT_ERROR)
        {
            TwoColDialog *newdialog = new TwoColDialog(Caption);
            if (newdialog->setup(tmptble, MODE_EDITNEW, newID) != RESULTOK)
            {
                newdialog->setModal(true);
                newdialog->exec();
                Update();
            }
            else
            {
                WARNMSG("");
                return;
            }
        }
        else
            WARNMSG("");
    }
    else if (Mode == MODE_CHOOSE_RAW)
    {
        QString newid = sqlc.InsertValuesToTable(Db, tble.at(0), QStringList(), QStringList()); // вставка новой пустой строки
        if (sqlc.result)
        {
            WARNMSG("");
            return;
        }
        QStringList tmptablefields = QStringList() << "date" << "idpers";
        QStringList tmpvalues = QStringList() << pc.DateTime << QString::number(pc.idPers);
        sqlc.UpdateValuesInTable(Db, tble.at(0), tmptablefields, tmpvalues, "id"+tble.at(0), newid);
        if (sqlc.result)
        {
            WARNMSG("");
            return;
        }
        TwoColDialog *newdialog = new TwoColDialog("");
        if (newdialog->SetupRaw(Db, tble.at(0), MODE_EDITNEW_RAW, newid) != RESULTOK)
        {
            newdialog->setModal(true);
            newdialog->exec();
            Update();
        }
        else
        {
            WARNMSG("");
            return;
        }
    }
}

void TwoColDialog::Update()
{
/*    MainModel->Setup(tble.at(0));
    if (mainmodel->result)
    {
        WARNMSG("");
        return;
    } */
}

void TwoColDialog::AddTable(QString tble)
{
    Q_UNUSED(tble);
/*    this->tble.append(tble);
    MainModel->Add(tble);
    if (MainModel->result)
    {
        WARNMSG("");
        return;
    }
    result = 0;
    return; */
}

void TwoColDialog::resizemainTV()
{
/*    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents(); */
}

void TwoColDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void TwoColDialog::accepted(QModelIndex idx)
{
    Q_UNUSED(idx);
    if ((Mode == MODE_CHOOSE) || (Mode == MODE_CHOOSE_RAW))
        accepted();
}

void TwoColDialog::accepted()
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
            tfl.Insert(newtble, newid);
            values.replace(tmph, newid); // создаём новую запись в некарантинной таблице
            tble.replace(0, newtble); // подготовка к следующему оператору
        }
        tmps = tble.at(0);
        tfl.Updatev(tmps, headers, values);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
            return;
        }
        INFOMSG("Записано успешно!");
        if (IsQuarantine)
        {
            tfl.Remove(oldtble, oldid); // при успешной записи в некарантин, из карантина старую надо удалить
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("");
                return;
            }
        }
    }
    else // MODE_EDIT_RAW & MODE_EDITNEW_RAW & MODE_CHOOSE_RAW
    {
        sqlc.UpdateValuesInTable(Db, tble.at(0), headers, values, "id"+tble.at(0), Id);
        if (sqlc.result)
        {
            WARNMSG("");
            return;
        }
        INFOMSG("Записано успешно!");
    }
    emit changeshasbeenMade(QString());
    this->close();
}

void TwoColDialog::cancelled()
{
    Cancelled = true;
    this->close();
}

void TwoColDialog::closeEvent(QCloseEvent *e)
{
    if (Cancelled && (Mode == MODE_EDITNEW)) // для ввода нового при нажатии отмены - удалить из БД текущую запись
    {
        QString tmps = tble.at(0);
        tfl.Delete(tmps, Id);
    }
    e->accept();
}

void TwoColDialog::FillHeaderData()
{
    if ((Mode == MODE_CHOOSE_RAW) || (Mode == MODE_EDITNEW_RAW) || (Mode == MODE_EDIT_RAW))
        return;
    int i;
    int ftype;
    for (i=0;i<MainModel->rowCount();i++)
    {
        QStringList tmpStringList = MainModel->data(MainModel->index(i,1,QModelIndex()),EditModel::LinksRole).toString().split(".");
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
