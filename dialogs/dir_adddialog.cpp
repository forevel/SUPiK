#include "dir_adddialog.h"
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QFontMetrics>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqcheckbox.h"
#include "../widgets/s_tqspinbox.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqwidget.h"
#include "../widgets/s_tqstackedwidget.h"
#include "s_accessdialog.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "../models/s_duniversal.h"

dir_adddialog::dir_adddialog(bool update, QString dirtype, QString dir, QWidget *parent) :
    QDialog(parent) // dirtype - имя таблицы из tablefields, где искать информацию о справочнике (dirlist, dirsyslist и т.д.)
                    // если диалог вызван для редактирования таблиц, dirtype должен быть пустым
{
    idx = 0;
//    setMinimumWidth(300);
    setStyleSheet("QDialog {background-color: rgba(255,255,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    isSomethingChanged = false;
    this->dir = dir;
    this->dirtype = dirtype;
    upd = update;
    IsDir = (!dirtype.isEmpty());
    setupUI();
    FW_Links.clear();
    FW_Links << "0.Автонумерация" << "1.Фиксированное значение" << "2.Простая ссылка" << "3.Ссылка на несколько таблиц" << "4.Ссылка на дочерние элементы" << \
                "5.Значение с макс. ИД" << "6.Форматированное поле" << "7.Вычисляемое поле" << "8.Простое поле" << "9.Права доступа" << \
                "10.Выбор таблицы" << "11.Специальная ссылка" << "12.Ссылка на методы" << "13.Вызов диалога редактирования строки" << "14.Конструктор ссылок" << \
                "15.Ссылка на файл" << "16.Ссылка на каталог" << "17.Ссылка на элемент внутри файла" << "18.Выбор даты" << "19.Автодополнение нулями";
}

void dir_adddialog::setupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl;
    s_tqPushButton *pb;
    s_tqComboBox *cb;
    s_tqSpinBox *spb;
    s_tqLineEdit *le;
    lbl = new s_tqLabel("Редактор таблиц");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0, Qt::AlignLeft);

    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(CancelAndClose()));
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    lyout->addLayout(hlyout);
    QTabWidget *mainTW = new QTabWidget;
    QDialog *dlg1 = new QDialog;
    QDialog *dlg2 = new QDialog;
    QDialog *dlg3 = new QDialog;
    dlg2->setObjectName("dlg2");
    dlg3->setObjectName("dlg3");
    QGridLayout *dlg1Layout = new QGridLayout;
    QVBoxLayout *dlg1vlyout = new QVBoxLayout;
    QGridLayout *dlg2Layout = new QGridLayout;
    QGridLayout *dlg3Layout = new QGridLayout;
    lbl = new s_tqLabel("Имя справочника(таблицы)");
    dlg1Layout->addWidget(lbl, 0, 0);
    le = new s_tqLineEdit;
    le->setObjectName("dirAlias");
    connect(le, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    dlg1Layout->addWidget(le, 0, 1, 1, 2);
    lbl = new s_tqLabel("Количество полей");
    spb = new s_tqSpinBox;
    spb->setObjectName("dirFieldNum");
    spb->setValue(1);
    spb->setMinimum(1);
    spb->setMaximum(FSIZE);
    spb->setDecimals(0);
    connect(spb, SIGNAL(valueChanged(double)), this, SLOT(updateTWFields(double)));
    dlg1Layout->addWidget(lbl, 1, 0);
    dlg1Layout->addWidget(spb, 1, 1, 1, 2);
    if (IsDir)
    {
        lbl = new s_tqLabel("Тип справочника");
        cb = new s_tqComboBox;
        cb->setObjectName("dirBelong");
        QStringListModel *tmpSLM = new QStringListModel;
        QStringList tmpSL;
        tmpSL << "Основной";
        if (pc.access & (ACC_ALT_WR | ACC_SYS_WR)) // САПРовские права
            tmpSL << "Altium" << "Schemagee" << "Solidworks" << "Устройства" << "Конструктивы";
        if (pc.access & (ACC_TB_WR | ACC_SYS_WR)) // ГИ права
            tmpSL << "ОТ и ТБ";
        if (pc.access & (ACC_SADM_WR | ACC_SYS_WR)) // Сисадминские права
            tmpSL << "СисАдмин";
        if (pc.access & ACC_SYS_WR)
            tmpSL << "Системный";
        dirBelongAliases.clear();
        dirBelongAliases.insert("Основной","ent");
        dirBelongAliases.insert("Altium","alt");
        dirBelongAliases.insert("Schemagee","sch");
        dirBelongAliases.insert("Solidworks","sol");
        dirBelongAliases.insert("Устройства","dev");
        dirBelongAliases.insert("Конструктивы","con");
        dirBelongAliases.insert("ОТ и ТБ","otb");
        dirBelongAliases.insert("СисАдмин","sys");
        dirBelongAliases.insert("Системный","sup");
        cb->setEnabled(true);
        tmpSLM->setStringList(tmpSL);
        cb->setModel(tmpSLM);
        dlg1Layout->addWidget(lbl, 2, 0);
        dlg1Layout->addWidget(cb, 2, 1, 1, 2);
        lbl = new s_tqLabel("Права доступа");
        le = new s_tqLineEdit;
        le->setEnabled(false);
        le->setObjectName("dirAccess");
        pb = new s_tqPushButton("...");
        if (pc.access & ACC_SYS_WR) // если есть права на изменение системных вещей
            pb->setEnabled(true);
        else
            pb->setEnabled(false);
        connect(pb, SIGNAL(clicked()), this, SLOT(setAccessRights()));
        dlg1Layout->addWidget(lbl, 3, 0);
        dlg1Layout->addWidget(le, 3, 1);
        dlg1Layout->addWidget(pb, 3, 2);
    }
    lbl = new s_tqLabel("Имя таблицы в БД (необязательно)");
    le = new s_tqLineEdit;
    le->setObjectName("dirName");
    dlg1Layout->addWidget(lbl, 4, 0);
    dlg1Layout->addWidget(le, 4, 1, 1, 2);
    dlg1Layout->setColumnStretch(0, 10);
    dlg1Layout->setColumnStretch(1, 90);
    dlg1Layout->setColumnStretch(2, 0);
    dlg1vlyout->addLayout(dlg1Layout);
    dlg1vlyout->addSpacing(200);
    dlg1->setLayout(dlg1vlyout);
    mainTW->addTab(dlg1, "Основные");

    for (int i = 0; i < FSIZE; i++)
    {
        QList<QWidget *> wl;
        lbl = new s_tqLabel("Поле №" + QString::number(i+1) + " :");
        lbl->setObjectName("hdr"+QString::number(i)+"L");
        wl << lbl;
        le = new s_tqLineEdit;
        le->setObjectName("name" + QString::number(i) + "LE");
        le->setAData(i);
        adjustFieldSize(le, 20);
        connect(le,SIGNAL(textChanged(QString,s_tqLineEdit*)),this,SLOT(transliteFieldName(QString,s_tqLineEdit*)));
        wl << le;
        lbl = new s_tqLabel(",сист. имя:");
        lbl->setObjectName("body"+QString::number(i)+"L1");
        wl << lbl;
        le = new s_tqLineEdit;
        le->setObjectName("field" + QString::number(i) + "LE");
        adjustFieldSize(le, 15);
        wl << le;
        lbl = new s_tqLabel(", содержит:");
        lbl->setObjectName("body"+QString::number(i)+"L2");
        wl << lbl;
        le = new s_tqLineEdit;
        le->setObjectName("value" + QString::number(i) + "LE");
        adjustFieldSize(le, 10);
        le->setEnabled(false);
        wl << le;
        pb = new s_tqPushButton("Конструктор");
        pb->setAData(i);
        pb->setObjectName("valueconst" + QString::number(i) + "PB");
        adjustFieldSize(pb, 4);
        connect(pb, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(FPBPressed(s_tqPushButton*)));
        wl << pb;
        if (i > FSIZE2)
            addLineToDlg(wl, *dlg3Layout, i-FSIZE2);
        else
            addLineToDlg(wl, *dlg2Layout, i);
    }
    dlg2Layout->setColumnStretch(1, 10);
    dlg3Layout->setColumnStretch(1, 10);
    dlg2Layout->setColumnStretch(5, 10);
    dlg3Layout->setColumnStretch(5, 10);
    dlg2->setLayout(dlg2Layout);
    dlg3->setLayout(dlg3Layout);
    mainTW->addTab(dlg2, "Поля 1");
    mainTW->addTab(dlg3, "Поля 2");

    mainTW->setObjectName("mainTW");
    lyout->addWidget(mainTW, 95);
    setLayout(lyout);
    setMinimumHeight(height()); // меньше не даём изменять, ибо некрасиво
    setMinimumWidth(width()+50);
    if (upd)
        fillFields();
    updateTWFields(spb->value());
}

void dir_adddialog::CancelAndClose()
{
    this->close();
}

void dir_adddialog::WriteAndClose()
{
    int i;
    s_tqLineEdit *lefield;
    s_tqLineEdit *levalue;
    s_tqLineEdit *lename;
    s_tqLineEdit *dirNameLE;
    s_tqLineEdit *dirAliasLE;
    s_tqLineEdit *dirAccessLE;
    s_tqComboBox *dirB;
    s_tqSpinBox *sb;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
    {
        emit error(ER_DIRADD,0x11);
        return;
    }
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        emit error(ER_DIRADD,0x12);
        return;
    }
    dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
    if (dirAccessLE == 0)
    {
        emit error(ER_DIRADD,0x13);
        return;
    }
    if (IsDir)
    {
        QString tmple = dirAccessLE->text();
        tmple.append("_полн");
        dirAccessLE->setText(tmple);
    }
    dirB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirB == 0)
    {
        emit error(ER_DIRADD,0x14);
        return;
    }
    QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "header", "tablename", dirAliasLE->text());
    if (sqlc.result == 2) // ошибка открытия таблицы
    {
        emit error(ER_DIRADD,0x15);
        return;
    }
    else if (!sqlc.result) // есть запись про таблицу
    {
        if (QMessageBox::question(this, "Данные справочника существуют", "Перезаписать?", QMessageBox::Yes|QMessageBox::No,\
                              QMessageBox::No) == QMessageBox::No)
            return;
    }
    sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        emit error(ER_DIRADD,0x16);
        return;
    }
    QSqlDatabase db = sqlc.getdb("sup");
    QStringList fl, vl;
    QStringList cmpfl, cmpvl;
    cmpfl << "tablename" << "tablefields";
    QString tble = dirBelongAliases.value(dirB->currentText());
    fl << "tablefields" << "table" << "keyfield" << "header" << "links" << "fieldsorder" << "tablename" << \
          "date" << "deleted" << "idpers";
    for (i = 0; i < sb->value(); i++)
    {
        levalue = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        lename = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        lefield = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
        vl.clear();
        cmpvl.clear();
        tmpString = (lename->text() == "ИД") ? "v" : "";
        vl << lefield->text() << tble+"."+dirNameLE->text() << tmpString << lename->text() << levalue->text();
        tmpString = QString("%1").arg(i, 2, 10, QChar('0'));
        vl << tmpString << dirAliasLE->text() << pc.DateTime << "0" << QString::number(pc.idPers);
        cmpvl << dirAliasLE->text() << lefield->text();
        QString id = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "idtablefields", cmpfl, cmpvl);
        if (sqlc.result == 1) // нет такой записи
            sqlc.insertvaluestotable(sqlc.getdb("sup"), "tablefields", fl, vl);
        else
            sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
        if (sqlc.result)
        {
            emit error(ER_DIRADD+sqlc.result,0x17);
            return;
        }
    }
    tble = "dirlist";
    if (IsDir)
    {
        fl.clear();
        vl.clear();
        fl << "dirlist" << "access" << "deleted";
        vl << dirAliasLE->text() << dirAccessLE->text() << "0";
        tmpString = sqlc.getvaluefromtablebyfield(db, tble, "dirlist", "dirlist", dirAliasLE->text());
        if (tmpString.isEmpty())
        {
            tmpString = sqlc.insertvaluestotable(db, tble, fl, vl);
            if (sqlc.result)
            {
                emit error(ER_DIRADD+sqlc.result,0x18);
                return;
            }
        }
        else
        {
            tmpString = sqlc.updatevaluesintable(db, tble, fl, vl, "dirlist", dirAliasLE->text());
            if (sqlc.result)
            {
                emit error(ER_DIRADD+sqlc.result,0x19);
                return;
            }
        }
    }
    QDialog *dlg = new QDialog;
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked()),this,SLOT(close()));
    connect(pb,SIGNAL(clicked()),dlg,SLOT(close()));
    s_tqLabel *lbl = new s_tqLabel("Записано успешно!");
    lyout->addWidget(lbl);
    lyout->addWidget(pb);
    dlg->setLayout(lyout);
    dlg->exec();
}

void dir_adddialog::updateTWFields(double dfn)
{
    s_tqLabel *ll = new s_tqLabel;
    s_tqLineEdit *le = new s_tqLineEdit;
    s_tqPushButton *pb = new s_tqPushButton;
    int i = 0;
    int fn = static_cast<int>(dfn);
    while (i < fn)
    {
        ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x21);
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x22);
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x23);
            return;
        }
        ll->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x24);
            return;
        }
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x25);
            return;
        }
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x26);
            return;
        }
        le->setVisible(true);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            emit error(ER_DIRADD,0x27);
            return;
        }
        pb->setVisible(true);
        i++;
    }
    while (i < FSIZE)
    {
        ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x28);
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x29);
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            emit error(ER_DIRADD,0x2A);
            return;
        }
        ll->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x2B);
            return;
        }
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x2C);
            return;
        }
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x2D);
            return;
        }
        le->setVisible(false);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            emit error(ER_DIRADD,0x2E);
            return;
        }
        pb->setVisible(false);
        i++;
    }
}

void dir_adddialog::setAccessRights()
{
    s_accessdialog *accessdialog = new s_accessdialog;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("dirAccess");
    accessdialog->SetupUI(le->text());
    connect(accessdialog, SIGNAL(acceptChanges(QString)), this, SLOT(acceptAccess(QString)));
    accessdialog->exec();
}

// обработка подтверждения в диалоге accessrights

void dir_adddialog::acceptAccess(QString rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("dirAccess");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x2F);
        return;
    }
    le->setText(rights);
}

void dir_adddialog::addLineToDlg(QList<QWidget *> wl, QGridLayout &lyt, int row)
{
    for (int i = 0; i < wl.size(); i++)
        lyt.addWidget(wl.at(i), row, i);
}

// Вызов конструктора ссылок

void dir_adddialog::FPBPressed(s_tqPushButton *ptr)
{
    QDialog *dlg = new QDialog(this);
    idx = ptr->getAData().toInt(0);
    // заполняем элементы конструктора ссылок
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x42);
        return;
    }
    QStringList links = le->text().split("."); // формируем links

    s_tqLabel *lbl1 = new s_tqLabel ("Тип поля");
    s_tqLabel *lbl2 = new s_tqLabel ("Тип ссылки");
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setObjectName("dtypecb");
    QStringListModel *cbmodel = new QStringListModel;
    QStringList tmpStringList; // делегаты
    tmpStringList << "0.Простое поле" << "1.Поле ввода" << "2.Поле выбора" << "3.Выпадающий список" << "4.Неактивное поле" <<  \
                     "5.Счётчик" << "6.Поле выбора с вводом" << "7.Поле с рамкой";
    cbmodel->setStringList(tmpStringList);
    cb->setModel(cbmodel);
    connect(cb,SIGNAL(currentIndexChanged(int)),this,SLOT(DTypeCBIndexChanged(int)));
    adjustFieldSize(cb, 25);
    s_tqComboBox *ltypecb = new s_tqComboBox;
    ltypecb->setObjectName("ltypecb");
    connect(ltypecb,SIGNAL(currentIndexChanged(QString)),this,SLOT(LTypeCBIndexChanged(QString)));
    adjustFieldSize(ltypecb, 30);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl1, 2);
    hlyout->setAlignment(lbl1, Qt::AlignRight);
    hlyout->addWidget(cb, 20);
    hlyout->addWidget(lbl2, 2);
    hlyout->setAlignment(lbl2, Qt::AlignRight);
    hlyout->addWidget(ltypecb, 20);
    lyout->addLayout(hlyout);
    cb->setCurrentIndex(0); // если links нет, то хотя бы установить выбор ссылки по 0-му делегату
    QStringList ids, vls;
    int i = 0;
    QSqlQuery get_tables(sqlc.getdb("sup"));
    get_tables.exec("SELECT DISTINCT `tablename` FROM `tablefields` ORDER BY `tablename` ASC;");
    while (get_tables.next())
    {
        i++;
        ids << QString("%1").arg(i, 5, 10, QChar('0'));
        vls << get_tables.value(0).toString();
    }

    s_tqLabel *lbl = new s_tqLabel("Зависимость от элемента");
    hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqSpinBox *spb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (spb == 0)
    {
        emit error(ER_DIRADD, 0x31);
        return;
    }
    int NumFields = spb->value();
    spb = new s_tqSpinBox;
    spb->setObjectName("dependsspb");
    spb->setMaximum(NumFields);
    spb->setMinimum(-1);
    spb->setDecimals(0);
    spb->setSingleStep(1);
    spb->setValue(-1);
    hlyout->addWidget(spb);
    if ((links.size()>2) && (!links.at(2).isEmpty()))
        spb->setValue(links.at(2).toInt());
    lyout->addLayout(hlyout);

    s_tqStackedWidget *sw = new s_tqStackedWidget;
    sw->setObjectName("linksconstrsw");
    s_tqWidget *wdgts[20];
    for (int i=0; i<20; i++)
    {
        wdgts[i] = new s_tqWidget;
        sw->addWidget(wdgts[i]);
    }

    lyout->addWidget(sw);

    // FW_NUMBER
    QVBoxLayout *vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Число");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    s_tqSpinBox *NumberSB = new s_tqSpinBox;
    NumberSB->setObjectName("number");
    NumberSB->setDecimals(0);
    NumberSB->setSingleStep(1);
    NumberSB->setMaximum(999999);
    NumberSB->setMinimum(0);
    hlyout->addWidget(NumberSB);
    vlyout->addLayout(hlyout);
    wdgts[1]->setLayout(vlyout);

    // FW_LINK
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Таблица");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqComboBox *tcb = new s_tqComboBox;
    tcb->setObjectName("fwlinkcb");
    QStringListModel *tml = new QStringListModel;
    tml->setStringList(vls);
    tcb->setModel(tml);
    tcb->setAData(FW_LINK);
    connect(tcb,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(TbleChoosed(QString,s_tqComboBox*)));
    hlyout->addWidget(tcb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поле");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqComboBox *stcb = new s_tqComboBox;
    stcb->setObjectName("fwlinkcb2");
    hlyout->addWidget(stcb);
    vlyout->addLayout(hlyout);
    wdgts[2]->setLayout(vlyout);

    // FW_ALLINK
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Таблица");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    tcb = new s_tqComboBox;
    tcb->setObjectName("fwallinkcb");
    tml = new QStringListModel;
    tml->setStringList(vls);
    tcb->setModel(tml);
    tcb->setAData(FW_ALLINK);
    connect(tcb,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(TbleChoosed(QString,s_tqComboBox*)));
    hlyout->addWidget(tcb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поле");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    stcb = new s_tqComboBox;
    stcb->setObjectName("fwallinkcb2");
    hlyout->addWidget(stcb);
    vlyout->addLayout(hlyout);
    wdgts[4]->setLayout(vlyout);

    // FW_SPECIAL
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Таблица");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    tcb = new s_tqComboBox;
    tcb->setObjectName("fwspecialcb");
    tml = new QStringListModel;
    tml->setStringList(vls);
    tcb->setModel(tml);
    tcb->setAData(FW_SPECIAL);
    connect(tcb,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(TbleChoosed(QString,s_tqComboBox*)));
    hlyout->addWidget(tcb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поле");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    stcb = new s_tqComboBox;
    stcb->setObjectName("fwspecialcb2");
    hlyout->addWidget(stcb);
    vlyout->addLayout(hlyout);
    wdgts[11]->setLayout(vlyout);

    // FW_MAXLINK
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Таблица");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    cb = new s_tqComboBox;
    tml = new QStringListModel;
    tml->setStringList(vls);
    cb->setModel(tml);
    cb->setObjectName("fwmaxlinkcb");
    cb->setAData(FW_MAXLINK);
    connect(cb,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(TbleChoosed(QString,s_tqComboBox*)));
    hlyout->addWidget(cb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поле значения");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    cb = new s_tqComboBox;
    cb->setObjectName("fwmaxlinkcb2");
    hlyout->addWidget(cb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Поле сравнения");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    cb = new s_tqComboBox;
    cb->setObjectName("fwmaxlinkcb3");
    hlyout->addWidget(cb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Значение (опц.)");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqLineEdit *ValueEditLE = new s_tqLineEdit;
    ValueEditLE->setObjectName("fwmaxlinkle");
    hlyout->addWidget(ValueEditLE);
    vlyout->addLayout(hlyout);
    wdgts[5]->setLayout(vlyout);

    // FW_DLINK
    vlyout = new QVBoxLayout;
    s_tqComboBox *TbleCB, *TbleFieldsCB;
    vls.insert(0,""); // добавление пустого элемента, ибо в DLINK возможно отсутствие значения
    for (i = 0; i < 4; i++)
    {
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Таблица"+QString::number(i));
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        TbleCB = new s_tqComboBox;
        tml = new QStringListModel;
        tml->setStringList(vls);
        TbleCB->setModel(tml);
        TbleCB->setObjectName("tble"+QString::number(i));
        TbleCB->setAData(FW_DLINK+20+i); // +20 - чтобы точно перекрыть диапазон возможных вариантов полей
        connect(TbleCB,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(TbleChoosed(QString,s_tqComboBox*)));
        hlyout->addWidget(TbleCB);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле"+QString::number(i));
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        TbleFieldsCB = new s_tqComboBox;
        TbleFieldsCB->setObjectName("tblefield"+QString::number(i));
        hlyout->addWidget(TbleFieldsCB);
        vlyout->addLayout(hlyout);
    }
    wdgts[3]->setLayout(vlyout);

    // FW_MASKED
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Регулярное выражение");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    ValueEditLE = new s_tqLineEdit;
    ValueEditLE->setObjectName("fwmaskedle");
    hlyout->addWidget(ValueEditLE);
    vlyout->addLayout(hlyout);
    wdgts[6]->setLayout(vlyout);

    // FW_EQUAT
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Выражение1");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    ValueEditLE = new s_tqLineEdit;
    ValueEditLE->setObjectName("fwequatle");
    hlyout->addWidget(ValueEditLE);
    vlyout->addLayout(hlyout);
    QStringList ops = QStringList() << "+" << "-" << "*" << "/";
    QStringListModel *opsmdl = new QStringListModel;
    opsmdl->setStringList(ops);
    s_tqComboBox *opscb = new s_tqComboBox;
    opscb->setObjectName("fwequatcb");
    opscb->setModel(opsmdl);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Операция");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    hlyout->addWidget(opscb);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Выражение2");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    ValueEditLE = new s_tqLineEdit;
    ValueEditLE->setObjectName("fwequatle2");
    hlyout->addWidget(ValueEditLE);
    vlyout->addLayout(hlyout);
    wdgts[7]->setLayout(vlyout);

    // FW_ID
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Размер поля");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    NumberSB = new s_tqSpinBox;
    NumberSB->setObjectName("id");
    NumberSB->setDecimals(0);
    NumberSB->setSingleStep(1);
    NumberSB->setMaximum(9);
    NumberSB->setMinimum(0);
    NumberSB->setValue(7);
    hlyout->addWidget(NumberSB);
    vlyout->addLayout(hlyout);
    wdgts[19]->setLayout(vlyout);

    lyout->setSizeConstraint(QLayout::SetFixedSize);
    hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked()),this,SLOT(ConstructLink()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Неа");
    connect(pb,SIGNAL(clicked()),dlg,SLOT(close()));
    hlyout->addWidget(pb);
    connect(this,SIGNAL(closelinkdialog()),dlg,SLOT(close()));
    lyout->addLayout(hlyout);
    dlg->setLayout(lyout);
    if (links.size()>1)
    {
        cb->setCurrentIndex(links.at(0).toInt());
        QString LTypeCBString = FW_Links.at(links.at(1).toInt());
        ltypecb->setCurrentText(LTypeCBString);
    }

    dlg->exec();
//    updateTWFields(sb->value());
}

// установка типа поля по выбранному типу делегата

void dir_adddialog::DTypeCBIndexChanged(int FD)
{
    QStringListModel *ltypecbmodel = new QStringListModel;
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("ltypecb");
    QStringList tmpStringList;
    if (cb == 0)
    {
        emit error(ER_DIRADD+0x01,0x41);
        return;
    }
    switch (FD)
    {
    case FD_SIMPLE:
    case FD_DISABLED:
    case FD_SIMGRID:
    {
        tmpStringList << FW_Links.at(0) << FW_Links.at(1) << FW_Links.at(5) << FW_Links.at(7) << FW_Links.at(8) << FW_Links.at(19);
        break;
    }
    case FD_LINEEDIT:
    {
        tmpStringList << FW_Links.at(6) << FW_Links.at(8);
        break;
    }
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        tmpStringList << FW_Links.at(2) << FW_Links.at(3) << FW_Links.at(4) << FW_Links.at(9) << FW_Links.at(10) \
                      << FW_Links.at(11) << FW_Links.at(12) << FW_Links.at(13) << FW_Links.at(14) << FW_Links.at(15) \
                      << FW_Links.at(16) << FW_Links.at(17) << FW_Links.at(18);
        break;
    }
    case FD_COMBO:
    {
        tmpStringList << FW_Links.at(2) << FW_Links.at(4) << FW_Links.at(10) << FW_Links.at(12);
        break;
    }
    case FD_SPIN:
    {
        tmpStringList << FW_Links.at(8);
        break;
    }
    default:
        break;
    }
    ltypecbmodel->setStringList(tmpStringList);
    cb->setModel(ltypecbmodel);
}

// отображение дополнительных элементов диалога при выборе типа ссылки

void dir_adddialog::LTypeCBIndexChanged(QString str)
{
    int wdgtsidx = FW_Links.indexOf(str);
    s_tqStackedWidget *sw = this->findChild<s_tqStackedWidget *>("linksconstrsw");
    if (sw == 0)
    {
        emit error(ER_DIRADD+0x01,0x42);
        return;
    }
    sw->setCurrentIndex(wdgtsidx);
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x42);
        return;
    }
    QStringList links = le->text().split("."); // формируем links
    switch (wdgtsidx)
    {
    case FW_NUMBER:
    {
        if (links.size()>3)
        {
            s_tqSpinBox *NumberSB = this->findChild<s_tqSpinBox *>("number");
            if (NumberSB == 0)
                return;
            NumberSB->setValue(links.at(3).toDouble());
        }
        break;
    }
    case FW_LINK:
    {
        if (links.size()>4)
        {
            s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwlinkcb");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwlinkcb2");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(4));
        }
        break;
    }
    case FW_SPECIAL:
    {
        if (links.size()>4)
        {
            s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwspecialcb");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwspecialcb2");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(4));
        }
        break;
    }
    case FW_ALLINK:
    {
        if (links.size()>4)
        {
            s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwallinkcb");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwallinkcb2");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(4));
        }
        break;
    }
    case FW_DLINK:
    {
        if (links.size()>4)
        {
            int i = 0;
            links.removeFirst();
            links.removeFirst();
            links.removeFirst();
            while (links.size()>2) // вытаскиваем данные попарно
            {
                s_tqComboBox *cb = this->findChild<s_tqComboBox *>("tble"+QString::number(i));
                if (cb == 0)
                    return;
                cb->setCurrentText(links.at(0));
                cb = this->findChild<s_tqComboBox *>("tblefield"+QString::number(i));
                if (cb == 0)
                    return;
                cb->setCurrentText(links.at(1));
                links.removeFirst();
                links.removeFirst();
                i++;
            }
        }
        break;
    }
    case FW_MAXLINK:
    {
        if (links.size()>5)
        {
            s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb2");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(4));
            tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
            if (tcb == 0)
                return;
            tcb->setCurrentText(links.at(5));
            if (links.size()>6)
            {
                s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaxlinkle");
                if (le == 0)
                    return;
                le->setText(links.at(6));
            }
        }
        break;
    }
    case FW_MASKED:
    {
        if (links.size()>4)
        {
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaskedle");
            if (le == 0)
                return;
            le->setText(links.at(4));
        }
        break;
    }
    case FW_ID:
    {
        if (links.size()>3)
        {
            s_tqSpinBox *NumberSB = this->findChild<s_tqSpinBox *>("id");
            if (NumberSB == 0)
                return;
            NumberSB->setValue(links.at(3).toDouble());
        }
        break;
    }
    case FW_EQUAT:
    {
        if (links.size()>4)
        {
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwequatle");
            if (le == 0)
                return;
            le->setText(links.at(4));
            if (links.size()>6)
            {
                le = this->findChild<s_tqLineEdit *>("fwequatle2");
                if (le == 0)
                    return;
                le->setText(links.at(6));
                s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fwequatcb");
                if (cb == 0)
                    return;
                QString op = links.at(5);
                if (op == "s")  cb->setCurrentIndex(0);
                if (op == "r")  cb->setCurrentIndex(1);
                if (op == "m")  cb->setCurrentIndex(2);
                if (op == "d")  cb->setCurrentIndex(3);
            }
        }
        break;
    }
    }


}

// сборка строки ссылки из элементов выбора и выход из диалога конструктора

void dir_adddialog::ConstructLink()
{
    QString links;
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("dtypecb");
    if (cb == 0)
    {
        emit error(ER_DIRADD, 0x61);
        return;
    }
    links.append(QString::number(cb->currentIndex()));
    links.append(".");
    cb = this->findChild<s_tqComboBox *>("ltypecb");
    if (cb == 0)
    {
        emit error(ER_DIRADD,0x62);
        return;
    }
    links.append(QString::number(FW_Links.indexOf(cb->currentText())));
    links.append(".");
    s_tqSpinBox *spb = this->findChild<s_tqSpinBox *>("dependsspb");
    if (spb == 0)
    {
        emit (ER_DIRADD,0x63);
        return;
    }
    int spbvalue = spb->value();
    if (spbvalue != -1)
        links.append(QString::number(spb->value()));
    links.append(".");
    switch (cb->currentIndex())
    {
    case FW_ALLINK:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwallinkcb");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x64);
            return;
        }
        links.append(tcb->currentText());
        tcb = this->findChild<s_tqComboBox *>("fwallinkcb2");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x64);
            return;
        }
        links.append(tcb->currentText());
        break;
    }
    case FW_DLINK:
    {
        for (int i = 0; i < 4; i++)
        {
            s_tqComboBox *cb = this->findChild<s_tqComboBox *>("tble"+QString::number(i));
            if (cb == 0)
            {
                emit error(ER_DIRADD,0x65);
                return;
            }
            if (cb->currentText().isEmpty())
                break;
            links.append(cb->currentText());
            links.append(".");
            cb = this->findChild<s_tqComboBox *>("tblefield"+QString::number(i));
            if (cb == 0)
            {
                emit error(ER_DIRADD,0x65);
                return;
            }
            links.append(cb->currentText());
            links.append(".");
        }
        links.chop(1); // убрали последнюю точку
        break;
    }
    case FW_EQUAT:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwequatle");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x66);
            return;
        }
        links.append(le->text());
        le = this->findChild<s_tqLineEdit *>("fwequatle2");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x66);
            return;
        }
        if (!le->text().isEmpty())
        {
            links.append(".");
            QString ops[4] = {"s","r","m","d"};
            s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fwequatcb");
            if (cb == 0)
            {
                emit error(ER_DIRADD,0x66);
                return;
            }
            links.append(ops[cb->currentIndex()]);
            links.append(".");
            links.append(le->text());
        }
        break;
    }
    case FW_ID:
    {
        spb = this->findChild<s_tqSpinBox *>("id");
        if (spb == 0)
        {
            emit error(ER_DIRADD,0x67);
            return;
        }
        links.append(QString::number(spb->value()));
        break;
    }
    case FW_LINK:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwlinkcb");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x68);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwlinkcb2");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x68);
            return;
        }
        links.append(tcb->currentText());
        break;
    }
    case FW_MASKED:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaskedle");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x69);
            return;
        }
        links.append(le->text());
        break;
    }
    case FW_MAXLINK:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x6A);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb2");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x6A);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x6A);
            return;
        }
        links.append(tcb->currentText());
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaxlinkle");
        if (le == 0)
        {
            emit error(ER_DIRADD,0x6A);
            return;
        }
        if (!le->text().isEmpty())
        {
            links.append(".");
            links.append(le->text());
        }
        break;
    }
    case FW_NUMBER:
    {
        spb = this->findChild<s_tqSpinBox *>("number");
        if (spb == 0)
        {
            emit error(ER_DIRADD,0x6B);
            return;
        }
        links.append(QString::number(spb->value()));
        break;
    }
    case FW_SPECIAL:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwspecialcb");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x6C);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwspecialcb2");
        if (tcb == 0)
        {
            emit error(ER_DIRADD,0x6C);
            return;
        }
        links.append(tcb->currentText());
        break;
    }
    default:
        break;
    }
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x6D);
        return;
    }
    le->setText(links);
    emit closelinkdialog();
}

void dir_adddialog::TbleChoosed(QString str, s_tqComboBox *ptr)
{
    int Type = ptr->getAData().toInt();
    s_tqComboBox *cb, *cb2;
    QString cb1name, cb2name;
    switch(Type)
    {
    case FW_LINK:
    {
        cb1name = "fwlinkcb";
        cb2name = "fwlinkcb2";
        break;
    }
    case FW_ALLINK:
    {
        cb1name = "fwallinkcb";
        cb2name = "fwallinkcb2";
        break;
    }
    case FW_MAXLINK:
    {
        cb1name = "fwmaxlinkcb";
        cb2name = "fwmaxlinkcb2";
        break;
    }
    case FW_SPECIAL:
    {
        cb1name = "fwspecialcb";
        cb2name = "fwspecialcb2";
        break;
    }
    case FW_DLINK+20:
    {
        cb1name = "tble0";
        cb2name = "tblefield0";
        break;
    }
    case FW_DLINK+21:
    {
        cb1name = "tble1";
        cb2name = "tblefield1";
        break;
    }
    case FW_DLINK+22:
    {
        cb1name = "tble2";
        cb2name = "tblefield2";
        break;
    }
    case FW_DLINK+23:
    {
        cb1name = "tble3";
        cb2name = "tblefield3";
        break;
    }
    }
    cb = this->findChild<s_tqComboBox *>("fwlinkcb");
    if (cb == 0)
        return;
    cb2 = this->findChild<s_tqComboBox *>("fwlinkcb2");
    if (cb2 == 0)
        return;
    QStringList sl = tfl.tableheaders(cb->currentText());
    if (tfl.result)
        return;
    QStringListModel *cb2mdl = new QStringListModel;
    cb2mdl->setStringList(sl);
    cb2->setModel(cb2mdl);
    if (Type == FW_MAXLINK)
    {
        s_tqComboBox *cb3 = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
        if (cb3 == 0)
            return;
        QStringListModel *cb3mdl = new QStringListModel;
        cb3mdl->setStringList(sl);
        cb3->setModel(cb3mdl);
    }
}

void dir_adddialog::adjustFieldSize(QWidget *wdgt, int widthInChar)
{
    QFontMetrics fm(wdgt->font());
    wdgt->setMinimumWidth(fm.width(QLatin1Char('x'))*widthInChar + 2*2);
}

void dir_adddialog::fillFields()
{
    s_tqLineEdit *lef = new s_tqLineEdit;
    s_tqLineEdit *len = new s_tqLineEdit;
    s_tqLineEdit *lev = new s_tqLineEdit;
    QList<QStringList> lsl;
    QStringList fl;
    fl << "table" << "tablefields" << "header" << "links";
    lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", dir, "fieldsorder", true);
    if (sqlc.result)
    {
        emit error(ER_DIRADD,0x81);
        return;
    }
    if (lsl.size() == 0)
    {
        emit error(ER_DIRADD,0x82);
        return;
    }
    s_tqLineEdit *dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
    {
        emit error(ER_DIRADD,0x83);
        return;
    }
    s_tqLineEdit *dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        emit error(ER_DIRADD,0x84);
        return;
    }
    s_tqComboBox *dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
    if ((dirBelongCB == 0) && (IsDir))
    {
        emit error(ER_DIRADD,0x85);
        return;
    }
    s_tqLineEdit *dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
    if ((dirAccessLE == 0) && (IsDir))
    {
        emit error(ER_DIRADD,0x86);
        return;
    }
    s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        emit error(ER_DIRADD,0x87);
        return;
    }
    if (IsDir)
    {
        QString tmpdir = dir;
        int diridx = tmpdir.indexOf("_полн");
        if (diridx != -1)
            tmpdir.chop(5);
        dirAliasLE->setText(tmpdir);
    }
    else
        dirAliasLE->setText(dir);
    dirNameLE->setText(lsl.at(0).at(0).split(".").at(1)); // берём имя таблицы по полю ИД
    if (IsDir)
    {
        QString tmpString = dirBelongAliases.key(lsl.at(0).at(0).split(".").at(0));
        if (tmpString.isEmpty())
        {
            emit error(ER_DIRADD,0x88);
            return;
        }
        dirBelongCB->setCurrentText(tmpString);
        QStringList fields = QStringList() << "Права доступа";
        QStringList values = tfl.valuesbyfield(dirtype+"_полн",fields,"Наименование",dir);
        if (tfl.result)
        {
            emit error(ER_DIRADD+0x01,0x88);
            return;
        }
        dirAccessLE->setText(values.at(0));
        dirAccessLE->setEnabled(true);
    }
    // для редактирования таблиц (не справочников) права не нужны, они по умолчанию системные
    sb->setValue(lsl.size());
    for (int i = 0; i < lsl.size(); i++)
    {
        if (i > 16)
            break;
        lef = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE"); // имя поля
        if (lef == 0)
        {
            emit error(ER_DIRADD,0x8A);
            return;
        }
        lev = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE"); // строка 1
        if (lev == 0)
        {
            emit error(ER_DIRADD,0x8B);
            return;
        }
        len = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE"); // строка 2
        if (len == 0)
        {
            emit error(ER_DIRADD,0x8C);
            return;
        }
        lef->setText(lsl.at(i).at(1));
        len->setText(lsl.at(i).at(2));
        lev->setText(lsl.at(i).at(3));
    }
}

void dir_adddialog::transliteFieldName(QString str, s_tqLineEdit *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("name"+ptr->getAData().toString()+"LE");
    s_tqLineEdit *le2 = new s_tqLineEdit;
    le2 = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x91);
        return;
    }
    if (le2 == 0)
    {
        emit error(ER_DIRADD,0x92);
        return;
    }
    if (le2->text().isEmpty())
        le2->setText(pc.getTranslit(le->text()));
}

void dir_adddialog::transliteDirName()
{
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
    {
        emit error(ER_DIRADD,0xA1);
        return;
    }
    if (dirNameLE->text().isEmpty())
    {
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
        {
            emit error(ER_DIRADD,0xA2);
            return;
        }
        QString tmpString = pc.getTranslit(dirAliasLE->text());
        dirNameLE->setText(tmpString);
    }
}
