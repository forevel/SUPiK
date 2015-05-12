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
#include "../models/s_duniversal.h"
#include "../gen/publicclass.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqcheckbox.h"
#include "../widgets/s_tqspinbox.h"
#include "s_accessdialog.h"
#include "../gen/s_sql.h"
#include "s_tablefilterdialog.h"
#include "s_acceptdialog.h"
#include "../widgets/s_tqtreeview.h"
#include "../models/s_ntmodel.h"
#include "../widgets/s_tqtableview.h"
#include "../models/s_ncmodel.h"

dir_adddialog::dir_adddialog(bool update, QString dir, QWidget *parent) :
    QDialog(parent)
{
    result = 0;
    idx = 0;
    setMinimumWidth(300);
    setStyleSheet("QDialog {background-color: rgba(255,255,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    isSomethingChanged = false;
    this->dir = dir;
    upd = update;
    setupUI();
}

void dir_adddialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *mainPBLayout = new QHBoxLayout;
    s_tqPushButton *WriteAndClosePB = new s_tqPushButton;
    WriteAndClosePB->setIcon(QIcon(":/icon_zap.png"));
    s_tqPushButton *CancelAndClosePB = new s_tqPushButton;
    CancelAndClosePB->setIcon(QIcon(":/cross.png"));
    QTabWidget *mainTW = new QTabWidget;
    QDialog *dlg1 = new QDialog;
    QDialog *dlg2 = new QDialog;
    dlg2->setObjectName("dlg2");
    QDialog *dlg3 = new QDialog;
    dlg3->setObjectName("dlg3");
    QGridLayout *dlg1Layout = new QGridLayout;
    QGridLayout *dlg2Layout = new QGridLayout;
    QGridLayout *dlg3Layout = new QGridLayout;
    s_tqLabel *dirAliasL = new s_tqLabel("Имя справочника");
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirAliasLE->setObjectName("dirAlias");
    s_tqLabel *dirNameL = new s_tqLabel("Имя таблицы справочника (необязательно)");
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    dirNameLE->setObjectName("dirName");
    s_tqLabel *dirFieldNumL = new s_tqLabel("Количество полей");
    s_tqspinbox *dirFieldNumSB = new s_tqspinbox;
    dirFieldNumSB->setObjectName("dirFieldNum");
    dirFieldNumSB->setValue(1);
    dirFieldNumSB->setMinimum(1);
    dirFieldNumSB->setMaximum(FSIZE);
    s_tqLabel *dirBelongL = new s_tqLabel("Тип справочника");
    s_tqComboBox *dirBelongCB = new s_tqComboBox;
    dirBelongCB->setObjectName("dirBelong");
    QStringListModel *tmpSLM = new QStringListModel;
    QStringList tmpSL;
    tmpSL << "Основной";
    if (pc.access & (ALT_WR | SYS_WR)) // САПРовские права
        tmpSL << "Altium" << "Schemagee" << "Solidworks" << "Устройства" << "Конструктивы";
    if (pc.access & (TB_WR | SYS_WR)) // ГИ права
        tmpSL << "ОТ и ТБ";
    if (pc.access & (SADM_WR | SYS_WR)) // Сисадминские права
        tmpSL << "СисАдмин";
    if (pc.access & SYS_WR)
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
    dirBelongCB->setEnabled(true);
    tmpSLM->setStringList(tmpSL);
    dirBelongCB->setModel(tmpSLM);
    s_tqLabel *dirAccessL = new s_tqLabel("Права доступа");
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    dirAccessLE->setObjectName("dirAccess");
    s_tqPushButton *dirAccessPB = new s_tqPushButton("...");
    if (pc.access & SYS_WR) // если есть права на изменение системных вещей
        dirAccessPB->setEnabled(true);
    else
        dirAccessPB->setEnabled(false);
    s_tqLabel *mainL = new s_tqLabel("Редактор справочников");
    QFont font;
    font.setPointSize(15);
    mainL->setFont(font);

    mainPBLayout->addWidget(WriteAndClosePB);
    mainPBLayout->addWidget(CancelAndClosePB);
    mainPBLayout->addStretch(300);
    mainLayout->addWidget(mainL, 0, Qt::AlignLeft);
    mainLayout->addLayout(mainPBLayout);
    dlg1Layout->addWidget(dirAliasL, 0, 0);
    dlg1Layout->addWidget(dirAliasLE, 0, 1, 1, 2);
    connect(dirAliasLE, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    dlg1Layout->addWidget(dirFieldNumL, 1, 0);
    dlg1Layout->addWidget(dirFieldNumSB, 1, 1, 1, 2);
    dlg1Layout->addWidget(dirBelongL, 2, 0);
    dlg1Layout->addWidget(dirBelongCB, 2, 1, 1, 2);
    dlg1Layout->addWidget(dirAccessL, 3, 0);
    dlg1Layout->addWidget(dirAccessLE, 3, 1);
    dlg1Layout->addWidget(dirAccessPB, 3, 2);
    dlg1Layout->addWidget(dirNameL, 4, 0);
    dlg1Layout->addWidget(dirNameLE, 4, 1, 1, 2);
    dlg1Layout->setColumnStretch(0, 10);
    dlg1Layout->setColumnStretch(1, 90);
    dlg1Layout->setColumnStretch(2, 0);
    QWidget *wdgt = new QWidget;
    QVBoxLayout *wdgtl = new QVBoxLayout;
    wdgtl->addStretch(100);
    dlg1Layout->addWidget(wdgt, 5, 0, 1, 3);
    dlg1->setLayout(dlg1Layout);
    mainTW->addTab(dlg1, "Основные");
    for (int i = 0; i < FSIZE; i++)
    {
        s_tqLabel *hdrL = new s_tqLabel("Поле №" + QString::number(i+1) + " :");
        hdrL->setObjectName("hdr"+QString::number(i)+"L");
        s_tqLineEdit *nameLE = new s_tqLineEdit;
        nameLE->setObjectName("name" + QString::number(i) + "LE");
        nameLE->setAData(i);
        adjustFieldSize(nameLE, 20);
        s_tqLabel *bodyL1 = new s_tqLabel(",сист. имя:");
        bodyL1->setObjectName("body"+QString::number(i)+"L1");
        s_tqLineEdit *fieldLE = new s_tqLineEdit;
        fieldLE->setObjectName("field" + QString::number(i) + "LE");
        adjustFieldSize(fieldLE, 15);
        s_tqLabel *bodyL2 = new s_tqLabel(", содержит:");
        bodyL2->setObjectName("body"+QString::number(i)+"L2");
        s_tqLineEdit *valueLE = new s_tqLineEdit;
        valueLE->setObjectName("value" + QString::number(i) + "LE");
        adjustFieldSize(valueLE, 10);
        valueLE->setEnabled(false);
        s_tqPushButton *valueconstPB = new s_tqPushButton("Конструктор");
        valueconstPB->setAData(i);
        valueconstPB->setObjectName("valueconst" + QString::number(i) + "PB");
        adjustFieldSize(valueconstPB, 4);
        connect(valueconstPB, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(FPBPressed(s_tqPushButton*)));
        connect(nameLE,SIGNAL(textChanged(QString,s_tqLineEdit*)),this,SLOT(transliteFieldName(QString,s_tqLineEdit*)));
        QList<QWidget *> wl;
        wl << hdrL << nameLE << bodyL1 << fieldLE << bodyL2 << valueLE << valueconstPB;
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
    mainLayout->addWidget(mainTW, 95);
    setLayout(mainLayout);
    setMinimumHeight(height()); // меньше не даём изменять, ибо некрасиво
    setMinimumWidth(width()+50);
    if (upd)
        fillFields();
    int res = updateTWFields(dirFieldNumSB->value());
    if (res)
    {
        result = 1;
        return;
    }
    connect(dirAccessPB, SIGNAL(clicked()), this, SLOT(setAccessRights()));
    connect(WriteAndClosePB, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    connect(CancelAndClosePB, SIGNAL(clicked()), this, SLOT(CancelAndClose()));
    connect(dirFieldNumSB, SIGNAL(valueChanged(int)), this, SLOT(updateTWFields(int)));
}

void dir_adddialog::CancelAndClose()
{
    this->close();
}

void dir_adddialog::WriteAndClose()
{
    try
    {
        int i;
        s_tqLineEdit *lefield = new s_tqLineEdit;
        s_tqLineEdit *levalue = new s_tqLineEdit;
        s_tqLineEdit *lename = new s_tqLineEdit;
        s_tqLineEdit *dirNameLE = new s_tqLineEdit;
        s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
        s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
        s_tqComboBox *dirB = new s_tqComboBox;
        s_tqspinbox *sb = new s_tqspinbox;
        dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
        if (dirNameLE == 0)
            throw(0x11);
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
            throw(0x12);
        dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
        if (dirAccessLE == 0)
            throw(0x13);
        dirB = this->findChild<s_tqComboBox *>("dirBelong");
        if (dirB == 0)
            throw(0x14);
        QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", dirAliasLE->text()+"_полная");
        if (sqlc.result == 2) // ошибка открытия таблицы
        {
            QMessageBox::warning(this,"warning!","Невозможно открыть БД tablefields!");
            return;
        }
        else if (!sqlc.result) // есть запись про таблицу
        {
            if (QMessageBox::question(this, "Данные справочника существуют", "Перезаписать?", QMessageBox::Yes|QMessageBox::No,\
                                  QMessageBox::No) == QMessageBox::No)
                return;
        }
        sb = this->findChild<s_tqspinbox *>("dirFieldNum");
        if (sb == 0)
            throw(0x15);
        QSqlDatabase db = sqlc.getdb("sup");
        QStringList fl, vl;
        QStringList cmpfl, cmpvl;
        cmpfl << "tablename" << "tablefields";
        QString tble = dirBelongAliases.value(dirB->currentText());
        fl << "tablefields" << "table" << "keyfield" << "headers" << "links" << "fieldsorder" << "tablename" << \
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
            vl << tmpString << dirAliasLE->text()+"_полная" << pc.DateTime << "0" << QString::number(pc.idPers);
            cmpvl << dirAliasLE->text()+"_полная" << lefield->text();
            QString id = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "idtablefields", cmpfl, cmpvl);
            if (sqlc.result == 1) // нет такой записи
                sqlc.insertvaluestotable(sqlc.getdb("sup"), "tablefields", fl, vl);
            else
                sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
            if (sqlc.result)
                throw(0x17);
        }
        tble = "dirlist";
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
                QMessageBox::warning(this, "warning!", "Ошибка обновления каталога справочников");
                return;
            }
        }
        else
        {
            tmpString = sqlc.updatevaluesintable(db, tble, fl, vl, "dirlist", dirAliasLE->text());
            if (sqlc.result)
            {
                QMessageBox::warning(this, "warning!", "Ошибка обновления каталога справочников");
                return;
            }
        }
        s_acceptdialog *dlg = new s_acceptdialog;
        dlg->setupUI("Ура!","Записано успешно!");
        connect(dlg,SIGNAL(destroyed()), this, SLOT(close()));
        dlg->exec();
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

int dir_adddialog::updateTWFields(int fn)
{
    try
    {
        s_tqLabel *ll = new s_tqLabel;
        s_tqLineEdit *le = new s_tqLineEdit;
        s_tqPushButton *pb = new s_tqPushButton;
        int i = 0;
        while (i < fn)
        {
            ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
            if (ll == 0)
                throw (0x21);
            ll->setVisible(true);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
            if (ll == 0)
                throw(0x22);
            ll->setVisible(true);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
            if (ll == 0)
                throw(0x23);
            ll->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x24);
            le->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x25);
            le->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x26);
            le->setVisible(true);
            pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
            if (pb == 0)
                throw(0x28);
            pb->setVisible(true);
            i++;
        }
        while (i < FSIZE)
        {
            ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
            if (ll == 0)
                throw(0x29);
            ll->setVisible(false);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
            if (ll == 0)
                throw(0x2a);
            ll->setVisible(false);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
            if (ll == 0)
                throw(0x2b);
            ll->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2c);
            le->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2d);
            le->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2e);
            le->setVisible(false);
            pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
            if (pb == 0)
                throw(0x2f);
            pb->setVisible(false);
            i++;
        }
        return 0;
    }
    catch (int res)
    {
        WarningAndClose(res);
        return res;
    }
}

void dir_adddialog::setAccessRights()
{
    s_accessdialog *accessdialog = new s_accessdialog;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
    accessdialog->SetupUI(le->text().toLongLong(0, 16));
    connect(accessdialog, SIGNAL(acceptChanges(long)), this, SLOT(acceptAccess(long)));
    accessdialog->exec();
}

// обработка подтверждения в диалоге accessrights

void dir_adddialog::acceptAccess(long rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
    le->setText(QString::number(rights, 16));
}

void dir_adddialog::addLineToDlg(QList<QWidget *> wl, QGridLayout &lyt, int row)
{
    for (int i = 0; i < wl.size(); i++)
        lyt.addWidget(wl.at(i), row, i);
}

// Вызов конструктора ссылок

void dir_adddialog::FPBPressed(s_tqPushButton *ptr)
{
    try
    {
        //int i;
        idx = ptr->getAData().toInt(0);
        s_tqspinbox *sb = new s_tqspinbox;
        sb = this->findChild<s_tqspinbox *>("dirFieldNum");
        if (sb == 0)
            throw(0x41);
        s_tqLabel *lbl1 = new s_tqLabel ("Тип поля");
        s_tqLabel *lbl2 = new s_tqLabel ("Тип ссылки");
        s_tqComboBox *cb = new s_tqComboBox;
        QList<QStringList> lsl;
        QStringListModel *cbmodel = new QStringListModel;
        QStringList tmpStringList;
        tmpStringList << "Предопределённое поле" << "Простое поле" << "Ссылка" << "Права доступа";
        cbmodel->setStringList(tmpStringList);
        cb->setModel(cbmodel);
        adjustFieldSize(cb, 25);
        s_tqComboBox *ltypecb = new s_tqComboBox;
        QStringListModel *ltypecbmodel = new QStringListModel;
        tmpStringList.clear();
        tmpStringList << "Ссылка на поле" << "Ссылка на справочник" << "Ссылка на таблицу" << "Ссылка на БД" << "Косвенная ссылка";
        ltypecbmodel->setStringList(tmpStringList);
        ltypecb->setModel(ltypecbmodel);
        adjustFieldSize(ltypecb, 30);
        s_tqLineEdit *le = new s_tqLineEdit;
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
        if (le == 0)
            throw(0x42);
//        s_tqtreeview *trv = new s_tqtreeview;
        s_tqTableView *tbv = new s_tqTableView(true);
        s_ntmodel *trvmodel = new s_ntmodel;
        s_ncmodel *tbvmodel = new s_ncmodel;
        QDialog *dlg = new QDialog;
        QVBoxLayout *lyout = new QVBoxLayout;
        lyout->addWidget(lbl1, 0, Qt::AlignLeft);
        lyout->addWidget(cb, 0, Qt::AlignLeft);
        lyout->addWidget(lbl2, 0, Qt::AlignLeft);
        lyout->addWidget(ltypecb, 0, Qt::AlignLeft);
        PublicClass::fieldformat ff;
        ff = pc.getFFfromLinks(le->text());
        switch (ff.delegate)
        {
        case FD_LINEEDIT:
        {
            cb->setCurrentIndex(1);
            ltypecb->setVisible(false);
            break;
        }
        case FD_DISABLED:
        {
            cb->setCurrentIndex(0);
            ltypecb->setVisible(false);
            break;
        }
        case FD_CHOOSE:
        {
            cb->setCurrentIndex(2);
            switch (ff.ftype)
            {
            case FW_LINK:
            {
                if (trvmodel->Setup(false, ff.link.at(0)) == 11) // если не дерево
                {
                    tmpStringList.clear();

// КАЖЕТСЯ, ЗДЕСЬ ЕСТЬ ОШИБКА
                    lsl = sqlc.searchintablefieldlike(sqlc.getdb("sup"), "tablefields", QStringList("tablename"), "tablename", "^.*_сокращ$");
                    QStringList sl;
                    sl = lsl.at(0);
                    while (sl.size() > 0)
                    {
                        tmpStringList << sl.at(0);
                        sl.removeAll(tmpStringList.last());
                    }
                    lsl.clear();
                    lsl.append(tmpStringList);
                    tbvmodel->fillModel(lsl);
                    s_duniversal *uniDelegate = new s_duniversal;
                    tbv->setEditTriggers(QAbstractItemView::AllEditTriggers);
                    tbv->verticalHeader()->setVisible(false);
                    tbv->horizontalHeader()->setVisible(false);
                    tbv->setItemDelegate(uniDelegate);
                    tbvmodel->setcolumnlinks(0, "7.8");
                    tbv->setModel(tbvmodel);
                    tbv->resizeColumnsToContents();
                    tbv->resizeRowsToContents();
                    lyout->addWidget(tbv);
                }
            }
/*            sqlc.prepareslsfortree(sl1, sl2);
            if (sl1.isEmpty())
                throw(0x44);
            s_tablefilterdialog *tabledialog = new s_tablefilterdialog;
            QString tmpString = le->text().split(".").last();
            tabledialog->SetupUI(sl1, sl2, tmpString);
            connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(updateFLE(QString)));
            tabledialog->exec(); */
            break;
            }
        }
/*        case 2: // права доступа
        {
            s_accessdialog *accessdialog = new s_accessdialog;
            accessdialog->SetupUI(le->text().toLongLong(0, 16));
            connect(accessdialog, SIGNAL(acceptChanges(long)), this, SLOT(updateALE(long)));
            accessdialog->exec();
            break;
        }
        case 3: // косвенная ссылка
        {
            QSqlDatabase db = sqlc.getdb(dir.left(3));
            if (!db.isValid())
                throw(0x45);
            QString tble = dir.right(dir.size()-4);
            QStringList fl = sqlc.getcolumnsfromtable(db, tble);
            s_tablefilterdialog *tabledialog = new s_tablefilterdialog;
            QString tmpString = le->text().split(".").last();
            tabledialog->SetupUI(fl, tmpString);
            connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(updateFLE(QString)));
            tabledialog->exec();
            break;
        } */
        default:
            break;
        }
        dlg->setLayout(lyout);
        dlg->exec();
//        updateTWFields(sb->value());
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

void dir_adddialog::CBPressed(QString str, s_tqComboBox *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqPushButton *pb = new s_tqPushButton;
    pb = this->findChild<s_tqPushButton *>("valueconst"+ptr->getAData().toString()+"PB");
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
    if (pb == 0)
        WarningAndClose(51);
    if (ptr->currentIndex() == 0)
    {
        le->setEnabled(true);
        pb->setEnabled(false);
    }
    else
    {
        le->setEnabled(true);
        pb->setEnabled(true);
    }
}

void dir_adddialog::adjustFieldSize(QWidget *wdgt, int widthInChar)
{
    QFontMetrics fm(wdgt->font());
    wdgt->setMinimumWidth(fm.width(QLatin1Char('x'))*widthInChar + 2*2);
}

void dir_adddialog::updateFLE(QString str)
{
    try
    {
        isSomethingChanged = true;
        int i, j;
        s_tqLineEdit *le = new s_tqLineEdit;
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
        s_tqComboBox *cb = new s_tqComboBox;
        cb = this->findChild<s_tqComboBox *>("ltype"+QString::number(idx)+"CB");
        if (cb == 0)
            throw(0x61);
        if (le == 0)
            throw(0x62);
        switch (cb->currentIndex())
        {
        case 1:
        {
            i = sl2.indexOf(str);
            if (i != -1)
            {
                j = 0;
                while (i > 0)
                {
                    if (sl2.at(i--) == "")
                        j++;
                }
                le->setText("pc."+sl1.at(j).left(3)+"."+str); // компонуем имя таблицы в формат <имя_БД>.<имя_таблицы>
            }
            break;
        }
        case 3:
        {
            le->setText("."+str);
        }
        default:
            break;
        }
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

void dir_adddialog::updateALE(long rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
        WarningAndClose(71);
    le->setText(QString::number(rights, 16));
}

void dir_adddialog::fillFields()
{
    try
    {
        s_tqLineEdit *lef = new s_tqLineEdit;
        s_tqLineEdit *len = new s_tqLineEdit;
        s_tqLineEdit *lev = new s_tqLineEdit;
        QList<QStringList> lsl;
        QStringList fl;
        fl << "table" << "tablefields" << "headers" << "links";
        lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", dir+"_полная", "fieldsorder", true);
        if (sqlc.result)
            throw(0x71);
        if (lsl.size() == 0)
            throw(0x72);
        s_tqLineEdit *dirNameLE = new s_tqLineEdit;
        dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
        if (dirNameLE == 0)
            throw(0x73);
        s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
            throw(0x74);
        s_tqComboBox *dirBelongCB = new s_tqComboBox;
        dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
        if (dirBelongCB == 0)
            throw(0x75);
        s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
        dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
        if (dirAccessLE == 0)
            throw(0x76);
        s_tqspinbox *sb = new s_tqspinbox;
        sb = this->findChild<s_tqspinbox *>("dirFieldNum");
        if (sb == 0)
            throw(0x77);
        dirAliasLE->setText(dir);
        dirNameLE->setText(lsl.at(0).at(0).split(".").at(1));
        QString tmpString = dirBelongAliases.key(lsl.at(0).at(0).split(".").at(0));
        if (tmpString.isEmpty())
            throw(0x78);
        dirBelongCB->setCurrentText(tmpString);
        tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"),"dirlist","access","dirlist",dir);
        if (tmpString.isEmpty())
            throw(0x79);
        dirAccessLE->setText(tmpString);
        sb->setValue(lsl.size());
        for (int i = 0; i < lsl.size(); i++)
        {
            if (i > 16)
                break;
            lef = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE"); // имя поля
            if (lef == 0)
                throw(0x7b);
            lev = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE"); // строка 1
            if (lev == 0)
                throw(0x7c);
            len = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE"); // строка 2
            if (len == 0)
                throw(0x7d);
            lef->setText(lsl.at(i).at(1));
            len->setText(lsl.at(i).at(2));
            lev->setText(lsl.at(i).at(3));
        }
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

void dir_adddialog::WarningAndClose(int res)
{
    int ernum = module_number + res;
    s_acceptdialog *dlg = new s_acceptdialog;
    dlg->setupUI("warning!","Ошибка №"+QString::number(ernum, 16));
    connect(dlg,SIGNAL(destroyed()), this, SLOT(close()));
    dlg->exec();
    this->close();
}

void dir_adddialog::transliteFieldName(QString str, s_tqLineEdit *ptr)
{
    try
    {
        isSomethingChanged = true;
        Q_UNUSED(str);
        s_tqLineEdit *le = new s_tqLineEdit;
        le = this->findChild<s_tqLineEdit *>("name"+ptr->getAData().toString()+"LE");
        s_tqLineEdit *le2 = new s_tqLineEdit;
        le2 = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
        if (le == 0)
            throw(0xa1);
        if (le2 == 0)
            throw(0xa2);
        if (le2->text().isEmpty())
            le2->setText(pc.getTranslit(le->text()));
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

void dir_adddialog::transliteDirName()
{
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
        WarningAndClose(91);
    if (dirNameLE->text().isEmpty())
    {
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
            WarningAndClose(92);
        QString tmpString = pc.getTranslit(dirAliasLE->text());
        dirNameLE->setText(tmpString);
    }
}
