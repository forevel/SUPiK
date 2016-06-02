#include "cmp_newsubsection.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include "../../widgets/waitwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../gen/s_tablefields.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../messagebox.h"

cmp_newsubsection::cmp_newsubsection(int CompType, QWidget *parent) : QDialog(parent)
{
    this->CompType = CompType;
    SomethingChanged = false;
    SetupUI();
}

cmp_newsubsection::~cmp_newsubsection()
{

}

void cmp_newsubsection::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/CompWallpaper.jpg"));

    event->accept();
}

void cmp_newsubsection::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Имя категории:");
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("name");
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SetSomethingChanged()));
    connect(le, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Имя в БД:");
    le = new s_tqLineEdit;
    le->setObjectName("tblename");
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SetSomethingChanged()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Группа параметров:");
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->setObjectName("cwpar");
    cw->Setup("2.2..Наборы параметров_сокращ.Наименование","Набор параметров");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SetSomethingChanged()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(cw, 1);
    lyout->addLayout(hlyout);
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    pbOk->setIcon(QIcon(":/res/ok.png"));
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    pbCancel->setIcon(QIcon(":/res/cross.png"));
    connect(pbOk,SIGNAL(clicked()),this,SLOT(Ok()));
    connect(pbCancel,SIGNAL(clicked()),this,SLOT(Cancel()));
    hlyout = new QHBoxLayout;
    hlyout->addWidget(pbOk);
    hlyout->addWidget(pbCancel);
    lyout->addLayout(hlyout);
    setLayout(lyout);
}

void cmp_newsubsection::transliteDirName()
{
    s_tqLineEdit *le1 = this->findChild<s_tqLineEdit *>("name");
    s_tqLineEdit *le2 = this->findChild<s_tqLineEdit *>("tblename");
    if ((le1 == 0) || (le2 == 0))
    {
        CMPNSDBG;
        return;
    }
    if (le2->text().isEmpty())
    {
        QString tmpString = pc.getTranslit(le1->text());
        le2->setText(tmpString);
    }
}

void cmp_newsubsection::SetSomethingChanged()
{
    SomethingChanged = true;
}

void cmp_newsubsection::Cancel()
{
    if (SomethingChanged)
    {
        if (MessageBox2::question(this,"Данные были изменены","Данные были изменены, всё равно выйти?"))
            this->close();
        else
            return;
    }
    this->close();
}

void cmp_newsubsection::Ok()
{
    // временное
    WaitWidget *w = new WaitWidget();
//    w->show();
    w->Start();
    // временное */
    s_tqLineEdit *le1 = this->findChild<s_tqLineEdit *>("name");
    s_tqLineEdit *le2 = this->findChild<s_tqLineEdit *>("tblename");
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("cwpar");
    if ((le1 == 0) || (le2 == 0) || (cw == 0))
    {
        CMPNSDBG;
        w->Stop();
        return;
    }
    if ((le1->text().isEmpty()) || (le2->text().isEmpty()))
    {
        CMPNSWARN;
        w->Stop();
        return;
    }
    QString desc = le2->text().toLower(); // наименование таблицы компонентов строчными буквами
    QString descfull = le1->text().toLower();
    QString descfull1 = descfull.left(1).toUpper();
    descfull.replace(0,1,descfull1); // делаем первую букву прописной
    // 2. Создать таблицу в соотв. БД (например, relay в altium)
    // сначала проверим наличие такой таблицы
    QStringList sl = QStringList() << "" << "А" << "З" << "Э" << "К" << "У";
    QStringList dbsl = QStringList() << "" << "alt" << "sch" << "sol" << "con" << "dev";
    QStringList Fields, Values;
    Fields = sqlc.GetColumnsFromTable(dbsl.at(CompType), desc); // проверка таблицы на существование
    if (sqlc.result) // нет такой таблицы
    {
        switch(CompType)
        {
        case CTYPE_ALT:
        {
            // создаём таблицу
            Fields = QStringList() << "Library Ref" << "Footprint Ref" << "Sim Description" << "Sim File" << \
                                      "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
                                      "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
                                      "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
                                      "Nominal" << "Unit" << "par4" << "par5";
            break;
        }
        }
        sqlc.CreateTable(dbsl.at(CompType), desc, Fields, true); // Simple=true, т.е. id без имени таблицы
        if (sqlc.result)
        {
            CMPNSWARN;
            w->Stop();
            return;
        }
    }
    else
        CMPNSINFO("Таблица "+desc+" уже имеется в БД!");
    // создаём её описание в description
    // сначала проверяем, нет ли такой записи
    Fields = QStringList() << "description" << "descriptionfull";
    Values = QStringList() << desc << descfull;
    QString TableName = sl.at(CompType)+"Компоненты_описание_полн";
    QString newID = sqlc.GetValueFromTableByFields(dbsl.at(CompType), "description", "iddescription", Fields, Values);
    if (sqlc.result) // нет такой записи
    {
        // создаём запись
        newID = tfl.insert(TableName);
        if (tfl.result)
        {
            CMPNSINFO("Ошибка добавления записи в таблицу description");
            w->Stop();
            return;
        }
    }
    QStringList fl = QStringList() << "ИД" << "Наименование" << "Описание";
    QStringList vl = QStringList() << newID << desc << descfull;
    tfl.idtois(TableName,fl,vl);
    if (tfl.result)
    {
        tfl.Delete(TableName, newID);
        CMPNSINFO("Ошибка обновления записи в таблице description");
        w->Stop();
        return;
    }

    // 3. Создать категорию, если её ещё нет
    TableName = "Категории_полн";
    if (!(tfl.Check(TableName,"Наименование",descfull))) // нет такой категории, пишем в БД
    {
        newID = tfl.insert(TableName);
        if (tfl.result)
        {
            CMPNSWARN;
            w->Stop();
            return;
        }
        QStringList fl = QStringList() << "ИД" << "Наименование" << "ИД_а" << "Параметры";
        QStringList vl = QStringList() << newID << descfull << "0" << cw->Value();
        tfl.idtois(TableName,fl,vl);
        if (tfl.result)
        {
            tfl.Delete(TableName, newID);
            CMPNSWARN;
            w->Stop();
            return;
        }
    }
    w->Stop();
    this->close();
    CMPNSINFO("Записано успешно!");
}
