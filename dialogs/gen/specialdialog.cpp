// Диалог с двумя таблицами - слева главная, справа - подчинённая
// Выбор осуществляется из обеих таблиц - сначала из главной, затем для выбранного пункта главной строится таблица подчинённой
// Главная таблица берётся из списка links (0 - таблица, 1 - поле), подчинённая - по ссылке из главной таблицы
// Пример: links = {"Компоненты описание_сокращ", "Описание"}
// Подчинённая таблица = Значение(alt.description.description)
// Элемент cursel = Значение(alt.description.description).<Значение(Подчинённая таблица).id<Подчинённая таблица>>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QHeaderView>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QDesktopWidget>
//#include <QFileSystemModel>
#include "specialdialog.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqframe.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../models/griddelegate.h"

SpecialDialog::SpecialDialog(QStringList links, QString MainSlaveItemId, QString hdr, QWidget *parent) :
    QDialog(parent)
{
    FirstRun = true;
    if (links.size()<2)
    {
        WARNMSG("");
        return;
    }
    SetupUI(links, hdr, MainSlaveItemId);
}

// в MainSlaveItemId находится совмещённый индекс элемента в виде <T1>.<T2>, где T1 - индекс по первой таблице, T2 - по второй
void SpecialDialog::SetupUI(QStringList links, QString hdr, QString MainSlaveItemId)
{
    QString TableName = links.at(0); // имя таблицы на русском (Компоненты_описание_полн)
    QString TableField = links.at(1); // имя поля на русском (Описание)
    int MainId=0, SlaveId=0;
    QStringList tmpsl = MainSlaveItemId.split(".");
    if (tmpsl.size()>0)
        MainId = tmpsl.at(0).toInt();
    if (tmpsl.size()>1)
        SlaveId = tmpsl.at(1).toInt();
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel(hdr);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    TreeView *MainTV = new TreeView(TreeView::TV_EXPANDABLE, TreeView::TV_PLAIN);
    TreeView *SlaveTV = new TreeView(TreeView::TV_EXPANDABLE, TreeView::TV_PLAIN);
    MainModel = new TreeModel;
    SlaveModel = new TreeModel;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    SlaveTV->horizontalHeader()->setVisible(true);
    SlaveTV->verticalHeader()->setVisible(false);
    connect (MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(MainItemChoosed()));
    connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(SlaveItemChoosed()));
    GridDelegate *GD = new GridDelegate;
    MainTV->setItemDelegate(GD);
    SlaveTV->setItemDelegate(GD);
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;
    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    rlyout->addWidget(SlaveTV);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    QHBoxLayout *hbl = new QHBoxLayout;
    s_tqPushButton *pbOk = new s_tqPushButton("Готово");
    s_tqPushButton *pbCancel = new s_tqPushButton("Отмена");
    connect(pbOk,SIGNAL(clicked()),this,SLOT(SlaveItemChoosed()));
    connect(pbCancel,SIGNAL(clicked()),this,SLOT(close()));
    hbl->addWidget(pbOk);
    hbl->addWidget(pbCancel);
    lyout->addLayout(hbl);
    setLayout(lyout);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MainModel->Setup(TableName); // подготовили главную таблицу
    MainTV->setModel(MainModel);
//    MainTV->resizeRowsToContents();
    SlaveTV->setModel(SlaveModel);
    // установить текущий элемент основной таблицы
    FindAndSetId(MainTV, QString::number(MainId));
    // заполним второстепенную таблицу в зависимости от переданного id
    // найдём реальное наименование столбца таблицы (tablefields) по ссылке
/*    tmpsl = tfl.TableFields(TableName,TableField);
    if (tmpsl.size()>1)
        MainTableFieldInRus = tmpsl.at(1);
    else
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    } */
    QString tmps = "Наименование";
    tfl.TableFields(TableName, tmps, tmpsl); // берём table, tablefields, links из tablefields таблицы links.at(0) с headers="Наименование"
    if (tfl.result == TFRESULT_ERROR)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    }
    MainTableField = tmpsl.at(1); // сохраняем имя колонки для последующего использования (descriptionfull)
    MainDb = tmpsl.at(0).split(".").at(0); // table = <db>.<tble> (alt)
    MainTble = tmpsl.at(0).split(".").at(1); // (description)
    MainItemChoosed(); // принудительно вызываем слот для отображения подчинённой таблицы
    // установить текущий элемент дополнительной таблицы
    FindAndSetId(SlaveTV, QString::number(SlaveId));
    QApplication::restoreOverrideCursor();
}

void SpecialDialog::FindAndSetId(TreeView *tv, QString id)
{
    int IdFound = -1, CurRow = 0;
    TreeModel *mdl = static_cast<TreeModel *>(tv->model());
    while ((IdFound == -1) && (CurRow < mdl->rowCount()))
    {
        if (TvData(tv, CurRow, 0) == id)
            IdFound = CurRow;
        CurRow++;
    }
    if (IdFound == -1)
        IdFound = 0;
    tv->setCurrentIndex(mdl->index(IdFound, 0, QModelIndex()));
}

QString SpecialDialog::TvData(TreeView *tv, int row, int column)
{
    QString tmpString = tv->model()->index(row, column, QModelIndex()).data(Qt::DisplayRole).toString();
    QStringList tmpsl = tmpString.split(".");
    if (tmpsl.size()>1) // если составной ИД
        tmpString = tmpsl.at(1);
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    tmpString.remove(QChar(0xFFFF));
    return tmpString;

}

void SpecialDialog::Refresh()
{
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if ((MainTV == 0) || (SlaveTV == 0))
    {
        DBGMSG;
        return;
    }
    ResizeTv(MainTV);
    ResizeTv(SlaveTV);
}

void SpecialDialog::Setup()
{
//    QStringList tmpsl = tfl.TableFields(links.at(0),links.at(1));
}

void SpecialDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void SpecialDialog::showEvent(QShowEvent *e)
{
    if (FirstRun)
    {
        Refresh();
        FirstRun = false;
    }
    e->accept();
}

void SpecialDialog::MainItemChoosed()
{
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if ((MainTV == 0) || (SlaveTV == 0))
    {
        DBGMSG;
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString MainID = TvData(MainTV, MainTV->currentIndex().row(), 0);
    QString sltble = sqlc.GetValueFromTableByField(MainDb,MainTble,MainTableField,"id"+MainTble,MainID); // берём из главной таблицы значение по полю "Наименование", для которого сохранённое tablefield равен текущему элементу таблицы
    if (sqlc.result)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    }
    QStringList sl = QStringList() << "id" << "PartNumber";
    if (SlaveModel->SetupRaw(MainDb, sltble, sl))
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    }
//    SlaveTV->resizeRowsToContents();
    Refresh();
    QApplication::restoreOverrideCursor();
}

void SpecialDialog::SlaveItemChoosed()
{
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    TreeView *SlaveTV = this->findChild<TreeView *>("SlaveTV");
    if ((MainTV == 0) || (SlaveTV == 0))
    {
        DBGMSG;
        return;
    }
    QString MainId = TvData(MainTV, MainTV->currentIndex().row(), 0);
    QString SlaveId = TvData(SlaveTV, SlaveTV->currentIndex().row(), 0);

    emit finished(MainId+"."+SlaveId);
    this->close();
}

void SpecialDialog::ResizeTv(TreeView *tv)
{
    tv->resizeRowsToContents();
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
