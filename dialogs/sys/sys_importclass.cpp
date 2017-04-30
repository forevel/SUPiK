#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
//#include <QFileDialog>
#include <QHeaderView>
#include <QPaintEvent>
#include <QPainter>
#include <QThread>
#include <QStringListModel>
#include <QTableWidget>
#include <QtXlsx/xlsxdocument.h>
#include "sys_importclass.h"
#include "../../threads/sys_importclass_t.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../gen/s_tablefields.h"
#include "../gen/messagebox.h"

sys_ImportClass::sys_ImportClass(QWidget *parent) : QDialog(parent)
{
    MaxXLSColumn = 0;
    XLSMap = new QMap<QString,QString>();
    XLSMap->clear();
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void sys_ImportClass::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));

    event->accept();
}

void sys_ImportClass::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    QTableWidget *xlstv = new QTableWidget;
    xlstv->setAttribute(Qt::WA_TranslucentBackground);
    xlstv->setObjectName("xlstv");
    xlstv->verticalHeader()->setVisible(false);
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(CancelPBPressed()));
    hlyout->addWidget(pb);
    hlyout->addStretch(10);
    lyout->addLayout(hlyout);

    hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Выберите файл:");
    s_tqChooseWidget *cw = new s_tqChooseWidget;
    cw->setObjectName("loadfilecw");
    cw->Setup("2.15..XLSX Documents (*_xlsx)");
    hlyout->addWidget(lbl,0);
    hlyout->addWidget(cw,5);
    pb = new s_tqPushButton("Загрузить файл");
    connect(pb,SIGNAL(clicked()), this, SLOT(LoadAndCheckFile()));
    pb->setToolTip("XLSX-файл должен удовлетворять следующим условиям:\n"
                   "1. Кроме данных в файле не должно быть никаких посторонних записей типа заголовков;\n"
                   "2. Первая строка файла не должна быть пустой, первый столбец не должен быть пустой;\n"
                   "3. Пустая строка свидетельствует об окончании записей, все остальные строки игнорируются. То же по столбцам.");
    hlyout->addWidget(pb,0);
    lyout->addLayout(hlyout);

    lyout->addWidget(xlstv, 10);

    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Выберите таблицу, куда импортировать данные: ");
    hlyout->addWidget(lbl);
    QStringList vls;
    vls = sqlc.GetValuesFromTableByColumn("sup", "tablefields", "tablename", "tablename");
    vls.removeDuplicates();
    s_tqComboBox *tcb = new s_tqComboBox;
    tcb->setObjectName("fwlinkcb");
    QStringListModel *tml = new QStringListModel;
    tml->setStringList(vls);
    tcb->setModel(tml);
    connect(tcb,SIGNAL(currentTextChanged(QString)),this,SLOT(TableChoosed(QString)));
    hlyout->addWidget(tcb,10);
    lyout->addLayout(hlyout);

    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поставить в соответствие:");
    hlyout->addWidget(lbl);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setObjectName("xlscb");
    hlyout->addWidget(cb,2);
    lbl = new s_tqLabel("-му столбцу файла XLS поле ");
    hlyout->addWidget(lbl);
    cb = new s_tqComboBox;
    cb->setObjectName("tablecb");
    hlyout->addWidget(cb,10);
    lbl = new s_tqLabel(" таблицы в БД или значение по умолчанию:");
    hlyout->addWidget(lbl);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("defaultle");
    hlyout->addWidget(le);
    pb = new s_tqPushButton("Выполнить");
    connect(pb,SIGNAL(clicked()),this,SLOT(MakeConnection()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);
    QTableWidget *connectiontv = new QTableWidget;
    connectiontv->setAttribute(Qt::WA_TranslucentBackground);
    connectiontv->setObjectName("connectiontv");
    connectiontv->verticalHeader()->setVisible(false);
    lyout->addWidget(connectiontv, 10);

    hlyout = new QHBoxLayout;
    s_tqCheckBox *chb = new s_tqCheckBox;
    chb->setText("Дерево");
    chb->setObjectName("treechb");
    QString tmps = "1. В импортируемом файле для структуры дерева необходимо в первом столбце\n"
            "иметь числа в виде: 01 010 011 012 013 0131 0132 и т.д.\n"
            "2. Принадлежность потомков родителям определяется по началу чисел (01->013->0132)\n"
            "3. Задавать соответствие первого столбца не требуется";
    chb->setToolTip(tmps);
    hlyout->addWidget(chb);
    hlyout->addStretch(10);
    pb = new s_tqPushButton("Начать импорт");
    pb->setIcon(QIcon(":/res/import.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(ImpExpPBPressed()));
//    hlyout->addStretch(10);
    hlyout->addWidget(pb);
//    hlyout->addStretch(10);
    lyout->addLayout(hlyout);
    setLayout(lyout);
}

void sys_ImportClass::ImpExpPBPressed()
{
    s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("treechb");
    if (chb == 0)
    {
        DBGMSG;
        return;
    }
    sys_ImportClass_T *t = new sys_ImportClass_T;
    sys_ImportClass_T::imp_struct InitialData;
    if ((filename.isEmpty()) || (XLSMap->isEmpty()))
    {
        WARNMSG("");
        return;
    }
    InitialData.filename = filename;
    InitialData.istree = chb->isChecked();
    InitialData.map = XLSMap;
    InitialData.tablename = tblename;
    t->Set(InitialData);
    WWidget = new WaitWidget;
    WWidget->setObjectName("waitwidget");
    QThread *thr = new QThread;
    t->moveToThread(thr);
    connect(t,SIGNAL(RowProcessing(QString)),WWidget,SLOT(SetMessage(QString)));
    connect(thr,SIGNAL(started()),t,SLOT(Run()));
    connect(thr,SIGNAL(finished()),t,SLOT(deleteLater()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    connect(t,SIGNAL(ProcessFinished()),WWidget,SLOT(Stop()));
    connect(WWidget,SIGNAL(finished()),this,SLOT(Done()));
    thr->start();
    WWidget->Start();
}

void sys_ImportClass::LoadAndCheckFile()
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("loadfilecw");
    QTableWidget *tv = this->findChild<QTableWidget *>("xlstv");
    if ((cw == 0) || (tv == 0))
    {
        DBGMSG;
        return;
    }
    filename = cw->Value();
    QXlsx::Document xlsx(filename.toUtf8());
    int row = 1;
    MaxXLSColumn = 10;
    while (row < 10)
    {
        if (tv->rowCount() < (row))
            tv->setRowCount(row);
        int j = 10; // 'A'
        QString readString = "12345"; // произвольный набор, чтобы только не пустая строка
        while ((readString != "") || (j < MaxXLSColumn))
        {
            QString tmpString = QString::number(j, 36).toUpper(); // from 'A' to 'Z', ограничение по столбцам - до Z
            tmpString += QString::number(row, 10); // формируем номер ячейки
            readString = xlsx.read(tmpString).toString();
            if (!readString.isEmpty())
            {
                QTableWidgetItem *item = new QTableWidgetItem(readString);
                if (tv->columnCount() < (j-9))
                    tv->setColumnCount(j-9);
                tv->setItem(row-1,j-10,item);
            }
            j++;
        }
        if (j > MaxXLSColumn)
        {
            MaxXLSColumn = j;
            if (row > 1) // надо перестроить все предыдущие строки
                row = 0; // начинаем с единицы (следующий оператор ++)
        }
        row++;
    }
    // обновим количество столбцов в комбобоксе xlscb
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("xlscb");
    if (cb == 0)
    {
        DBGMSG;
        return;
    }
    QStringList sl;
    QStringListModel *slm = new QStringListModel;
    for (int i=10; i<(MaxXLSColumn-1); i++)
        sl << QString::number(i-9);
    slm->setStringList(sl);
    cb->setModel(slm);
}

void sys_ImportClass::TableChoosed(QString tble)
{
    // 1. Прочитать структуру таблицы
    // 2. Заполнить структурой комбобокс tablecb
    QStringList sl;
    QString field = "header";
    tfl.TableColumn(tble, field, sl);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    tblename = tble;
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("tablecb");
    if (cb == 0)
    {
        DBGMSG;
        return;
    }
    QStringListModel *slm = new QStringListModel;
    slm->setStringList(sl);
    cb->setModel(slm);
}

void sys_ImportClass::MakeConnection()
{
    s_tqComboBox *tablecb = this->findChild<s_tqComboBox *>("tablecb");
    s_tqComboBox *xlscb = this->findChild<s_tqComboBox *>("xlscb");
    QTableWidget *conntw = this->findChild<QTableWidget *>("connectiontv");
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("defaultle");
    if ((tablecb == 0) || (xlscb == 0) || (conntw == 0) || (le == 0))
    {
        DBGMSG;
        return;
    }
    if (tablecb->count() == 0)
    {
        WARNMSG("");
        return;
    }
    QString xlscbitem;
    if (!le->text().isEmpty())
        xlscbitem = "_"+le->text();
    else if (xlscb->count() != 0)
    {
        xlscbitem = xlscb->currentText();
        xlscb->removeItem(xlscb->currentIndex());
    }
    else
    {
        WARNMSG("");
        return;
    }
    QString tablecbitem = tablecb->currentText();
    int currow = conntw->rowCount();
    conntw->setRowCount(currow+1); // добавляем строку в таблицу
    if (conntw->columnCount() < 2)
        conntw->setColumnCount(2);
    QTableWidgetItem *item = new QTableWidgetItem(xlscbitem);
    conntw->setItem(currow,0,item);
    item = new QTableWidgetItem(tablecbitem);
    conntw->setItem(currow,1,item);
    XLSMap->insert(tablecbitem, xlscbitem);
    // удаляем соотв. элементы из обоих комбобоксов, чтобы не было дублирующихся строк
    tablecb->removeItem(tablecb->currentIndex());
    le->clear();
}

void sys_ImportClass::CancelPBPressed()
{
    this->close();
}

void sys_ImportClass::Done()
{
    WWidget->close();
    MessageBox2::information(this,"Успешно!","Импорт завершён успешно!");
    this->close();
}
