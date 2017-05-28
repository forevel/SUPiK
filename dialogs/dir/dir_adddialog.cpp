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
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/wd_func.h"
#include "../gen/accessdialog.h"
#include "../gen/messagebox.h"
#include "../../gen/s_sql.h"
#include "../../gen/client.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
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
                "15.Ссылка на файл" << "16.Ссылка на каталог" << "17.Ссылка на элемент внутри файла" << "18.Выбор даты" << "19.Автодополнение нулями" << "20.Шифрование" << \
                "21.Выбор даты и времени" << "22.Текст с изображением";
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
    lbl = new s_tqLabel("Имя справочника(таблицы)");
    dlg1Layout->addWidget(lbl, 0, 0);
    le = new s_tqLineEdit;
    le->setObjectName("dirAlias");
    connect(le, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    dlg1Layout->addWidget(le, 0, 1);
    lbl = new s_tqLabel("Количество полей (исключая id и deleted, но включая idpers и date)");
    spb = WDFunc::NewSPB(this, "dirFieldNum", 1, FSIZE, 1, 0);
    WDFunc::SetSPBData(this, "dirFieldNum", 1);
    connect(spb, SIGNAL(valueChanged(double)), this, SLOT(updateTWFields(double)));
    dlg1Layout->addWidget(lbl, 1, 0);
    dlg1Layout->addWidget(spb, 1, 1);
    lbl = new s_tqLabel("Тип справочника");
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
    cb = WDFunc::NewCB(this, "dirBelong", tmpSL);
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
    le = WDFunc::NewLE(this, "dirname");
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

    QVBoxLayout *vlyout = new QVBoxLayout;
    QDialog *tdlg = new QDialog;
    int i = 0;
    int tabcount = 1;
    while (i < FSIZE)
    {
        QList<QWidget *> wl;
        lbl = new s_tqLabel("Поле №" + QString::number(i+1) + " :");
        wl << lbl;
        le = WDFunc::NewLE(this, "name." + QString::number(i));
        adjustFieldSize(le, 20);
        wl << le;
        lbl = new s_tqLabel("Системное имя:");
        wl << lbl;
        cb = WDFunc::NewCB(this, "field."+QString::number(i));
        cb->setEditable(true);
        adjustFieldSize(cb, 15);
        wl << cb;
        lbl = new s_tqLabel("Описание (links):");
        wl << lbl;
        le = WDFunc::NewLE(this, "value." + QString::number(i));
        adjustFieldSize(le, 10);
        le->setEnabled(false);
        wl << le;
        pb = new s_tqPushButton("Конструктор");
        pb->setObjectName("valueconstr." + QString::number(i));
        adjustFieldSize(pb, 4);
        connect(pb, SIGNAL(clicked()), this, SLOT(FPBPressed()));
        wl << pb;
        s_tqCheckBox *chb = new s_tqCheckBox;
        chb->setObjectName("short."+QString::number(i));
        chb->setToolTip("Учёт в кратком виде справочника");
        wl << chb;
        addLineToDlg(vlyout, wl, i);
        ++i;
        if (i%FSZPR == 0) // дошли до очередного предела количества полей на одной вкладке
        {
            vlyout->addStretch(200);
            tdlg->setLayout(vlyout);
            mainTW->addTab(tdlg, "Поля "+QString::number(tabcount));
            ++tabcount;
            tdlg = new QDialog;
            vlyout = new QVBoxLayout;
        }
    }
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
    QString tmpString, FullTblename, ShortTblename;
    QStringList sl;
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    QString tmps, dirAlias;
    WDFunc::LEData(this, "dirAlias", dirAlias);
    if (IsDir)
    {
        FullTblename = dirAlias+"_полн";
        ShortTblename = dirAlias+"_сокращ";
    }
    else
        FullTblename = dirAlias;
    double numfields;
    WDFunc::SPBData(this, "dirFieldNum", numfields);
    WDFunc::CBData(this, "dirBelong", tmps);
    QString dirdb = dirBelongAliases.value(tmps);
    QString tble;
    WDFunc::LEData(this, "dirname", tble);
    // проверяем, существует ли таблица, на которую пытаются сослаться поля справочника
    sqlc.GetColumnsFromTable(dirdb, tble);
    if (sqlc.result)
    {
        // нет такой таблицы, продолжаем
        // для каждого поля "namele"
        for (i = 0; i < numfields; i++)
        {
            //      запоминаем его значение в список sl
            WDFunc::CBData(this, "field."+QString::number(i), tmps);
            sl << tmps;
        }
        // делаем create table с запомненным списком sl
        sqlc.CreateTable(dirdb, tble, sl);
        if (sqlc.result)
        {
            WARNMSG("");
            return;
        }
        MessageBox2::information(this, "Внимание", "Таблица справочника создана успешно");
    }
    else // если таблица есть, надо проверить её на соответствие нашей структуре
    {
        // считываем структуру таблицы
        QStringList cmpsl = sqlc.GetColumnsFromTable(dirdb, tble);
        cmpsl.removeAll("id"+tble); // убираем из сравнения ИД, если он есть
        if (sqlc.result)
        {
            DBGMSG;
            return;
        }
        // убрать стандартные поля
        cmpsl.removeAll("idpers");
        cmpsl.removeAll("date");
        cmpsl.removeAll("deleted");
        // для каждого поля "cbfield"
        for (i=0; i<numfields; i++)
        {
            WDFunc::CBData(this, "field."+QString::number(i), tmps);
            //      проверяем наличие такого элемента в считанной структуре
            int cmpslidx = cmpsl.indexOf(tmps);
            if (cmpslidx == -1)
                //      если такого нет, запоминаем в список sl
                sl << tmps;
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
                WARNMSG("");
                return;
            }
            MessageBox2::information(this, "Внимание", "Таблица справочника изменена успешно");
        }
    }
    QStringList fl, vl;
    QStringList cmpfl, cmpvl;
    // соберём данные по столбцам tablefields для таблиц FullTbleName и ShortTbleName
    QStringList FullTbleDeleteList, ShortTbleDeleteList;
    QString table = "tablefields";
    tfl.TableColumn(FullTblename,table, FullTbleDeleteList);
    tfl.TableColumn(ShortTblename, table, ShortTbleDeleteList);
    FullTbleDeleteList.removeAll("id"+tble);
    ShortTbleDeleteList.removeAll("id"+tble);
    // теперь для каждого значения в полях levalue, cbfield сравним значения cbfield со значениями в соответствующих списках
    for (i = 0; i < numfields; i++)
    {
        bool isChecked;
        WDFunc::CBData(this, "field."+QString::number(i), tmps);
        WDFunc::ChBData(this, "short."+QString::number(i), isChecked);
        // удаляем из наших списков найденное в полях значение, если хоть одно останется, значит, надо будет его удалить потом
        FullTbleDeleteList.removeAll(tmps);
        if (isChecked)
            ShortTbleDeleteList.removeAll(tmps);
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
                WARNMSG("");
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
                WARNMSG("");
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
        QString levalue, lename, cbtext;
        bool isChecked;
        WDFunc::LEData(this, "value."+QString::number(i), levalue);
        WDFunc::LEData(this, "name."+QString::number(i), lename);
        WDFunc::CBData(this, "field."+QString::number(i), cbtext);
        WDFunc::ChBData(this, "short."+QString::number(i), isChecked);
        vl.clear();
        vl << cbtext << dirdb+"."+tble << "" << lename << levalue; // "" - на месте ключевого поля, т.к. ИД пишем далее
        tmpString = QString("%1").arg(i+1, 2, 10, QChar('0')); // fieldsorder = i+1, т.к. на 0-м месте должен идти ИД
        vl << tmpString << FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
        cmpvl.clear();
        cmpvl << FullTblename << cbtext;
        WriteToTfl(fl, vl, cmpfl, cmpvl);
        vl.replace(6, ShortTblename); // заменяем полное наименование на сокращённое
        cmpvl = QStringList() << ShortTblename << cbtext;
        if (isChecked)
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
                    WARNMSG("");
                    return;
                }
            }
        }
    }
    // запись в tablefields полей ДАТА и ИДПОЛЬЗ
    cmpvl = QStringList() << FullTblename << "date";
    int tmpi = numfields;
    vl = QStringList() << "date" << dirdb+"."+tble << "" << "Дата" << "2.21.." << QString("%1").arg(tmpi+1, 2, 10, QChar('0')) << \
                          FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
    WriteToTfl(fl, vl, cmpfl, cmpvl);
    cmpvl = QStringList() << FullTblename << "idpers";
    vl = QStringList() << "idpers" << dirdb+"."+tble << "" << "ИДПольз" << "2.2..Персонал_полн.ФИО" << QString("%1").arg(tmpi+2, 2, 10, QChar('0')) << \
                          FullTblename << pc.DateTime << "0" << QString::number(pc.idPers);
    WriteToTfl(fl, vl, cmpfl, cmpvl);

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
        QString tmpdir = dirAlias;
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
                WARNMSG("");
                return;
            }
        }
        else
        {
            tmpString = sqlc.UpdateValuesInTable("sup", tble, fl, vl, "dirlist", tmpdir);
            if (sqlc.result)
            {
                WARNMSG("");
                return;
            }
        }
    }
    MessageBox2::information(this, "Внимание", "Записано успешно!");
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
        WARNMSG("");
        return;
    }
}

void dir_adddialog::updateTWFields(double dfn)
{
    int i = 0;
    int fn = static_cast<int>(dfn);
    while (i < fn)
    {
        s_tqWidget *w = this->findChild<s_tqWidget *>("wdgt."+QString::number(i));
        if (w == 0)
        {
            DBGMSG;
            return;
        }
        w->setVisible(true);
        ++i;
    }
    while (i < FSIZE)
    {
        s_tqWidget *w = this->findChild<s_tqWidget *>("wdgt."+QString::number(i));
        if (w == 0)
        {
            DBGMSG;
            return;
        }
        w->setVisible(false);
        i++;
    }
}

void dir_adddialog::setAccessRights()
{
    AccessDialog *accessdialog = new AccessDialog;
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
        DBGMSG;
        return;
    }
    le->setText(rights);
}

void dir_adddialog::addLineToDlg(QVBoxLayout *lyt, QList<QWidget *> wl, int idx)
{
    s_tqWidget *w = new s_tqWidget;
    QHBoxLayout *lyout = new QHBoxLayout;
    for (int i = 0; i < wl.size(); i++)
        lyout->addWidget(wl.at(i));
    lyout->setStretch(1, 10);
    lyout->setStretch(5, 10);
    w->setLayout(lyout);
    w->setObjectName("wdgt."+QString::number(idx));
    lyt->addWidget(w);
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
    QString tmps;
    WDFunc::CBData(this, "dirBelong", tmps);
    QStringList dirColumns = sqlc.GetColumnsFromTable(dirBelongAliases[tmps], tblename);
    dirColumns.removeAt(dirColumns.indexOf("id"+tblename)); // убираем ИД, т.к. с ним разговор особый
    if (sqlc.result)
    {
//        MessageBox2::information(this, "Внимание", "Не найдена таблица");
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
        s_tqComboBox *cb = this->findChild<s_tqComboBox *>("field."+QString::number(i));
        if (cb == 0)
        {
            DBGMSG;
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

void dir_adddialog::PixTEChBClicked(bool value)
{
    QString chbname = sender()->objectName();
    if (chbname == "fwpixteprefixenchb")
        WDFunc::SetEnabled(this, "fwpixteprefix", !value);
    if (chbname == "fwpixtesuffixenchb")
        WDFunc::SetEnabled(this, "fwpixtesuffix", !value);
}

void dir_adddialog::TbleChoosed()
{
    QString objname = sender()->objectName();
    QStringList sl = objname.split(".");
    if (sl.size() < 2)
    {
        DBGMSG;
        return;
    }
    bool ok;
    int Type = sl.at(1).toInt(&ok);
    if (!ok)
    {
        DBGMSG;
        return;
    }
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
    cb = static_cast<s_tqComboBox *>(sender());
    cb2 = this->findChild<s_tqComboBox *>(cb2name);
    if (cb2 == 0)
    {
        DBGMSG;
        return;
    }
    tfl.tableheaders(cb->currentText(), sl);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    QStringListModel *cb2mdl = new QStringListModel;
    cb2mdl->setStringList(sl);
    cb2->setModel(cb2mdl);
    if (Type == FW_MAXLINK)
    {
        s_tqComboBox *cb3 = this->findChild<s_tqComboBox *>("fwmaxlinkcb3");
        if (cb3 == 0)
        {
            DBGMSG;
            return;
        }
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
    s_tqLineEdit *len;
    QList<QStringList> lsl, lslshort;
    QStringList fl;
    fl << "table" << "tablefields" << "header" << "links";
    lsl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(dir), "fieldsorder", true);
    if (sqlc.result)
    {
        WARNMSG("");
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
        WARNMSG("");
        return;
    }
    s_tqLineEdit *dirNameLE = this->findChild<s_tqLineEdit *>("dirname");
    s_tqLineEdit *dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    s_tqComboBox *dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
    s_tqChooseWidget *dirAccessCW = this->findChild<s_tqChooseWidget *>("dirAccess");
    s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if ((dirNameLE == 0) || (dirAliasLE == 0) || (dirBelongCB == 0) || (dirAccessCW == 0) || (sb == 0))
    {
        DBGMSG;
        return;
    }
    QString tmpString = dirBelongAliases.key(lsl.at(0).at(0).split(".").at(0));
    if (tmpString.isEmpty())
    {
        DBGMSG;
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
        QStringList values;
        QString table = dirtype+"_полн";
        QString field = "Наименование";
        tfl.valuesbyfield(table,fields,field,tmpdir, values);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
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
        if (i > FSIZE)
            break;
        len = this->findChild<s_tqLineEdit *>("name."+QString::number(i)); // строка 2
        WDFunc::SetCBData(this, "field."+QString::number(i), lsl.at(i).at(1)); // tablefields
        WDFunc::SetLEData(this, "name."+QString::number(i), lsl.at(i).at(2)); // header
        QString tmps = lsl.at(i).at(3); // links
        if ((lsl.at(i).at(1) == "date") || (lsl.at(i).at(1) == "idpers"))
        {
            WDFunc::SetEnabled(this, "field."+QString::number(i), false);
            WDFunc::SetEnabled(this, "name."+QString::number(i), false);
            WDFunc::SetEnabled(this, "valueconstr." + QString::number(i), false); // "Конструктор"
        }
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
        WDFunc::SetLEData(this, "value."+QString::number(i), tmps); // links
        for (int j=0; j<lslshort.size(); j++) // проверка наличия в сокращённой таблице такого же элемента, как и в полной
        {
            if (lslshort.at(j).at(2) == len->text())
            {
                WDFunc::SetChBData(this, "short."+QString::number(i), true);
                break;
            }
        }
    }
}

void dir_adddialog::transliteDirName()
{
    QString tmps;
    WDFunc::LEData(this, "dirname", tmps);
    if (tmps.isEmpty())
    {
        WDFunc::LEData(this, "dirAlias", tmps);
        QString tmpString = pc.getTranslit(tmps);
        WDFunc::SetLEData(this, "dirname", tmpString);
    }
}

// Вызов конструктора ссылок

void dir_adddialog::FPBPressed()
{
    QString pbname = sender()->objectName();
    QStringList sl = pbname.split(".");
    if (sl.size() < 2)
    {
        DBGMSG;
        return;
    }
    bool ok;
    idx = sl.at(1).toInt(&ok);
    if (!ok)
    {
        DBGMSG;
        return;
    }
    QDialog *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    // заполняем элементы конструктора ссылок
    QString tmps;
    WDFunc::LEData(this, "value."+QString::number(idx), tmps);
    QStringList links = tmps.split("."); // формируем links

    s_tqLabel *lbl1 = new s_tqLabel ("Тип поля");
    s_tqLabel *lbl2 = new s_tqLabel ("Тип ссылки");
    // делегаты
    QStringList tmpStringList = QStringList() << "0.Простое поле" << "1.Поле ввода" << "2.Поле выбора" << "3.Выпадающий список" << "4.Неактивное поле" <<  \
                     "5.Счётчик" << "6.Поле выбора с вводом" << "7.Поле с рамкой" << "8.Поле бинарного ввода" << "9.Многострочное поле";
    s_tqComboBox *dtypecb = WDFunc::NewCB(this, "dtypecb", tmpStringList);
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

    s_tqLabel *lbl = new s_tqLabel("Зависимость от элемента");
    hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqSpinBox *spb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (spb == 0)
    {
        DBGMSG;
        return;
    }
    int NumFields = spb->value();
    spb = WDFunc::NewSPB(this, "dependsspb", -1, NumFields, 1, 0);
    hlyout->addWidget(spb);
    if ((links.size()>2) && (!links.at(2).isEmpty()))
        spb->setValue(links.at(2).toInt());
    lyout->addLayout(hlyout);

    s_tqStackedWidget *sw = new s_tqStackedWidget;
    sw->setObjectName("linksconstrsw");
    lyout->addWidget(sw);
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
    dlg->show(); // to let the widgets appear
    if (links.size()>1)
    {
        dtypecb->setCurrentIndex(links.at(0).toInt());
        QString LTypeCBString = FW_Links.at(links.at(1).toInt());
        ltypecb->setCurrentText(LTypeCBString);
    }
    else
        DTypeCBIndexChanged(0); // если links нет, то хотя бы установить выбор ссылки по 0-му делегату
    dlg->exec();
}

s_tqWidget *dir_adddialog::SetWidget(int FType)
{
    QVBoxLayout *vlyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl;
    s_tqSpinBox *spb;
    s_tqWidget *w = new s_tqWidget;
    s_tqComboBox *cb;
    s_tqLineEdit *le;
    QStringList vls = sqlc.GetValuesFromTableByColumn("sup", "tablefields", "tablename", "tablename");
    vls.removeDuplicates();
    switch (FType)
    {
    case FW_NUMBER:
    {
        lbl = new s_tqLabel("Число");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl, Qt::AlignRight);
        spb = WDFunc::NewSPB(this, "number", 0, 999999, 1, 0);
        hlyout->addWidget(spb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_LINK:
    {
        lbl = new s_tqLabel("Таблица");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "tcb."+QString::number(FW_LINK), vls);
        connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(TbleChoosed()));
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwlinkcb2");
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_DLINK:
    {
        vls.insert(0,""); // добавление пустого элемента, ибо в DLINK возможно отсутствие значения
        for (int i = 0; i < 4; i++)
        {
            hlyout = new QHBoxLayout;
            lbl = new s_tqLabel("Таблица"+QString::number(i));
            hlyout->addWidget(lbl);
            hlyout->setAlignment(lbl,Qt::AlignRight);
            cb = WDFunc::NewCB(this, "tble."+QString::number(FW_DLINK+FW_COUNT+i), vls); // +20 - чтобы точно перекрыть диапазон возможных вариантов полей
            connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(TbleChoosed()));
            hlyout->addWidget(cb);
            vlyout->addLayout(hlyout);
            hlyout = new QHBoxLayout;
            lbl = new s_tqLabel("Поле"+QString::number(i));
            hlyout->addWidget(lbl);
            hlyout->setAlignment(lbl,Qt::AlignRight);
            cb = WDFunc::NewCB(this, "tblefield."+QString::number(i));
            hlyout->addWidget(cb);
            vlyout->addLayout(hlyout);
        }
        break;
    }
    case FW_ALLINK:
    {
        lbl = new s_tqLabel("Таблица");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "tcb."+QString::number(FW_ALLINK), vls);
        connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(TbleChoosed()));
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwallinkcb2");
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_MAXLINK:
    {
        lbl = new s_tqLabel("Таблица");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "tcb."+QString::number(FW_MAXLINK), vls);
        connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(TbleChoosed()));
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле значения");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwmaxlinkcb2");
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле сравнения");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwmaxlinkcb3");
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Значение (опц.)");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        le = WDFunc::NewLE(this, "fwmaxlinkle");
        hlyout->addWidget(le);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_MASKED:
    {
        lbl = new s_tqLabel("Регулярное выражение");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        le = WDFunc::NewLE(this, "fwmaskedle");
        le->setToolTip("Пример: [0-9]{0,2}-[0-9]{0,9}");
        hlyout->addWidget(le);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_EQUAT:
    {
        lbl = new s_tqLabel("Выражение1");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        le = WDFunc::NewLE(this, "fwequatle");
        hlyout->addWidget(le);
        vlyout->addLayout(hlyout);
        QStringList ops = QStringList() << "+" << "-" << "*" << "/";
        cb = WDFunc::NewCB(this, "fwequatcb", ops);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Операция");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Выражение2");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        le = WDFunc::NewLE(this, "fwequatle2");
        hlyout->addWidget(le);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_FNUMBER:
    {
        lbl = new s_tqLabel("Количество знаков целой части");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        spb = WDFunc::NewSPB(this, "fnumber", 0, 9, 1, 0);
        hlyout->addWidget(spb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Количество знаков дробной части");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        spb = WDFunc::NewSPB(this, "fnumber2", 0, 5, 1, 0);
        hlyout->addWidget(spb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_SPECIAL:
    {
        lbl = new s_tqLabel("Таблица");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "tcb."+QString::number(FW_SPECIAL), vls);
        connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(TbleChoosed()));
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Поле");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwspecialcb2");
        hlyout->addWidget(cb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_ID:
    {
        lbl = new s_tqLabel("Размер поля");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl, Qt::AlignRight);
        spb = WDFunc::NewSPB(this, "id", 0, 9, 1, 0);
        spb->setValue(7);
        hlyout->addWidget(spb);
        vlyout->addLayout(hlyout);
        break;
    }
    case FW_PIXTE:
    {
        lbl = new s_tqLabel("Префикс пути к файлам");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl, Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwpixteprefix", Cli->PathPrefixes);
        hlyout->addWidget(cb);
        s_tqCheckBox *chb = WDFunc::NewChB(this, "fwpixteprefixenchb", "Нет");
        connect(chb,SIGNAL(toggled(bool)),this,SLOT(PixTEChBClicked(bool)));
        hlyout->addWidget(chb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
        lbl = new s_tqLabel("Суффикс пути к файлам");
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl, Qt::AlignRight);
        cb = WDFunc::NewCB(this, "fwpixtesuffix", Cli->PathSuffixes);
        hlyout->addWidget(cb);
        chb = WDFunc::NewChB(this, "fwpixtesuffixenchb", "Нет");
        connect(chb,SIGNAL(toggled(bool)),this,SLOT(PixTEChBClicked(bool)));
        hlyout->addWidget(chb);
        vlyout->addLayout(hlyout);
        break;
    }
    default:
        break;
    }
    w->setLayout(vlyout);
    return w;
}

// установка типа поля по выбранному типу делегата

void dir_adddialog::DTypeCBIndexChanged(int FD)
{
    QStringListModel *ltypecbmodel = new QStringListModel;
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("ltypecb");
    QStringList tmpStringList;
    if (cb == 0)
    {
        DBGMSG;
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
        tmpStringList << FW_Links.at(FW_MASKED) << FW_Links.at(FW_PLAIN) << FW_Links.at(FW_CRYPT);
        break;
    }
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        tmpStringList << FW_Links.at(FW_LINK) << FW_Links.at(FW_DLINK) << FW_Links.at(FW_ALLINK) << FW_Links.at(FW_RIGHTS) << FW_Links.at(FW_FNUMBER) \
                      << FW_Links.at(FW_SPECIAL) << FW_Links.at(FW_2CD  ) << FW_Links.at(FW_LLINK) << FW_Links.at(FW_FLINK) \
                      << FW_Links.at(FW_ILINK) << FW_Links.at(FW_FLLINK) << FW_Links.at(FW_DATE) << FW_Links.at(FW_DATETIME);
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
    case FD_TEXTEDIT:
    {
        tmpStringList << FW_Links.at(FW_PLAIN) << FW_Links.at(FW_PIXTE);
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
        DBGMSG;
        return;
    }
    s_tqWidget *w = SetWidget(wdgtsidx);
    sw->clear();
    sw->addWidget(w);
    sw->setCurrentWidget(w);
    QString tmps;
    WDFunc::LEData(this, "value."+QString::number(idx), tmps);
    QStringList links = tmps.split("."); // формируем links
    switch (wdgtsidx)
    {
    case FW_NUMBER:
    {
        if (links.size()>3)
            WDFunc::SetSPBData(this, "number", links.at(3).toDouble());
        break;
    }
    case FW_LINK:
    {
        if (links.size()>4)
        {
            WDFunc::SetCBData(this, "tcb."+QString::number(FW_LINK), links.at(3));
            WDFunc::SetCBData(this, "fwlinkcb2", links.at(4));
        }
        break;
    }
    case FW_SPECIAL:
    {
        if (links.size()>4)
        {
            WDFunc::SetCBData(this, "tcb."+QString::number(FW_SPECIAL), links.at(3));
            WDFunc::SetCBData(this, "fwspecialcb2", links.at(4));
        }
        break;
    }
    case FW_ALLINK:
    {
        if (links.size()>4)
        {
            WDFunc::SetCBData(this, "tcb."+QString::number(FW_ALLINK), links.at(3));
            WDFunc::SetCBData(this, "fwallinkcb2", links.at(4));
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
            while (links.size()>=2) // вытаскиваем данные попарно
            {
                WDFunc::SetCBData(this, "tble."+QString::number(i), links.at(0));
                WDFunc::SetCBData(this, "tblefield."+QString::number(i), links.at(1));
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
            WDFunc::SetCBData(this, "tcb."+QString::number(FW_MAXLINK), links.at(3));
            WDFunc::SetCBData(this, "fwmaxlinkcb2", links.at(4));
            WDFunc::SetCBData(this, "fwmaxlinkcb3", links.at(5));
            if (links.size()>6)
                WDFunc::SetLEData(this, "fwmaxlinkle", links.at(6));
        }
        break;
    }
    case FW_MASKED:
    {
        if (links.size()>3)
        {
            QString tmps = links.at(3);
            int begidx = tmps.indexOf("\\\"^");
            if (begidx != -1)
                tmps.remove(begidx, 3);
            int endidx = tmps.indexOf("$\\\"");
            if (endidx != -1)
                tmps.remove(endidx, 3);
            WDFunc::SetLEData(this, "fwmaskedle", tmps);
        }
        break;
    }
    case FW_ID:
    {
        if (links.size()>3)
            WDFunc::SetSPBData(this, "id", links.at(3).toDouble());
        break;
    }
    case FW_EQUAT:
    {
        if (links.size()>4)
        {
            WDFunc::SetLEData(this, "fwequatle", links.at(4));
            if (links.size()>6)
            {
                WDFunc::SetLEData(this, "fwequatle2", links.at(6));
                QString op = links.at(5);
                if (op == "s")  WDFunc::SetCBIndex(this, "fwequatcb", 0);
                if (op == "r")  WDFunc::SetCBIndex(this, "fwequatcb", 1);
                if (op == "m")  WDFunc::SetCBIndex(this, "fwequatcb", 2);
                if (op == "d")  WDFunc::SetCBIndex(this, "fwequatcb", 3);
            }
        }
        break;
    }
    case FW_FNUMBER:
    {
        if (links.size()>3)
        {
            WDFunc::SetSPBData(this, "fnumber", links.at(3).count("n", Qt::CaseSensitive));
            WDFunc::SetSPBData(this, "fnumber2", links.at(3).count("d", Qt::CaseSensitive));
        }
        break;
    }
    case FW_PIXTE:
    {
        if (links.size() > 3)
            WDFunc::SetCBIndex(this, "fwpixteprefix", links.at(3).toInt());
        if (links.size() > 4)
            WDFunc::SetCBIndex(this, "fwpixtesuffix", links.at(4).toInt());
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
    double tmpd;
    int tmpi;
    QString tmps;
    WDFunc::CBIndex(this, "dtypecb", tmpi);
    links.append(QString::number(tmpi));
    links.append(".");
    WDFunc::CBData(this, "ltypecb", tmps);
    links.append(QString::number(FW_Links.indexOf(tmps)));
    links.append(".");
    WDFunc::SPBData(this, "dependsspb", tmpd);
    if (tmpd != -1)
        links.append(QString::number(tmpd));
    links.append(".");
    int CurFW = tmps.split(".").at(0).toInt(); // вытаскиваем номер типа ссылки из её названия
    switch (CurFW)
    {
    case FW_ALLINK:
    {
        WDFunc::CBData(this, "tcb."+QString::number(FW_ALLINK), tmps);
        links.append(tmps);
        links.append(".");
        WDFunc::CBData(this, "fwallinkcb2", tmps);
        links.append(tmps);
        break;
    }
    case FW_DLINK:
    {
        for (int i = 0; i < 4; i++)
        {
            WDFunc::CBData(this, "tble."+QString::number(i), tmps);
            if (tmps.isEmpty())
                break;
            links.append(tmps);
            links.append(".");
            WDFunc::CBData(this, "tblefield."+QString::number(i), tmps);
            links.append(tmps);
            links.append(".");
        }
        links.chop(1); // убрали последнюю точку
        break;
    }
    case FW_EQUAT:
    {
        WDFunc::LEData(this, "fwequatle", tmps);
        links.append(tmps);
        WDFunc::LEData(this, "fwequatle2", tmps);
        if (!tmps.isEmpty())
        {
            links.append(".");
            QString ops[4] = {"s","r","m","d"};
            WDFunc::CBIndex(this, "fwequatcb", tmpi);
            links.append(ops[tmpi]);
            links.append(".");
            links.append(tmps);
        }
        break;
    }
    case FW_ID:
    {
        WDFunc::SPBData(this, "id", tmpd);
        links.append(QString::number(tmpd));
        break;
    }
    case FW_LINK:
    {
        WDFunc::CBData(this, "tcb."+QString::number(FW_LINK), tmps);
        links.append(tmps);
        links.append(".");
        WDFunc::CBData(this, "fwlinkcb2", tmps);
        links.append(tmps);
        break;
    }
    case FW_MASKED:
    {
        WDFunc::LEData(this, "fwmaskedle", tmps);
        links.append("\\\"^"); // в lineedit-е \", для БД кавычки надо ещё больше обрамить
        links.append(tmps);
        links.append("$\\\"");
        break;
    }
    case FW_MAXLINK:
    {
        WDFunc::CBData(this, "tcb."+QString::number(FW_MAXLINK), tmps);
        links.append(tmps);
        links.append(".");
        WDFunc::CBData(this, "fwmaxlinkcb2", tmps);
        links.append(tmps);
        links.append(".");
        WDFunc::CBData(this, "fwmaxlinkcb3", tmps);
        links.append(tmps);
        WDFunc::LEData(this, "fwmaxlinkle", tmps);
        if (!tmps.isEmpty())
        {
            links.append(".");
            links.append(tmps);
        }
        break;
    }
    case FW_NUMBER:
    {
        WDFunc::SPBData(this, "number", tmpd);
        links.append(QString::number(tmpd));
        break;
    }
    case FW_SPECIAL:
    {
        WDFunc::CBData(this, "tcb."+QString::number(FW_SPECIAL), tmps);
        links.append(tmps);
        links.append(".");
        WDFunc::CBData(this, "fwspecialcb2", tmps);
        links.append(tmps);
        break;
    }
    case FW_FNUMBER:
    {
        QString tmps;
        WDFunc::SPBData(this, "fnumber", tmpd);
        tmps.fill('n', tmpd);
        links.append(tmps);
        WDFunc::SPBData(this, "fnumber2", tmpd);
        tmps.fill('d', tmpd);
        links.append(tmps);
        break;
    }
    case FW_PIXTE:
    {
        bool tmpb;
        int tmpi;
        WDFunc::ChBData(this, "fwpixteprefixenchb", tmpb);
        if (!tmpb)
        {
            WDFunc::CBIndex(this, "fwpixteprefix", tmpi);
            links.append(QString::number(tmpi));
        }
        links.append(".");
        WDFunc::ChBData(this, "fwpixtesuffixenchb", tmpb);
        if (!tmpb)
        {
            WDFunc::CBIndex(this, "fwpixtesuffix", tmpi);
            links.append(QString::number(tmpi));
        }
        break;
    }
    default:
        break;
    }
    WDFunc::SetLEData(this, "value."+QString::number(idx), links);
    emit closelinkdialog();
}
