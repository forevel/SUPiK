#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QHeaderView>
#include <QPaintEvent>
#include <QPainter>
#include <QTableWidget>
#include <QtXlsx/xlsxdocument.h>
#include "sys_importclass.h"
#include "../../threads/sys_importclass_t.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

sys_ImportClass::sys_ImportClass(QWidget *parent) : QDialog(parent)
{
    MaxXLSColumn = 0;
    XLSMap.clear();
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
    lbl = new s_tqLabel(" таблицы в БД");
    hlyout->addWidget(lbl);
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
    pb = new s_tqPushButton("Начать импорт");
    pb->setIcon(QIcon(":/res/import.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(ImpExpPBPressed()));
    hlyout->addStretch(10);
    hlyout->addWidget(pb);
    hlyout->addStretch(10);
    lyout->addLayout(hlyout);
    setLayout(lyout);
}

void sys_ImportClass::ImpExpPBPressed()
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("loadfilecw");
    if (cw == 0)
    {
        MessageBox2::information(this,"","Элемент CW не найден");
        return;
    }
    sys_ImportClass_T *t = new sys_ImportClass_T(cw->Value());
    WWidget = new WaitWidget;
    WWidget->setObjectName("waitwidget");
    connect(t,SIGNAL(RowProcessing(QString)),WWidget,SLOT(SetMessage(QString)));
    connect(t,SIGNAL(ProcessFinished()),this,SLOT(SysICTFinished()));
    connect(t,SIGNAL(finished()),t,SLOT(deleteLater()));
    t->start();
    WWidget->Start();
}

void sys_ImportClass::LoadAndCheckFile()
{
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("loadfilecw");
    QTableWidget *tv = this->findChild<QTableWidget *>("xlstv");
    if ((cw == 0) || (tv == 0))
    {
        SIMPDBG;
        return;
    }
    QString filename = cw->Value();
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
        SIMPDBG;
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
    QStringList sl = tfl.TableColumn(tble, "header");
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("tablecb");
    if (cb == 0)
    {
        SIMPDBG;
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
    if ((tablecb == 0) || (xlscb == 0) || (conntw == 0))
    {
        SIMPDBG;
        return;
    }
    QString tablecbitem = tablecb->currentText();
    QString xlscbitem = xlscb->currentText();
    int currow = conntw->rowCount();
    conntw->setRowCount(currow+1); // добавляем строку в таблицу
    if (conntw->columnCount() < 2)
        conntw->setColumnCount(2);
    QTableWidgetItem *item = new QTableWidgetItem(xlscbitem);
    conntw->setItem(currow,0,item);
    item = new QTableWidgetItem(tablecbitem);
    conntw->setItem(currow,1,item);
    // удаляем соотв. элементы из обоих комбобоксов, чтобы не было дублирующихся строк
    tablecb->removeItem(tablecb->currentIndex());
    xlscb->removeItem(xlscb->currentIndex());
}

void sys_ImportClass::CancelPBPressed()
{
    this->close();
}

void sys_ImportClass::SysICTFinished()
{
    WWidget->Stop();
}
