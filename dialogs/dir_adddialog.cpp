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
                    // если диалог вызван для редактирования таблиц, dirtype содержит тип справочника - sys,alt,con,ent и т.д.
{
    idx = 0;
    setAttribute(Qt::WA_DeleteOnClose);
    isSomethingChanged = false;
    this->dir = dir;
    this->dirtype = dirtype;
    upd = update;
    this->IsDir = (!dirtype.isEmpty());
    if (IsDir)
        setStyleSheet("QDialog {background-color: rgba(255,255,153);}");
    else
        setStyleSheet("QDialog {background-color: rgba(194,194,194);}");
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
    dlg1Layout->addWidget(le, 0, 1);
    lbl = new s_tqLabel("Количество полей");
    spb = new s_tqSpinBox;
    spb->setObjectName("dirFieldNum");
    spb->setValue(1);
    spb->setMinimum(1);
    spb->setMaximum(FSIZE);
    spb->setDecimals(0);
    connect(spb, SIGNAL(valueChanged(double)), this, SLOT(updateTWFields(double)));
    dlg1Layout->addWidget(lbl, 1, 0);
    dlg1Layout->addWidget(spb, 1, 1);
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
    dlg1Layout->addWidget(cb, 2, 1);
    lbl = new s_tqLabel("Права доступа");
    s_tqChooseWidget *cw = new s_tqChooseWidget;
    cw->Setup("2.9");
    cw->setObjectName("dirAccess");
    if (pc.access & ACC_SYS_WR) // если есть права на изменение системных вещей
        cw->setEnabled(true);
    else
        cw->setEnabled(false);
    if (IsDir);
    else
    {
        cw->setEnabled(false);
        cw->SetData("узч");
    }
    dlg1Layout->addWidget(lbl, 3, 0);
    dlg1Layout->addWidget(cw, 3, 1);
    lbl = new s_tqLabel("Имя таблицы в БД (необязательно)");
    le = new s_tqLineEdit;
    le->setObjectName("dirname");
    connect(le,SIGNAL(editingFinished(QString)),this,SLOT(TbleNameChanged(QString)));
    dlg1Layout->addWidget(lbl, 4, 0);
    dlg1Layout->addWidget(le, 4, 1);
    dlg1Layout->setColumnStretch(0, 10);
    dlg1Layout->setColumnStretch(1, 90);
    dlg1Layout->setColumnStretch(2, 0);
    dlg1vlyout->addLayout(dlg1Layout);
    dlg1vlyout->addStretch(200);
    dlg1->setLayout(dlg1vlyout);
    mainTW->addTab(dlg1, "Основные");

    QVBoxLayout *dlg2vlyout = new QVBoxLayout;
    QVBoxLayout *dlg3vlyout = new QVBoxLayout;
/*    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel ("Учесть в кратком виде");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    dlg21lyout->addLayout(hlyout); */
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
        wl << le;
        lbl = new s_tqLabel("Системное имя:");
        lbl->setObjectName("body"+QString::number(i)+"L1");
        wl << lbl;
        cb = new s_tqComboBox;
        cb->setObjectName("field"+QString::number(i)+"CB");
        adjustFieldSize(cb, 15);
        wl << cb;
        lbl = new s_tqLabel("Описание (links):");
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
        s_tqCheckBox *chb = new s_tqCheckBox;
        chb->setObjectName("short"+QString::number(i));
        chb->setToolTip("Учёт в кратком виде справочника");
        wl << chb;
        if (i > FSIZE2)
            addLineToDlg(wl, *dlg3Layout, i-FSIZE2);
        else
            addLineToDlg(wl, *dlg2Layout, i);
    }
    dlg2Layout->setColumnStretch(1, 10);
    dlg3Layout->setColumnStretch(1, 10);
    dlg2Layout->setColumnStretch(5, 10);
    dlg3Layout->setColumnStretch(5, 10);
    dlg2vlyout->addLayout(dlg2Layout);
    dlg3vlyout->addLayout(dlg3Layout);
    dlg2vlyout->addStretch(200);
    dlg3vlyout->addStretch(200);
//    dlg21lyout->addLayout(dlg2vlyout);
//    dlg2->setLayout(dlg21lyout);
    dlg2->setLayout(dlg2vlyout);
    dlg3->setLayout(dlg3vlyout);
    mainTW->addTab(dlg2, "Поля 1");
    mainTW->addTab(dlg3, "Поля 2");

    mainTW->setObjectName("mainTW");
    lyout->addWidget(mainTW, 95);
    hlyout = new QHBoxLayout;
    pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(CancelAndClose()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);

    setLayout(lyout);
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
    s_tqComboBox *cbfield,*dirB;
    s_tqLineEdit *levalue,*lename,*dirNameLE,*dirAliasLE;
    s_tqSpinBox *sb;
    QString FullTblename, ShortTblename;
    bool FullToWrite=true, ShortToWrite=true;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    if (dirNameLE == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    if (dirAccessCW == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    if (IsDir)
    {
        QString tmps = dirAliasLE->text();
        FullTblename = tmps+"_полн";
        ShortTblename = tmps+"_сокращ";
    }
    else
        FullTblename = dirAliasLE->text();
    dirB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirB == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "header", "tablename", FullTblename);
    if (sqlc.result == 2) // ошибка открытия таблицы
    {
        WARNMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    else if (!sqlc.result) // есть запись про таблицу
    {
        if (QMessageBox::question(this, "Данные полного справочника существуют", "Перезаписать?", QMessageBox::Yes|QMessageBox::No,\
                              QMessageBox::No) == QMessageBox::No)
            FullToWrite = false;
    }
    if (IsDir)
    {
        tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "header", "tablename", ShortTblename); // ищем сокращённое описание справочника
        if (sqlc.result == 2) // ошибка открытия таблицы
        {
            WARNMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        else if (!sqlc.result) // есть запись про таблицу
        {
            if (QMessageBox::question(this, "Данные сокращённого справочника существуют", "Перезаписать?", QMessageBox::Yes|QMessageBox::No,\
                                  QMessageBox::No) == QMessageBox::No)
                ShortToWrite = false;
        }
    }
    else
        ShortToWrite = false; // для редактирования справочника через систему его имя уже содержится в FullTblename с необходимым суффиксом
    if (!ShortToWrite && !FullToWrite) // если везде ответили "нет", то выходим
        return;
    sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
        cbfield = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if ((levalue == 0) || (lename == 0) || (cbfield == 0) || (chb == 0))
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        vl.clear();
        cmpvl.clear();
        tmpString = (lename->text() == "ИД") ? "v" : "";
        vl << cbfield->currentText() << tble+"."+dirNameLE->text() << tmpString << lename->text() << levalue->text();
        tmpString = QString("%1").arg(i, 2, 10, QChar('0'));
        vl << tmpString << FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
        if (FullToWrite) // сначала пишем полный вариант
        {
            cmpvl << FullTblename << cbfield->currentText();
            QString id = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "idtablefields", cmpfl, cmpvl);
            if (sqlc.result == 1) // нет такой записи
                sqlc.insertvaluestotable(sqlc.getdb("sup"), "tablefields", fl, vl);
            else
                sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
            if (sqlc.result)
            {
                WARNMSG(PublicClass::ER_DIRADD,__LINE__);
                return;
            }
        }
        if (ShortToWrite) // теперь пишем сокращённый вариант только в том случае, если установлена пометка
        {
            vl.replace(6, ShortTblename); // заменяем полное наименование на сокращённое
            cmpvl = QStringList() << ShortTblename << cbfield->currentText();
            QString id = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "idtablefields", cmpfl, cmpvl);
            if (chb->isChecked())
            {
                if (sqlc.result == 1) // нет такой записи
                    sqlc.insertvaluestotable(sqlc.getdb("sup"), "tablefields", fl, vl);
                else
                    sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
                if (sqlc.result)
                {
                    WARNMSG(PublicClass::ER_DIRADD,__LINE__);
                    return;
                }
            }
            else
            {
                if (sqlc.result == 1); // нет такой записи, и хорошо
                else // иначе надо её удалить
                {
                    int delidx = fl.indexOf("deleted");
                    vl.replace(delidx, "1");
                    sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
                    if (sqlc.result)
                    {
                        WARNMSG(PublicClass::ER_DIRADD,__LINE__);
                        return;
                    }
                }
            }
        }
    }
    tble = "dirlist";
    if (IsDir)
    {
        QString tmpdir = dirAliasLE->text();
        int diridx = tmpdir.indexOf("_полн");
        if (diridx != -1)
            tmpdir.chop(5);
        fl.clear();
        vl.clear();
        fl << "dirlist" << "access" << "deleted";
        vl << tmpdir << dirAccessCW->Value().toString() << "0";
        tmpString = sqlc.getvaluefromtablebyfield(db, tble, "dirlist", "dirlist", tmpdir);
        if (tmpString.isEmpty())
        {
            tmpString = sqlc.insertvaluestotable(db, tble, fl, vl);
            if (sqlc.result)
            {
                WARNMSG(PublicClass::ER_DIRADD,__LINE__);
                return;
            }
        }
        else
        {
            tmpString = sqlc.updatevaluesintable(db, tble, fl, vl, "dirlist", tmpdir);
            if (sqlc.result)
            {
                WARNMSG(PublicClass::ER_DIRADD,__LINE__);
                return;
            }
        }
    }
    INFOMSG(PublicClass::ER_CMPMAIN,__LINE__,"Записано успешно!");
    this->close();
}

void dir_adddialog::updateTWFields(double dfn)
{
    s_tqLabel *ll;
    s_tqLineEdit *le;
    s_tqPushButton *pb;
    s_tqComboBox *cb;
    int i = 0;
    int fn = static_cast<int>(dfn);
    while (i < fn)
    {
        ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(true);
        cb = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        if (cb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        cb->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        le->setVisible(true);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        pb->setVisible(true);
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        chb->setVisible(true);
        i++;
    }
    while (i < FSIZE)
    {
        ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        ll->setVisible(false);
        cb = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        if (cb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        cb->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        le->setVisible(false);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        pb->setVisible(false);
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        chb->setVisible(false);
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    le->setText(rights);
}

void dir_adddialog::addLineToDlg(QList<QWidget *> wl, QGridLayout &lyt, int row)
{
    for (int i = 0; i < wl.size(); i++)
        lyt.addWidget(wl.at(i), row, i);
}

// Обработка изменения системного имени таблицы

void dir_adddialog::TbleNameChanged(QString tblename)
{
    // 1. Взять все столбцы из таблицы tblename
    // 2. NumCols = количество столбцов
    // 3. Создать модель mdl по столбцам
    // 4. Для каждого комбобокса с именем fields<i>CB взять текущий индекс idx
    // 5. Если idx >= NumCols, idx = NumCols-1
    // 6. Заполнить комбобокс моделью mdl
    // 1
    s_tqComboBox *dirB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirB == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    QString tmps = dirB->currentText();
    QStringList dirColumns = sqlc.getcolumnsfromtable(sqlc.getdb(dirBelongAliases[dirB->currentText()]), tblename);
    if (sqlc.result)
    {
        INFOMSG(PublicClass::ER_DIRADD,__LINE__,"Не найдена таблица");
        return;
    }
    dirColumns.removeAll("date");
    dirColumns.removeAll("deleted");
    dirColumns.removeAll("idpers");
    // 2
    int NumCols = dirColumns.size();
    // 3
    QStringListModel *mdl = new QStringListModel;
    mdl->setStringList(dirColumns);
    // 4
    for (int i = 0; i < FSIZE; i++)
    {
        s_tqComboBox *cb = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        if (cb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        int idx = cb->currentIndex();
        // 5
        if (idx >= NumCols)
            idx = NumCols-1;
        // 6
        cb->setModel(mdl);
        cb->setCurrentIndex(idx);
    }
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
    cb = this->findChild<s_tqComboBox *>(cb1name);
    if (cb == 0)
        return;
    cb2 = this->findChild<s_tqComboBox *>(cb2name);
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
    s_tqComboBox *cbf;
    s_tqLineEdit *len;
    s_tqLineEdit *lev;
    QList<QStringList> lsl, lslshort;
    QStringList fl;
    fl << "table" << "tablefields" << "header" << "links";
    lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", dir, "fieldsorder", true);
    if (sqlc.result)
    {
        WARNMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    if (IsDir)
    {
        QString tmps = dir;
        tmps.replace("_полн","_сокращ");
        lslshort = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", tmps, "fieldsorder", true);
    }
    if (lsl.size() == 0)
    {
        WARNMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqLineEdit *dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    if (dirNameLE == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqLineEdit *dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqComboBox *dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirBelongCB == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    if (dirAccessCW == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    QString tmpString = dirBelongAliases.key(lsl.at(0).at(0).split(".").at(0));
    if (tmpString.isEmpty())
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    dirBelongCB->setCurrentText(tmpString);
    dirNameLE->setText(lsl.at(0).at(0).split(".").at(1)); // берём имя таблицы по полю ИД
    if (IsDir)
    {
        QString tmpdir = dir;
        int diridx = tmpdir.indexOf("_полн");
        if (diridx != -1)
            tmpdir.chop(5);
        dirAliasLE->setText(tmpdir);
        QStringList fields = QStringList() << "Права доступа";
        QStringList values = tfl.valuesbyfield(dirtype+"_полн",fields,"Наименование",tmpdir);
        if (tfl.result)
        {
            WARNMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        dirAccessCW->SetValue(values.at(0));
        dirAccessCW->setEnabled(true);
    }
    else
        dirAliasLE->setText(dir);
    // для редактирования таблиц (не справочников) права не нужны, они по умолчанию системные
    sb->setValue(lsl.size());
    TbleNameChanged(dirNameLE->text()); // принудительно имитируем изменение имени таблицы для заполнения комбобоксов
    for (int i = 0; i < lsl.size(); i++)
    {
        if (i > 16)
            break;
        cbf = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB"); // имя поля
        if (cbf == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        lev = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE"); // строка 1
        if (lev == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        len = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE"); // строка 2
        if (len == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        cbf->setCurrentText(lsl.at(i).at(1));
        len->setText(lsl.at(i).at(2));
        QString tmps = lsl.at(i).at(3);
        if (tmps.startsWith("1.6")) // если максированное поле, надо добавить обратных слешей
        {
            QStringList tmpsl = tmps.split(".");
            if (tmpsl.size()>3) // может быть неправильная запись
            {
                QString tmps2 = tmpsl.at(3); // берём сам link
                tmps2.insert(0, "\\");
                if (tmps2.size()>2)
                    tmps2.insert(tmps2.size()-1, "\\");
                tmpsl.replace(3, tmps2);
            }
            tmps = tmpsl.join(".");
        }
        lev->setText(tmps);
        for (int j=0; j<lslshort.size(); j++) // проверка наличия в сокращённой таблице такого же элемента, как и в полной
        {
            if (lslshort.at(j).at(2) == len->text())
            {
                chb->setChecked(true);
                break;
            }
        }
    }
}

void dir_adddialog::transliteDirName()
{
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    if (dirNameLE == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    if (dirNameLE->text().isEmpty())
    {
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        QString tmpString = pc.getTranslit(dirAliasLE->text());
        dirNameLE->setText(tmpString);
    }
}

// Вызов конструктора ссылок

void dir_adddialog::FPBPressed(s_tqPushButton *ptr)
{
    QDialog *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    idx = ptr->getAData().toInt(0);
    // заполняем элементы конструктора ссылок
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    QStringList links = le->text().split("."); // формируем links

    s_tqLabel *lbl1 = new s_tqLabel ("Тип поля");
    s_tqLabel *lbl2 = new s_tqLabel ("Тип ссылки");
    s_tqComboBox *dtypecb = new s_tqComboBox;
    dtypecb->setObjectName("dtypecb");
    QStringListModel *cbmodel = new QStringListModel;
    QStringList tmpStringList; // делегаты
    tmpStringList << "0.Простое поле" << "1.Поле ввода" << "2.Поле выбора" << "3.Выпадающий список" << "4.Неактивное поле" <<  \
                     "5.Счётчик" << "6.Поле выбора с вводом" << "7.Поле с рамкой";
    cbmodel->setStringList(tmpStringList);
    dtypecb->setModel(cbmodel);
    connect(dtypecb,SIGNAL(currentIndexChanged(int)),this,SLOT(DTypeCBIndexChanged(int)));
    adjustFieldSize(dtypecb, 25);
    s_tqComboBox *ltypecb = new s_tqComboBox;
    ltypecb->setObjectName("ltypecb");
    connect(ltypecb,SIGNAL(currentIndexChanged(QString)),this,SLOT(LTypeCBIndexChanged(QString)));
    adjustFieldSize(ltypecb, 30);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl1, 2);
    hlyout->setAlignment(lbl1, Qt::AlignRight);
    hlyout->addWidget(dtypecb, 20);
    hlyout->addWidget(lbl2, 2);
    hlyout->setAlignment(lbl2, Qt::AlignRight);
    hlyout->addWidget(ltypecb, 20);
    lyout->addLayout(hlyout);
    dtypecb->setCurrentIndex(0); // если links нет, то хотя бы установить выбор ссылки по 0-му делегату
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
    stcb->setAData(FW_SPECIAL);
    hlyout->addWidget(stcb);
    vlyout->addLayout(hlyout);
    wdgts[11]->setLayout(vlyout);

    // FW_MAXLINK
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Таблица");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqComboBox *cb = new s_tqComboBox;
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
    ValueEditLE->setToolTip("Пример: [0-9]{0,2}-[0-9]{0,9}");
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
        dtypecb->setCurrentIndex(links.at(0).toInt());
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    sw->setCurrentIndex(wdgtsidx);
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            TbleChoosed(links.at(3),tcb);
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
        if (links.size()>3)
        {
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaskedle");
            if (le == 0)
                return;
            QString tmps = links.at(3);
            int begidx = tmps.indexOf("\\\"^");
            if (begidx != -1)
                tmps.remove(begidx, 3);
            int endidx = tmps.indexOf("$\\\"");
            if (endidx != -1)
                tmps.remove(endidx, 3);
            le->setText(tmps);
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    links.append(QString::number(cb->currentIndex()));
    links.append(".");
    cb = this->findChild<s_tqComboBox *>("ltypecb");
    if (cb == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    links.append(QString::number(FW_Links.indexOf(cb->currentText())));
    links.append(".");
    s_tqSpinBox *spb = this->findChild<s_tqSpinBox *>("dependsspb");
    if (spb == 0)
    {
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    int spbvalue = spb->value();
    if (spbvalue != -1)
        links.append(QString::number(spb->value()));
    links.append(".");
    int CurFW = cb->currentText().split(".").at(0).toInt(); // вытаскиваем номер типа ссылки из её названия
    switch (CurFW)
    {
    case FW_ALLINK:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwallinkcb");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwallinkcb2");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
                DBGMSG(PublicClass::ER_DIRADD,__LINE__);
                return;
            }
            if (cb->currentText().isEmpty())
                break;
            links.append(cb->currentText());
            links.append(".");
            cb = this->findChild<s_tqComboBox *>("tblefield"+QString::number(i));
            if (cb == 0)
            {
                DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(le->text());
        le = this->findChild<s_tqLineEdit *>("fwequatle2");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        if (!le->text().isEmpty())
        {
            links.append(".");
            QString ops[4] = {"s","r","m","d"};
            s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fwequatcb");
            if (cb == 0)
            {
                DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwlinkcb2");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append("\\\"^"); // в lineedit-е \", для БД кавычки надо ещё больше обрамить
        links.append(le->text());
        links.append("$\\\"");
        break;
    }
    case FW_MAXLINK:
    {
        s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb2");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaxlinkle");
        if (le == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwspecialcb2");
        if (tcb == 0)
        {
            DBGMSG(PublicClass::ER_DIRADD,__LINE__);
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
        DBGMSG(PublicClass::ER_DIRADD,__LINE__);
        return;
    }
    le->setText(links);
    emit closelinkdialog();
}
