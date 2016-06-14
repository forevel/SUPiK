#include "dir_adddialog.h"
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QFontMetrics>
#include <QInputDialog>
#include <QHeaderView>
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqspinbox.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../s_accessdialog.h"
#include "../messagebox.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../models/s_ntmodel.h"
#include "../../models/s_ncmodel.h"
#include "../../models/s_duniversal.h"

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
    dirBelongAliases.clear();
    dirBelongAliases.insert("Основной","ent");
    dirBelongAliases.insert("Altium","alt");
    dirBelongAliases.insert("Schemagee","sch");
    dirBelongAliases.insert("Solidworks","sol");
    dirBelongAliases.insert("Устройства","dev");
    dirBelongAliases.insert("Конструктивы","con");
    dirBelongAliases.insert("ОТ и ТБ","tb");
    dirBelongAliases.insert("СисАдмин","sys");
    dirBelongAliases.insert("Системный","sup");
    setupUI();
    FW_Links.clear();
    FW_Links << "0.Автонумерация" << "1.Фиксированное значение" << "2.Простая ссылка" << "3.Ссылка на несколько таблиц" << "4.Ссылка на дочерние элементы" << \
                "5.Значение с макс. ИД" << "6.Форматированное поле" << "7.Вычисляемое поле" << "8.Простое поле" << "9.Права доступа" << \
                "10.Форматированное число" << "11.Специальная ссылка" << "12.Булево поле" << "13.Вызов диалога редактирования строки" << "14.Конструктор ссылок" << \
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
    QString tmps;
    if (IsDir)
        tmps = dir.split("_").at(0);
    else
        tmps = dir;
    tmps.insert(0, "Редактор справочника: ");
    lbl = new s_tqLabel(tmps);
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
        PublicClass::ValueStruct vs;
        vs.Type = VS_STRING;
        vs.Value = "узч";
        cw->SetData(vs);
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
        cb->setEditable(true);
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
    QString tmpString, FullTblename, ShortTblename;
    QStringList sl;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    if (dirNameLE == 0)
    {
        DADDDBG;
        return;
    }
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        DADDDBG;
        return;
    }
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    if (dirAccessCW == 0)
    {
        DADDDBG;
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
        DADDDBG;
        return;
    }
    sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        DADDDBG;
        return;
    }
    int numfields = sb->value();
    QString dirdb = dirBelongAliases.value(dirB->currentText());
    QString tble = dirNameLE->text();
    // проверяем, существует ли таблица, на которую пытаются сослаться поля справочника
    sqlc.GetColumnsFromTable(dirdb, tble);
    if (sqlc.result)
    {
        // нет такой таблицы, продолжаем
        // для каждого поля "namele"
        for (i = 0; i < numfields; i++)
        {
            //      запоминаем его значение в список sl
            cbfield = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
            if (cbfield == 0)
            {
                DADDDBG;
                return;
            }
            sl << cbfield->currentText();
        }
        // делаем create table с запомненным списком sl
        sqlc.CreateTable(dirdb, tble, sl);
        if (sqlc.result)
        {
            DADDWARN;
            return;
        }
        DADDINFO("Таблица справочника создана успешно");
    }
    else // если таблица есть, надо проверить её на соответствие нашей структуре
    {
        // считываем структуру таблицы
        QStringList cmpsl = sqlc.GetColumnsFromTable(dirdb, tble);
        cmpsl.removeAll("id"+tble); // убираем из сравнения ИД, если он есть
        if (sqlc.result)
        {
            DADDDBG;
            return;
        }
        // убрать стандартные поля
        cmpsl.removeAll("idpers");
        cmpsl.removeAll("date");
        cmpsl.removeAll("deleted");
        // для каждого поля "cbfield"
        for (i=0; i<numfields; i++)
        {
            cbfield = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
            if (cbfield == 0)
            {
                DADDDBG;
                return;
            }
            //      проверяем наличие такого элемента в считанной структуре
            int cmpslidx = cmpsl.indexOf(cbfield->currentText());
            if (cmpslidx == -1)
                //      если такого нет, запоминаем в список sl
                sl << cbfield->currentText();
            else
                //      в противном случае удаляем элемент из списка непросмотренных
                cmpsl.removeAt(cmpslidx);
        }
        if ((sl.isEmpty()) && (cmpsl.isEmpty())); // если таблица уже соответствует нашей структуре, делать ничего не надо
        else
        {
            // есть такая таблица, надо спросить, не хотим ли её поменять?
            if (!(MessageBox2::question(this, "Таблица существует", "Таблица для справочника уже существует\nПерезаписать?")))
                return; // не готовы перезаписывать, значит, не будет соответствия между справочником и таблицей. Выход.
            // проверяем, если остались неохваченные поля в считанной структуре
            if (!cmpsl.isEmpty())
            {
                //      спрашиваем, если не удалять
                tmpString = cmpsl.join(",");
                tmpString = "В таблице существуют следующие поля:\n" + tmpString + "\nПерезаписать?";
                if (!(MessageBox2::question(this, "Поля существуют", tmpString)))
                    return; // выход, ибо опять же, не будет соответствия
            }
            // делаем alter table с запомненным списком sl
            sqlc.AlterTable(dirdb, tble, cmpsl, sl);
            if (sqlc.result)
            {
                DADDWARN;
                return;
            }
            DADDINFO("Таблица справочника изменена успешно");
        }
    }
    QStringList fl, vl;
    QStringList cmpfl, cmpvl;
    // соберём данные по столбцам tablefields для таблиц FullTbleName и ShortTbleName
    QStringList FullTbleDeleteList = tfl.TableColumn(FullTblename,"tablefields");
    QStringList ShortTbleDeleteList = tfl.TableColumn(ShortTblename, "tablefields");
    FullTbleDeleteList.removeAll("id"+tble);
    ShortTbleDeleteList.removeAll("id"+tble);
    // теперь для каждого значения в полях levalue, cbfield сравним значения cbfield со значениями в соответствующих списках
    for (i = 0; i < numfields; i++)
    {
        levalue = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        lename = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        cbfield = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if ((levalue == 0) || (lename == 0) || (cbfield == 0) || (chb == 0))
        {
            DADDDBG;
            return;
        }
        // удаляем из наших списков найденное в полях значение, если хоть одно останется, значит, надо будет его удалить потом
        FullTbleDeleteList.removeAll(cbfield->currentText());
        if (chb->isChecked())
            ShortTbleDeleteList.removeAll(cbfield->currentText());
    }
    // если есть значения на удаление
    if (!FullTbleDeleteList.isEmpty())
    {
        fl = QStringList() << "tablefields" << "tablename";
        // в цикле удаляем поля по списку
        while (!FullTbleDeleteList.isEmpty())
        {
            vl = QStringList() << FullTbleDeleteList.at(0) << FullTblename;
            sqlc.RealDeleteFromDB("sup","tablefields",fl,vl);
            if (sqlc.result)
            {
                DADDWARN;
                return;
            }
            FullTbleDeleteList.removeFirst();
        }
    }
    if (!ShortTbleDeleteList.isEmpty())
    {
        fl = QStringList() << "tablefields" << "table";
        // в цикле удаляем поля по списку
        while (!ShortTbleDeleteList.isEmpty())
        {
            vl = QStringList() << ShortTbleDeleteList.at(0) << ShortTblename;
            sqlc.RealDeleteFromDB("sup","tablefields",fl,vl);
            if (sqlc.result)
            {
                DADDWARN;
                return;
            }
            ShortTbleDeleteList.removeFirst();
        }
    }
    cmpfl << "tablename" << "tablefields";
    fl = QStringList() << "tablefields" << "table" << "keyfield" << "header" << "links" << "fieldsorder" << "tablename" << \
          "date" << "deleted" << "idpers";
    for (i = 0; i < numfields; i++)
    {
        levalue = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        lename = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        cbfield = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if ((levalue == 0) || (lename == 0) || (cbfield == 0) || (chb == 0))
        {
            DADDDBG;
            return;
        }
        vl.clear();
        cmpvl.clear();
        vl << cbfield->currentText() << dirdb+"."+tble  << "" << lename->text() << levalue->text(); // "" - на месте ключевого поля, т.к. ИД пишем далее
        tmpString = QString("%1").arg(i+1, 2, 10, QChar('0')); // i+1, т.к. на 0-м месте должен идти ИД
        vl << tmpString << FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
        cmpvl << FullTblename << cbfield->currentText();
        WriteToTfl(fl, vl, cmpfl, cmpvl);
        vl.replace(6, ShortTblename); // заменяем полное наименование на сокращённое
        cmpvl = QStringList() << ShortTblename << cbfield->currentText();
        if (chb->isChecked())
            WriteToTfl(fl, vl, cmpfl, cmpvl);
        else
        {
            sqlc.GetValueFromTableByFields("sup", "tablefields", "idtablefields", cmpfl, cmpvl);
            if (sqlc.result == 1); // нет такой записи, и хорошо
            else if (!sqlc.result) // иначе надо её удалить
            {
                int delidx = fl.indexOf("deleted");
                vl.replace(delidx, "1");
                QString id = sqlc.GetValueFromTableByFields("sup", "tablefields", "idtablefields", cmpfl, cmpvl);
                sqlc.UpdateValuesInTable("sup", "tablefields", fl, vl, "idtablefields", id);
                if (sqlc.result)
                {
                    DADDWARN;
                    return;
                }
            }
        }
    }
    // теперь записать id
    vl.clear();
    cmpvl.clear();
    vl << "id"+tble << dirdb+"."+tble << "v" << "ИД" << "4.19..7";
    tmpString = QString("%1").arg(0, 2, 10, QChar('0'));
    vl << tmpString << FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
    // сначала пишем полный вариант
    cmpvl << FullTblename << "id"+tble;
    WriteToTfl(fl, vl, cmpfl, cmpvl);
    // теперь пишем сокращённый вариант
    vl.replace(6, ShortTblename); // заменяем полное наименование на сокращённое
    cmpvl = QStringList() << ShortTblename << "id"+tble;
    WriteToTfl(fl, vl, cmpfl, cmpvl);

    // теперь проверим данные о справочнике в каталоге справочников
    tble = "dirlist";
    if (IsDir)
    {
        QString tmpdir = dirAliasLE->text();
        int diridx = tmpdir.indexOf("_полн");
        if (diridx != -1)
            tmpdir.chop(5);
        fl.clear();
        vl.clear();
        fl << "dirlist" << "access" << "deleted" << "date" << "idpers";
        vl << tmpdir << dirAccessCW->Value() << "0" << pc.DateTime << QString::number(pc.idPers);
        tmpString = sqlc.GetValueFromTableByField("sup", tble, "dirlist", "dirlist", tmpdir);
        if (tmpString.isEmpty())
        {
            tmpString = sqlc.InsertValuesToTable("sup", tble, fl, vl);
            if (sqlc.result)
            {
                DADDWARN;
                return;
            }
        }
        else
        {
            tmpString = sqlc.UpdateValuesInTable("sup", tble, fl, vl, "dirlist", tmpdir);
            if (sqlc.result)
            {
                DADDWARN;
                return;
            }
        }
    }
    DADDINFO("Записано успешно!");
    this->close();
}

void dir_adddialog::WriteToTfl(QStringList fl, QStringList vl, QStringList cmpfl, QStringList cmpvl)
{
    QString id = sqlc.GetValueFromTableByFields("sup", "tablefields", "idtablefields", cmpfl, cmpvl);
    if (sqlc.result == SQLC_EMPTY) // нет такой записи
        sqlc.InsertValuesToTable("sup", "tablefields", fl, vl);
    else
        sqlc.UpdateValuesInTable("sup", "tablefields", fl, vl, "idtablefields", id);
    if (sqlc.result)
    {
        DADDWARN;
        return;
    }
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
            DADDDBG;
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            DADDDBG;
            return;
        }
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            DADDDBG;
            return;
        }
        ll->setVisible(true);
        cb = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        if (cb == 0)
        {
            DADDDBG;
            return;
        }
        cb->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            DADDDBG;
            return;
        }
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            DADDDBG;
            return;
        }
        le->setVisible(true);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            DADDDBG;
            return;
        }
        pb->setVisible(true);
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DADDDBG;
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
            DADDDBG;
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
        if (ll == 0)
        {
            DADDDBG;
            return;
        }
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
        if (ll == 0)
        {
            DADDDBG;
            return;
        }
        ll->setVisible(false);
        cb = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB");
        if (cb == 0)
        {
            DADDDBG;
            return;
        }
        cb->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        if (le == 0)
        {
            DADDDBG;
            return;
        }
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        if (le == 0)
        {
            DADDDBG;
            return;
        }
        le->setVisible(false);
        pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
        if (pb == 0)
        {
            DADDDBG;
            return;
        }
        pb->setVisible(false);
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DADDDBG;
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
        DADDDBG;
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
        DADDDBG;
        return;
    }
    QStringList dirColumns = sqlc.GetColumnsFromTable(dirBelongAliases[dirB->currentText()], tblename);
    dirColumns.removeAt(dirColumns.indexOf("id"+tblename)); // убираем ИД, т.к. с ним разговор особый
    if (sqlc.result)
    {
        DADDINFO("Не найдена таблица");
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
            DADDDBG;
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
    case FW_DLINK+FW_COUNT:
    {
        cb1name = "tble0";
        cb2name = "tblefield0";
        break;
    }
    case FW_DLINK+FW_COUNT+1:
    {
        cb1name = "tble1";
        cb2name = "tblefield1";
        break;
    }
    case FW_DLINK+FW_COUNT+2:
    {
        cb1name = "tble2";
        cb2name = "tblefield2";
        break;
    }
    case FW_DLINK+FW_COUNT+3:
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
    lsl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(dir), "fieldsorder", true);
    if (sqlc.result)
    {
        DADDWARN;
        return;
    }
    if (IsDir)
    {
        QString tmps = dir;
        tmps.replace("_полн","_сокращ");
        lslshort = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(tmps), "fieldsorder", true);
    }
    if (lsl.size() == 0)
    {
        DADDWARN;
        return;
    }
    s_tqLineEdit *dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    if (dirNameLE == 0)
    {
        DADDDBG;
        return;
    }
    s_tqLineEdit *dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        DADDDBG;
        return;
    }
    s_tqComboBox *dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirBelongCB == 0)
    {
        DADDDBG;
        return;
    }
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    if (dirAccessCW == 0)
    {
        DADDDBG;
        return;
    }
    s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        DADDDBG;
        return;
    }
    QString tmpString = dirBelongAliases.key(lsl.at(0).at(0).split(".").at(0));
    if (tmpString.isEmpty())
    {
        DADDDBG;
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
            DADDWARN;
            return;
        }
        dirAccessCW->SetValue(values.at(0));
        dirAccessCW->setEnabled(true);
    }
    else
        dirAliasLE->setText(dir);
    // для редактирования таблиц (не справочников) права не нужны, они по умолчанию системные
    for (int i=0; i<lsl.size(); i++)
    {
        if (lsl.at(i).at(2) == "ИД")
        {
            lsl.removeAt(i);
            break;
        }
    }
    sb->setValue(lsl.size());
    TbleNameChanged(dirNameLE->text()); // принудительно имитируем изменение имени таблицы для заполнения комбобоксов
    for (int i = 0; i < lsl.size(); i++)
    {
        if (i > 16)
            break;
        cbf = this->findChild<s_tqComboBox *>("field"+QString::number(i)+"CB"); // имя поля
        if (cbf == 0)
        {
            DADDDBG;
            return;
        }
        lev = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE"); // строка 1
        if (lev == 0)
        {
            DADDDBG;
            return;
        }
        len = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE"); // строка 2
        if (len == 0)
        {
            DADDDBG;
            return;
        }
        s_tqCheckBox *chb = this->findChild<s_tqCheckBox *>("short"+QString::number(i));
        if (chb == 0)
        {
            DADDDBG;
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
        DADDDBG;
        return;
    }
    if (dirNameLE->text().isEmpty())
    {
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
        {
            DADDDBG;
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
        DADDDBG;
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
                     "5.Счётчик" << "6.Поле выбора с вводом" << "7.Поле с рамкой" << "8.Поле бинарного ввода";
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
    QSqlQuery get_tables(sqlc.GetDB("sup"));
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
        DADDDBG;
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

    // 1. FW_NUMBER
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

    // 2. FW_LINK
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

    // 3. FW_DLINK
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
        TbleCB->setAData(FW_DLINK+FW_COUNT+i); // +20 - чтобы точно перекрыть диапазон возможных вариантов полей
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

    // 4. FW_ALLINK
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

    // 5. FW_MAXLINK
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

    // 6. FW_MASKED
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

    // 7. FW_EQUAT
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

    // 10. FW_FNUMBER
    vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Количество знаков целой части");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    NumberSB = new s_tqSpinBox;
    NumberSB->setObjectName("fnumber");
    NumberSB->setDecimals(0);
    NumberSB->setSingleStep(1);
    NumberSB->setMaximum(9);
    NumberSB->setMinimum(0);
    hlyout->addWidget(NumberSB);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Количество знаков дробной части");
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    NumberSB = new s_tqSpinBox;
    NumberSB->setObjectName("fnumber2");
    NumberSB->setDecimals(0);
    NumberSB->setSingleStep(1);
    NumberSB->setMaximum(5);
    NumberSB->setMinimum(0);
    hlyout->addWidget(NumberSB);
    vlyout->addLayout(hlyout);
    wdgts[10]->setLayout(vlyout);

    // 11. FW_SPECIAL
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

    // 19. FW_ID
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
        DADDDBG;
        return;
    }
    switch (FD)
    {
    case FD_SIMPLE:
    case FD_DISABLED:
    case FD_SIMGRID:
    {
        tmpStringList << FW_Links.at(FW_AUTONUM) << FW_Links.at(FW_NUMBER) << FW_Links.at(FW_MAXLINK) << FW_Links.at(FW_EQUAT) << FW_Links.at(FW_PLAIN) << FW_Links.at(FW_ID);
        break;
    }
    case FD_LINEEDIT:
    {
        tmpStringList << FW_Links.at(FW_MASKED) << FW_Links.at(FW_PLAIN);
        break;
    }
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        tmpStringList << FW_Links.at(FW_LINK) << FW_Links.at(FW_DLINK) << FW_Links.at(FW_ALLINK) << FW_Links.at(FW_RIGHTS) << FW_Links.at(FW_FNUMBER) \
                      << FW_Links.at(FW_SPECIAL) << FW_Links.at(FW_2CD  ) << FW_Links.at(FW_LLINK) << FW_Links.at(FW_FLINK) \
                      << FW_Links.at(FW_ILINK) << FW_Links.at(FW_FLLINK) << FW_Links.at(FW_DATE);
        break;
    }
    case FD_COMBO:
    {
        tmpStringList << FW_Links.at(FW_LINK) << FW_Links.at(FW_ALLINK);
        break;
    }
    case FD_SPIN:
    {
        tmpStringList << FW_Links.at(FW_FNUMBER);
        break;
    }
    case FD_CHECK:
    {
        tmpStringList << FW_Links.at(FW_BOOL);
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
        DADDDBG;
        return;
    }
    sw->setCurrentIndex(wdgtsidx);
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        DADDDBG;
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
            {
                DADDDBG;
                return;
            }
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
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwlinkcb2");
            if (tcb == 0)
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(4));
        }
        break;
    }
    case FW_SPECIAL:
    {
        if (links.size()>4)
        {
            s_tqComboBox *tcb = this->findChild<s_tqComboBox *>("fwspecialcb");
            {
                DADDDBG;
                return;
            }
                return;
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwspecialcb2");
            if (tcb == 0)
            {
                DADDDBG;
                return;
            }
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
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwallinkcb2");
            if (tcb == 0)
            {
                DADDDBG;
                return;
            }
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
                {
                    DADDDBG;
                    return;
                }
                cb->setCurrentText(links.at(0));
                cb = this->findChild<s_tqComboBox *>("tblefield"+QString::number(i));
                if (cb == 0)
                {
                    DADDDBG;
                    return;
                }
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
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(3));
            tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb2");
            if (tcb == 0)
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(4));
            tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
            if (tcb == 0)
            {
                DADDDBG;
                return;
            }
            tcb->setCurrentText(links.at(5));
            if (links.size()>6)
            {
                s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaxlinkle");
                if (le == 0)
                {
                    DADDDBG;
                    return;
                }
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
            {
                DADDDBG;
                return;
            }
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
            {
                DADDDBG;
                return;
            }
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
            {
                DADDDBG;
                return;
            }
            le->setText(links.at(4));
            if (links.size()>6)
            {
                le = this->findChild<s_tqLineEdit *>("fwequatle2");
                if (le == 0)
                {
                    DADDDBG;
                    return;
                }
                le->setText(links.at(6));
                s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fwequatcb");
                if (cb == 0)
                {
                    DADDDBG;
                    return;
                }
                QString op = links.at(5);
                if (op == "s")  cb->setCurrentIndex(0);
                if (op == "r")  cb->setCurrentIndex(1);
                if (op == "m")  cb->setCurrentIndex(2);
                if (op == "d")  cb->setCurrentIndex(3);
            }
        }
        break;
    }
    case FW_FNUMBER:
    {
        if (links.size()>3)
        {
            s_tqSpinBox *NumberSB = this->findChild<s_tqSpinBox *>("fnumber");
            if (NumberSB == 0)
            {
                DADDDBG;
                return;
            }
            NumberSB->setValue(links.at(3).count("n", Qt::CaseSensitive));
            NumberSB = this->findChild<s_tqSpinBox *>("fnumber2");
            if (NumberSB == 0)
            {
                DADDDBG;
                return;
            }
            NumberSB->setValue(links.at(3).count("d", Qt::CaseSensitive));
        }
        break;
    }
    default:
        break;
    }
}

// сборка строки ссылки из элементов выбора и выход из диалога конструктора

void dir_adddialog::ConstructLink()
{
    QString links;
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("dtypecb");
    if (cb == 0)
    {
        DADDDBG;
        return;
    }
    links.append(QString::number(cb->currentIndex()));
    links.append(".");
    cb = this->findChild<s_tqComboBox *>("ltypecb");
    if (cb == 0)
    {
        DADDDBG;
        return;
    }
    links.append(QString::number(FW_Links.indexOf(cb->currentText())));
    links.append(".");
    s_tqSpinBox *spb = this->findChild<s_tqSpinBox *>("dependsspb");
    if (spb == 0)
    {
        DADDDBG;
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
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwallinkcb2");
        if (tcb == 0)
        {
            DADDDBG;
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
                DADDDBG;
                return;
            }
            if (cb->currentText().isEmpty())
                break;
            links.append(cb->currentText());
            links.append(".");
            cb = this->findChild<s_tqComboBox *>("tblefield"+QString::number(i));
            if (cb == 0)
            {
                DADDDBG;
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
            DADDDBG;
            return;
        }
        links.append(le->text());
        le = this->findChild<s_tqLineEdit *>("fwequatle2");
        if (le == 0)
        {
            DADDDBG;
            return;
        }
        if (!le->text().isEmpty())
        {
            links.append(".");
            QString ops[4] = {"s","r","m","d"};
            s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fwequatcb");
            if (cb == 0)
            {
                DADDDBG;
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
            DADDDBG;
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
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwlinkcb2");
        if (tcb == 0)
        {
            DADDDBG;
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
            DADDDBG;
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
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb2");
        if (tcb == 0)
        {
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
        if (tcb == 0)
        {
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fwmaxlinkle");
        if (le == 0)
        {
            DADDDBG;
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
            DADDDBG;
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
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        links.append(".");
        tcb = this->findChild<s_tqComboBox *>("fwspecialcb2");
        if (tcb == 0)
        {
            DADDDBG;
            return;
        }
        links.append(tcb->currentText());
        break;
    }
    case FW_FNUMBER:
    {
        QString tmps;
        spb = this->findChild<s_tqSpinBox *>("fnumber");
        if (spb == 0)
        {
            DADDDBG;
            return;
        }
        tmps.fill('n', spb->value());
        links.append(tmps);
        spb = this->findChild<s_tqSpinBox *>("fnumber2");
        if (spb == 0)
        {
            DADDDBG;
            return;
        }
        tmps.fill('d', spb->value());
        links.append(tmps);
        break;
    }
    default:
        break;
    }
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        DADDDBG;
        return;
    }
    le->setText(links);
    emit closelinkdialog();
}
