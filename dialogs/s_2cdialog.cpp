#include "s_2cdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_sql.h"

#include <QSortFilterProxyModel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

// Диалог, состоящий из двух столбцов
// Предназначен для организации списков выбора либо для редактирования полей различных таблиц
// Списки выбора представляют собой таблицы (не деревья! для деревьев есть s_2ctdialog)
// Режим задаётся в Mode (MODE_CHOOSE для режима выбора и MODE_EDIT - для режима редактирования)
// В режиме выбора пользователь имеет возможность только выбрать из предлагаемой таблицы одно значение (строку), и слот
// accepted() вернёт в сигнале "datachanged" значение нулевой колонки по выбранной строке
// В режиме редактирования диалог представляет в первой колонке имена полей, а во второй - значения, причём значения
// редактируются в зависимости от выбранного делегата. В этом случае слот accepted() осуществляет запись в базу новых значений
//
// режим редактирования карантинных таблиц - особый, для него accepted() должен вызывать специальный обработчик

s_2cdialog::s_2cdialog(QWidget *parent) :
    QDialog(parent)
{
    setupUI();
}

void s_2cdialog::setup(QString tble, int Mode, QString id, QString caption, QString matchtext, bool isQuarantine)
{
    this->tble.clear();
    this->tble.append(tble);
    this->Mode = Mode;
    this->caption = caption;
    switch (Mode)
    {
    case MODE_CHOOSE:
    {
        result = mainmodel->setup(tble);
        if (result)
        {
            result+=ER_2CDLG+0x11;
            return;
        }
        mainmodel->isEditable = false;
        break;
    }
    case MODE_EDIT:
    {
        result = mainmodel->setup(tble, id);
        if (result)
        {
            result+=ER_2CDLG+0x14;
            return;
        }
        mainmodel->isEditable = true;
        fillModelAdata();
        break;
    }
    default:
    {
        result=ER_2CDLG+0x17;
        return;
    }
    }
    this->IsQuarantine = isQuarantine;
    if (!matchtext.isEmpty())
        SetTvCurrentText(matchtext);
    result=0;
}

// конструктор подготавливает диалог из одного столбца, links содержит список
// вспомогательных полей для каждого из элементов столбца. Подходит для
// организации списка выбора

void s_2cdialog::setup(QStringList sl, QString caption, QStringList links, QString str)
{
    this->tble.clear();
    mainmodel = new s_ncmodel;
    this->caption = caption;
    QList<QStringList> tmpsl;
    tmpsl.append(sl);
    QList<int> il;
    il << sl.size();
    mainmodel->prepareModel(il);
    if (links.isEmpty())
        mainmodel->setcolumnlinks(0, "7.8");
    else
        mainmodel->setcolumnlinks(0, links);
    mainmodel->fillModel(tmpsl);
    if (mainmodel->result)
        ShowErMsg(ER_2CDLG+mainmodel->result+0x21); // не выходим, т.к. проблема м.б. с одной из ячеек, которую можно будет поправить
    fillModelAdata();
    IsQuarantine = false;
    Mode = MODE_CHOOSE;
    SetTvCurrentText(str);
}

void s_2cdialog::setupUI()
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *pbLayout = new QHBoxLayout;
    s_tqTableView *mainTV = new s_tqTableView; // autoResize = true
    mainTV->setObjectName("mainTV");
    s_duniversal *uniDelegate = new s_duniversal;
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    s_tqLabel *lbl = new s_tqLabel;
    mainmodel = new s_ncmodel;
    lbl->setText(caption);
    QFont font;
    font.setPointSize(10);
    lbl->setFont(font);
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
//    pmainmodel = new QSortFilterProxyModel;
  //  pmainmodel->setSourceModel(mainmodel);
    mainTV->setModel(mainmodel);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->verticalHeader()->setVisible(false);
    mainTV->horizontalHeader()->setVisible(false);
    mainTV->setItemDelegate(uniDelegate);
    mainTV->resizeColumnsToContents();
    mainTV->resizeRowsToContents();
    mainLayout->addWidget(lbl, 0, Qt::AlignRight);
    mainLayout->addWidget(mainTV);
    mainLayout->addLayout(pbLayout);
    setLayout(mainLayout);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    connect(mainTV, SIGNAL(datachanged()), this, SLOT(resizemainTV()));
}


// процедура заполняет модель значениями из двух списков (sl1, sl2). При этом в списке links находятся вспомогательные поля, соответствующие по индексам
// значениям в списке sl2. Значения из списка sl1 отображаются как обычные надписи.
// Замена для s_sqlfieldsdialog

/*int s_2cdialog::setup(QStringList sl1, QStringList links1, QStringList sl2, QStringList links2)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    if (tv == 0)
        return(ER_2CDLG+0x01);
    QList<QStringList> sl;
    sl.append(sl1);
    sl.append(sl2);
    QList<int> il;
    il << sl1.size() << sl2.size();
    mainmodel->prepareModel(il);
    if (links1.isEmpty())
        mainmodel->setcolumnlinks(0, "0.8");
    else
        mainmodel->setcolumnlinks(0, links1);
    if (links2.isEmpty())
        mainmodel->setcolumnlinks(1, "7.8");
    else
        mainmodel->setcolumnlinks(1, links2);
    mainmodel->fillModel(sl);
    if (mainmodel->result)
        ShowErMsg(ER_2CDLG+mainmodel->result+0x04); // не выходим, т.к. проблема м.б. с одной из ячеек, которую можно будет поправить
    fillModelAdata();
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tv->resizeColumnsToContents();
    DialogIsNeedToBeResized = true;
    return 0;
}*/

void s_2cdialog::AddTable(QString tble)
{
    this->tble.append(tble);
    result = mainmodel->setup(tble);
    if (result)
    {
        result+=ER_2CDLG+0x11;
        return;
    }
    result = 0;
    return;
}

void s_2cdialog::resizemainTV()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    if (tv == 0)
        return;
    tv->resizeColumnsToContents();
    int wdth = 0;
    for (int i = 0; i < tv->model()->columnCount(); i++)
        wdth += tv->columnWidth(i);
    tv->setMinimumWidth(wdth+10);
    int hgth = 0;
    for (int i = 0; i < tv->model()->rowCount(); i++)
        hgth += tv->rowHeight(i);
    int tmpi = QApplication::desktop()->availableGeometry().height()-150; // -150 - чтобы высота диалога не выходила за пределы видимой части экрана
    hgth = (hgth > tmpi) ? tmpi : hgth;
    tv->setMinimumHeight(hgth+10);
}

void s_2cdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void s_2cdialog::accepted()
{
    QString tmpString, oldtble, oldid, newid;
    tmpString.clear();
    int tmph;
    if (Mode == MODE_EDIT) // для режима редактирования - запись в базу
    {
        QStringList headers = mainmodel->cvalues(0);
        QStringList values = mainmodel->cvalues(1);
        if (IsQuarantine)
        {
            oldtble = tble.at(0);
            QString newtble = oldtble;
            tmph = headers.indexOf("ИД");
            if (tmph != -1)
            oldid = values.at(tmph);
            int posq = newtble.indexOf(" карантин");
            if (posq != -1)
                newtble.remove(posq, 9); // убираем " карантин", т.к. пишем в некарантинную таблицу
            newid = tfl.insert(newtble);
            values.replace(tmph, newid); // создаём новую запись в некарантинной таблице
            tble.replace(0, newtble); // подготовка к следующему оператору
        }
        tfl.idtois(tble.at(0), headers, values);
        if (tfl.result)
        {
            ShowErMsg(ER_2CDLG+0x21+tfl.result);
            return;
        }
        QMessageBox::information(this, "Успешно!", "Записано успешно!");
        if (IsQuarantine)
        {
            tfl.remove(oldtble, oldid); // при успешной записи в некарантин, из карантина старую надо удалить
            if (tfl.result)
            {
                ShowErMsg(ER_2CDLG+0x24+tfl.result);
                return;
            }
        }
    }
    else // список выбора
    {
        s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
        if (tv == 0)
        {
            ShowErMsg(ER_2CDLG+0x27);
            return;
        }
        tmpString = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    }
    emit changeshasbeenMade(tmpString);
    this->close();
}

void s_2cdialog::cancelled()
{
    this->close();
}

void s_2cdialog::sortModel()
{
//    pmainmodel->sort(0, Qt::AscendingOrder);
}

// незакончено - дописать другие варианты

void s_2cdialog::fillModelAdata()
{
    int i;
    int ftype;
    for (i=0;i<mainmodel->rowCount();i++)
    {
        QStringList tmpStringList = mainmodel->data(mainmodel->index(i,1,QModelIndex()),Qt::UserRole).toString().split(".");
        if (tmpStringList.size() > 1)
            ftype = tmpStringList.at(1).toInt();
        QString hdradd = mainmodel->data(mainmodel->index(i,0,QModelIndex()),Qt::DisplayRole).toString();
        switch (ftype)
        {
        case FW_ALLINK:
        case FW_LINK:
        {
            mainmodel->setData(mainmodel->index(i,0,QModelIndex()),QVariant(caption+":"+hdradd),Qt::UserRole+1);
            break;
        }
        default:
            break;
        }
    }
}

void s_2cdialog::ShowErMsg(int ernum)
{
    QMessageBox::warning(this, "warning!", "Ошибка 0x" + QString::number(CD_ERROR + ernum, 16), QMessageBox::Ok, QMessageBox::NoButton);
}

void s_2cdialog::SetTvCurrentText(QString str)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    if (tv == 0)
        return;
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(str), 1, Qt::MatchExactly);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
}
