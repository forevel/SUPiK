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
#include "s_accessdialog.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "../models/s_duniversal.h"

dir_adddialog::dir_adddialog(bool update, QString dirtype, QString dir, QWidget *parent) :
    QDialog(parent) // dirtype - имя таблицы из tablefields, где искать информацию о справочнике (dirlist, dirsyslist и т.д.)
{
    idx = 0;
//    setMinimumWidth(300);
    setStyleSheet("QDialog {background-color: rgba(255,255,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    isSomethingChanged = false;
    this->dir = dir;
    this->dirtype = dirtype;
    upd = update;
    setupUI();
}

void dir_adddialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *mainPBLayout = new QHBoxLayout;
    s_tqPushButton *WriteAndClosePB = new s_tqPushButton;
    WriteAndClosePB->setIcon(QIcon(":/res/icon_zap.png"));
    s_tqPushButton *CancelAndClosePB = new s_tqPushButton;
    CancelAndClosePB->setIcon(QIcon(":/res/cross.png"));
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
    s_tqSpinBox *dirFieldNumSB = new s_tqSpinBox;
    dirFieldNumSB->setObjectName("dirFieldNum");
    dirFieldNumSB->setValue(1);
    dirFieldNumSB->setMinimum(1);
    dirFieldNumSB->setMaximum(FSIZE);
    dirFieldNumSB->setDecimals(0);
    s_tqLabel *dirBelongL = new s_tqLabel("Тип справочника");
    s_tqComboBox *dirBelongCB = new s_tqComboBox;
    dirBelongCB->setObjectName("dirBelong");
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
    dirBelongCB->setEnabled(true);
    tmpSLM->setStringList(tmpSL);
    dirBelongCB->setModel(tmpSLM);
    s_tqLabel *dirAccessL = new s_tqLabel("Права доступа");
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    dirAccessLE->setObjectName("dirAccess");
    s_tqPushButton *dirAccessPB = new s_tqPushButton("...");
    if (pc.access & ACC_SYS_WR) // если есть права на изменение системных вещей
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
    updateTWFields(dirFieldNumSB->value());
    connect(dirAccessPB, SIGNAL(clicked()), this, SLOT(setAccessRights()));
    connect(WriteAndClosePB, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    connect(CancelAndClosePB, SIGNAL(clicked()), this, SLOT(CancelAndClose()));
    connect(dirFieldNumSB, SIGNAL(valueChanged(double)), this, SLOT(updateTWFields(double)));
}

void dir_adddialog::CancelAndClose()
{
    this->close();
}

void dir_adddialog::WriteAndClose()
{
    int i;
    s_tqLineEdit *lefield = new s_tqLineEdit;
    s_tqLineEdit *levalue = new s_tqLineEdit;
    s_tqLineEdit *lename = new s_tqLineEdit;
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    s_tqComboBox *dirB = new s_tqComboBox;
    s_tqSpinBox *sb = new s_tqSpinBox;
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
    dirB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirB == 0)
    {
        emit error(ER_DIRADD,0x14);
        return;
    }
    QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "header", "tablename", dirAliasLE->text()+"_полн");
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
        vl << tmpString << dirAliasLE->text()+"_полн" << pc.DateTime << "0" << QString::number(pc.idPers);
        cmpvl << dirAliasLE->text()+"_полн" << lefield->text();
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
    QDialog *dlg = new QDialog;
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked()),this,SLOT(close()));
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
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
    accessdialog->SetupUI(le->text());
    connect(accessdialog, SIGNAL(acceptChanges(QString)), this, SLOT(acceptAccess(QString)));
    accessdialog->exec();
}

// обработка подтверждения в диалоге accessrights

void dir_adddialog::acceptAccess(QString rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
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
    idx = ptr->getAData().toInt(0);
    s_tqSpinBox *sb = new s_tqSpinBox;
    sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        emit error(ER_DIRADD,0x41);
        return;
    }
    s_tqLabel *lbl1 = new s_tqLabel ("Тип поля");
    s_tqLabel *lbl2 = new s_tqLabel ("Тип ссылки");
    s_tqComboBox *cb = new s_tqComboBox;
    QList<QStringList> lsl;
    QStringListModel *cbmodel = new QStringListModel;
    QStringList tmpStringList; // делегаты
    tmpStringList << "Простое поле" << "Поле ввода" << "Поле выбора" << "Выпадающий список" << "Неактивное поле" <<  \
                     "Счётчик" << "Поле выбора с вводом" << "Поле с рамкой";
    cbmodel->setStringList(tmpStringList);
    cb->setModel(cbmodel);
    connect(cb,SIGNAL(currentIndexChanged(int)),this,SLOT(DTypeCBIndexChanged(int)));
    adjustFieldSize(cb, 25);
    s_tqComboBox *ltypecb = new s_tqComboBox;
    ltypecb->setObjectName("ltypecb");
    connect(ltypecb,SIGNAL(currentIndexChanged(int)),this,SLOT(LTypeCBIndexChanged(int)));
    adjustFieldSize(ltypecb, 30);
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x42);
        return;
    }
    s_tqTableView *tbv = new s_tqTableView;
    s_ncmodel *tbvmodel = new s_ncmodel;
    QDialog *dlg = new QDialog(this);
    QVBoxLayout *lyout = new QVBoxLayout;
    lyout->addWidget(lbl1, 0, Qt::AlignLeft);
    lyout->addWidget(cb, 0, Qt::AlignLeft);
    lyout->addWidget(lbl2, 0, Qt::AlignLeft);
    lyout->addWidget(ltypecb, 0, Qt::AlignLeft);

    s_tqLabel *lbl = new s_tqLabel("Число");
    lbl->setObjectName("fw1l");
    lbl->setVisible(false);
    lyout->addWidget(lbl);
    sb = new s_tqSpinBox;
    sb->setObjectName("fw1sb");
    sb->setVisible(false);
    sb->setDecimals(0);
    sb->setSingleStep(1);
    sb->setMaximum(999999);
    sb->setMinimum(0);
    lyout->addWidget(sb);


/*    case FD_CHOOSE:
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
//        break;
/*        }
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
//    default:
//        break;
//    }
    lyout->setSizeConstraint(QLayout::SetFixedSize);
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked()),this,SLOT(ConstructLink()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("Неа");
    connect(pb,SIGNAL(clicked()),dlg,SLOT(close()));
    lyout->addWidget(pb);
    connect(this,SIGNAL(closelinkdialog()),dlg,SLOT(close()));
    dlg->setLayout(lyout);

    PublicClass::fieldformat ff;
    ltype = le->text();
    ff = pc.getFFfromLinks(ltype);
    cb->setCurrentIndex(ff.delegate);
    ltypecb->setCurrentIndex(ff.ftype);
    dlg->exec();
//        updateTWFields(sb->value());
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
        tmpStringList << "0.Автонумерация" << "1.Фиксированное значение" << "5.Значение с макс. ИД" << "7.Вычисляемое поле" << "8.Простое поле" << "19.Автодополнение нулями";
        break;
    }
    case FD_LINEEDIT:
    {
        tmpStringList << "6.Форматированное поле" << "8.Простое поле";
        break;
    }
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        tmpStringList << "2.Простая ссылка" << "3.Ссылка на несколько таблиц" << "4.Ссылка на дочерние элементы" \
                      << "9.Права доступа" << "10.Выбор таблицы" \
                      << "11.Специальная ссылка" << "12.Ссылка на методы" << "13.Вызов диалога редактирования строки" << "14.Конструктор ссылок" << "15.Ссылка на файл" \
                      << "16.Ссылка на каталог" << "17.Ссылка на элемент внутри файла" << "18.Выбор даты";
        break;
    }
    case FD_COMBO:
    {
        tmpStringList << "2.Простая ссылка" << "4.Ссылка на дочерние элементы" \
                      << "10.Выбор таблицы" \
                      << "12.Ссылка на методы";
        break;
    }
    case FD_SPIN:
    {
        tmpStringList << "8.Простое поле";
        break;
    }
    default:
        break;
    }
    ltypecbmodel->setStringList(tmpStringList);
    cb->setModel(ltypecbmodel);
}

// отображение дополнительных элементов диалога при выборе типа ссылки

void dir_adddialog::LTypeCBIndexChanged(int FW)
{
    s_tqLabel *lbl = this->findChild<s_tqLabel *>("fw1l");
    if (lbl == 0)
    {
        emit error(ER_DIRADD,0x51);
        return;
    }
    s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fw1sb");
    if (sb == 0)
    {
        emit error(ER_DIRADD,0x52);
        return;
    }

    lbl->setVisible(false);
    sb->setVisible(false);
    switch (FW)
    {
    case FW_AUTONUM:
    {
        break;
    }
    case FW_NUMBER:
    {
        lbl->setVisible(true);
        sb->setVisible(true);
        break;
    }
    default:
        break;
    }
}

// активация кнопки конструктора при выборе элемента типа "ссылка"

void dir_adddialog::CBPressed(QString str, s_tqComboBox *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqPushButton *pb = new s_tqPushButton;
    pb = this->findChild<s_tqPushButton *>("valueconst"+ptr->getAData().toString()+"PB");
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
    if (pb == 0)
    {
        emit error(ER_DIRADD,0x51);
        return;
    }
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

// сборка строки ссылки из элементов выбора и выход из диалога конструктора

void dir_adddialog::ConstructLink()
{
    emit closelinkdialog();
}

void dir_adddialog::adjustFieldSize(QWidget *wdgt, int widthInChar)
{
    QFontMetrics fm(wdgt->font());
    wdgt->setMinimumWidth(fm.width(QLatin1Char('x'))*widthInChar + 2*2);
}

void dir_adddialog::updateFLE(QString str)
{
    isSomethingChanged = true;
    int i, j;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    s_tqComboBox *cb = new s_tqComboBox;
    cb = this->findChild<s_tqComboBox *>("ltype"+QString::number(idx)+"CB");
    if (cb == 0)
    {
        emit error(ER_DIRADD,0x61);
        return;
    }
    if (le == 0)
    {
        emit error(ER_DIRADD,0x62);
        return;
    }
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

void dir_adddialog::updateALE(long rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
    {
        emit error(ER_DIRADD,0x71);
        return;
    }
    le->setText(QString::number(rights, 16));
}

void dir_adddialog::fillFields()
{
    s_tqLineEdit *lef = new s_tqLineEdit;
    s_tqLineEdit *len = new s_tqLineEdit;
    s_tqLineEdit *lev = new s_tqLineEdit;
    QList<QStringList> lsl;
    QStringList fl;
    fl << "table" << "tablefields" << "header" << "links";
    lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", dir+"_полн", "fieldsorder", true);
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
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
    {
        emit error(ER_DIRADD,0x83);
        return;
    }
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
    {
        emit error(ER_DIRADD,0x84);
        return;
    }
    s_tqComboBox *dirBelongCB = new s_tqComboBox;
    dirBelongCB = this->findChild<s_tqComboBox *>("dirBelong");
    if (dirBelongCB == 0)
    {
        emit error(ER_DIRADD,0x85);
        return;
    }
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
    if (dirAccessLE == 0)
    {
        emit error(ER_DIRADD,0x86);
        return;
    }
    s_tqSpinBox *sb = new s_tqSpinBox;
    sb = this->findChild<s_tqSpinBox *>("dirFieldNum");
    if (sb == 0)
    {
        emit error(ER_DIRADD,0x87);
        return;
    }
    dirAliasLE->setText(dir);
    dirNameLE->setText(lsl.at(0).at(0).split(".").at(1)); // берём имя таблицы по полю ИД
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
