#include "../inc/s_2cdialog.h"
#include "../inc/s_duniversal.h"
#include "../inc/s_tqtableview.h"
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_tqlabel.h"
#include "../inc/s_sql.h"
#include "../inc/publicclass.h"

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
// Режим задаётся в публичной переменной Mode (MODE_CHOOSE для режима выбора и MODE_EDIT - для режима редактирования)
// В режиме выбора пользователь имеет возможность только выбрать из предлагаемой таблицы одно значение (строку), и слот
// accepted() вернёт в сигнале "datachanged" значение нулевой колонки по выбранной строке
// В режиме редактирования диалог представляет в первой колонке имена полей, а во второй - значения, причём значения
// редактируются в зависимости от выбранного делегата. В этом случае слот accepted() осуществляет запись в базу новых значений
//
// Принцип работы с диалогом:
// s_2cdialog *dlg = new s_2cdialog (hdr); // hdr - заголовок диалога в caption
// dlg.Mode = MODE_CHOOSE; // выбор режима работы
// dlg.IsQuarantine = true; // режим редактирования карантинных таблиц - особый, для него accepted() должен вызывать специальный обработчик
// if (!(res = dlg.setup(table, id))) // если формирование диалога прошло нормально
//      dlg.exec(); // вызов диалога

s_2cdialog::s_2cdialog(QString hdr, QWidget *parent) :
    QDialog(parent)
{
    DialogIsNeedToBeResized = false;
    IsQuarantine = false;
    Mode = MODE_CHOOSE;
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    QSizePolicy fixed(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(fixed);
    setupUI(hdr);
}

void s_2cdialog::setupUI(QString hdr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *pbLayout = new QHBoxLayout;
    s_tqTableView *mainTV = new s_tqTableView(true); // autoResize = true
    mainTV->setObjectName("mainTV");
    mainmodel = new s_ncmodel;
    s_duniversal *uniDelegate = new s_duniversal;
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setText(hdr);
    this->hdr=hdr;
    QFont font;
    font.setPointSize(10);
    lbl->setFont(font);
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
    pmainmodel = new QSortFilterProxyModel;
    pmainmodel->setSourceModel(mainmodel);
    mainTV->setModel(pmainmodel);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->verticalHeader()->setVisible(false);
    mainTV->horizontalHeader()->setVisible(false);
    mainTV->setItemDelegate(uniDelegate);
    mainLayout->addWidget(lbl, 0, Qt::AlignRight);
    mainLayout->addWidget(mainTV, 100, Qt::AlignLeft);
    mainLayout->addLayout(pbLayout);
    constheight=lbl->minimumSizeHint().height()+pbOk->minimumSizeHint().height();
    setLayout(mainLayout);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    connect(mainTV, SIGNAL(datachanged()), this, SLOT(updatedialogsize()));
//    connect(mainTV, SIGNAL(datachanged()), this, SLOT(resizemainTV()));
}

// процедура заполняет модель значениями из двух списков (sl1, sl2). При этом в списке links находятся вспомогательные поля, соответствующие по индексам
// значениям в списке sl2. Значения из списка sl1 отображаются как обычные надписи.
// Замена для s_sqlfieldsdialog

void s_2cdialog::setup(QStringList sl1, QStringList links1, QStringList sl2, QStringList links2)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
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
    fillModelAdata();
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tv->resizeColumnsToContents();
    DialogIsNeedToBeResized = true;
}

// процедура подготавливает диалог из одного столбца, links содержит список
// вспомогательных полей для каждого из элементов столбца. Подходит для
// организации списка выбора

void s_2cdialog::setup(QStringList sl, QStringList links, QString str)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
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
    fillModelAdata();
 //   QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(str), 1, Qt::MatchExactly);
 //   if (!item.isEmpty())
 //       tv->setCurrentIndex(item.at(0));
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tv->resizeColumnsToContents();
    DialogIsNeedToBeResized = true;
}

// процедура подготавливает диалог из столбцов таблицы tble в tablefields

int s_2cdialog::setupchoosetable(QString tble, QString id)
{
    try
    {
        this->tble = tble; // в слоте accepted() надо знать, с какой таблицей мы работаем
        s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
        if (tv == 0)
            throw 0x51;
        int res = mainmodel->setup(tble);
        if (res)
            throw res;
/*        QList<QStringList> lsl;
        QStringList fl;
        int i;
        fl << "tablefields" << "table" << "headers" << "links";
        lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", tble, "fieldsorder", true);
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Проблемы с получением значений по таблице "+tble+" из таблицы tablefields");
            return 0x01;
        }
        fl.clear();
        tble = lsl.at(0).at(1); // имя таблицы в формате db.tble
        for (i = 0; i < lsl.size(); i++)
        {
            mainmodel->addColumn(lsl.at(i).at(2));
            mainmodel->setcolumnlinks(i, lsl.at(i).at(3));
            fl << lsl.at(i).at(0); // поля таблицы db.tble
        }
        QString db = tble.split(".").at(0);
        tble = tble.split(".").at(1);
        lsl=sqlc.getvaluesfromtablebycolumns(sqlc.getdb(db), tble, fl);
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Проблемы с получением значений по таблице "+tble);
            return 0x02;
        }
        mainmodel->fillModel(lsl); */
        QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(id), 1, Qt::MatchExactly);
        if (!item.isEmpty())
            tv->setCurrentIndex(item.at(0));
        tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tv->resizeColumnsToContents();
        DialogIsNeedToBeResized = true;
        return 0;
    }
    catch (int res)
    {
        ShowMessage(res);
        return res;
    }

}

// процедура подготавливает диалог заполнения полей таблицы tble по строке id

int s_2cdialog::setup(QString tble, QString id)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    QList<QStringList> lsl;
    QStringList fl;
    QStringList headersvl;
    QStringList tmpStringlist, overallStringList;
    QString keydb, keytble;
    int keyidx = -1;
    int i;
    fl << "tablefields" << "table" << "headers" << "links" << "keyfield";
    lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", tble, "fieldsorder", true);
    if (sqlc.result)
    {
        QMessageBox::warning(this, "warning!", "Проблемы с получением значений по таблице "+tble+" из таблицы tablefields");
        return 1;
    }
    QStringList links, headers;
    fl.clear();
    links.clear();
    headers.clear();
    for (i = 0; i < lsl.size(); i++)
    {
        links << lsl.at(i).at(3);
        headers << lsl.at(i).at(2);
        if (lsl.at(i).at(4) == "v")
        {
            keydb = lsl.at(i).at(1).split(".").at(0);
            keytble = lsl.at(i).at(1).split(".").at(1);
            keyidx = i;
        }
    }
    if (keyidx == -1)
    {
        QMessageBox::warning(this, "warning!", "Отсутствует ключевое поле по таблице "+tble+" в таблице tablefields");
        return 4;
    }
    if (id != "0") // если требуется изменить элемент
    {
        QStringList curfl;
        overallStringList.clear();
        headersvl.clear();
        while (lsl.size() > 0)
        {
            curfl.clear();
            QString curtble = lsl.at(0).at(1);
            for (i = 0; i < lsl.size(); i++)
            {
                if (lsl.at(i).at(1) == curtble)
                {
                    curfl << lsl.at(i).at(0); // в curfl находятся только те поля, которые относятся к текущей таблице
                    lsl.removeAt(i);
                    headersvl << headers.at(i);
                    headers.removeAt(i);
                    i--;
                }
            }
            tmpStringlist = curtble.split(".");
            int idx = curfl.indexOf("id"+tmpStringlist.at(1));
            if (idx != -1)
                curfl.swap(0, idx);
            // считываем все данные из таблицы
            QSqlQuery get_child_from_db2 (sqlc.getdb(tmpStringlist.at(0)));
            QString tmpString = "SELECT ";
            for (i = 0; i < curfl.count(); i++)
                tmpString += "`" + curfl.at(i) + "`,";
            tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
            tmpString += " FROM `"+tmpStringlist.at(1)+"` WHERE `id"+keytble+"`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+tmpStringlist.at(1)+"` ASC;";
            get_child_from_db2.exec(tmpString);

            while (get_child_from_db2.next())
            {
                tmpStringlist.clear();
                for (i = 0; i < curfl.size(); i++)
                    tmpStringlist << get_child_from_db2.value(i).toString();
            }
            overallStringList.append(tmpStringlist);
        }
        if (overallStringList.isEmpty())
        {
            QMessageBox::warning(this, "warning!", "Проблемы с получением значений из таблицы "+tble+" по полю №"+id);
            return 3; // проблемы с получением данных по нужному id
        }
    }
    else
    {
        for (i = 0; i < lsl.size(); i++)
            overallStringList << "";
        overallStringList.replace(keyidx, QString::number(sqlc.getnextfreeindex(sqlc.getdb(keydb), keytble)));
    }

    lsl.clear();
    lsl.append(headersvl);
    lsl.append(overallStringList);
    QList<int> il;
    for (i = 0; i < lsl.size(); i++)
        il << lsl.at(i).size();
    mainmodel->prepareModel(il);
    mainmodel->setcolumnlinks(0, "0.8"); // простые поля для первого столбца
    mainmodel->setcolumnlinks(1, links); // какие записаны в таблице tablefields поля - для второго столбца
    mainmodel->fillModel(lsl);
    fillModelAdata();
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tv->resizeColumnsToContents();
    DialogIsNeedToBeResized = true;

/*
    // в fl - наименования полей
    // в ll - ссылки на другие таблицы
    // в vl - значения полей

        case FT_INDIRECT:
        {
            // сначала берём ссылку из текущей таблицы
            tmpString = tmpString.right(tmpString.size()-1); // убираем точку, получаем имя столбца в текущей таблице
            int idx = sl.indexOf(tmpString); // позиция столбца, на который ссылается текущий столбец
            tmpString = ll.at(idx); // получаем ссылку по ссылочному столбцу
            QString tmpdb = tmpString.mid(3, 3);
            QString tmptble = tmpString.right(tmpString.size()-7);
            tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpdb), tmptble, sl.at(i).right(sl.at(i).size()-2), "id"+tmptble, vl.at(idx)); // взяли имя таблицы в БД <sl.at(i).right(sl.at(i).size()-2)>
            fdb.insert(i, sl.at(i).right(sl.at(i).size()-2)); // взяли три буквы БД из наименования столбца (idalt-id=alt)
            ftble.insert(i, tmpString);
            break;
        }
        int res = addPushButtonWithDialog(vl.at(i), i, *GridLayout);
*/
    return 0;
}

void s_2cdialog::resizemainTV()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    tv->resizeColumnsToContents();
//    tv->datachangedintable = true;
    updatedialogsize();
}

void s_2cdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/pic/Pic/2cWallPaper.png"));
    setFixedSize(minimumSizeHint());
    e->accept();
}

void s_2cdialog::accepted()
{
    QString tmpString;
    try
    {
        if (IsQuarantine)
        {
            // отдельная обработка
        }
        if (Mode == MODE_EDIT)
        {
            // для режима редактирования - запись в базу
        }
        else
        {
            s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
            if (tv == 0)
                throw 0x21;
            tmpString = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
        }
        emit changeshasbeenMade(tmpString);
        this->close();
    }
    catch (int res)
    {
        Q_UNUSED(res);
        return;
    }
}

void s_2cdialog::cancelled()
{
    this->close();
}

void s_2cdialog::updatedialogsize()
{
    DialogIsNeedToBeResized = true;
}

QSize s_2cdialog::minimumSizeHint()
{
    if (DialogIsNeedToBeResized)
    {
        int curwidth = QApplication::desktop()->screenGeometry(this).width();
        int curheight = QApplication::desktop()->screenGeometry(this).height();
        int f2 = 0;
        s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
        for (int i = 0; i < tv->horizontalHeader()->count(); i++)
            f2 += tv->columnWidth(i)+20;
        if (f2 > curwidth)
            f2 = curwidth;
        if (f2 < 300) // диалоги слишком узкие нам не нужны
            f2 = 300;
        int f1 = constheight+tv->minimumSizeHint().height();
        if (f1>curheight)
            f1 = curheight;
        DialogIsNeedToBeResized = false;
//        return QSize(f2, this->size().height());
        return QSize(f2,f1);
    }
    else
        return this->size();
}

void s_2cdialog::sortModel()
{
    pmainmodel->sort(0, Qt::AscendingOrder);
}

void s_2cdialog::setTvCurrentText(QString text)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mainTV");
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchExactly);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
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
            mainmodel->setData(mainmodel->index(i,0,QModelIndex()),hdr+":"+hdradd,Qt::UserRole+1);
            break;
        }
        default:
            break;
        }
    }
}

void s_2cdialog::ShowMessage(int ernum)
{
    QMessageBox::warning(this, "warning!", "Ошибка 2CD 0x" + QString::number(ernum, 16), QMessageBox::Ok, QMessageBox::NoButton);
}
