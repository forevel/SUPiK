#include "s_ncdialog.h"

#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QApplication>
#include <QFrame>
#include <QHeaderView>
#include <QMessageBox>

#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqspinbox.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqcheckbox.h"
#include "../widgets/s_maskedle.h"
#include "../gen/s_sql.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqchoosewidget.h"
#include "../gen/publicclass.h"

s_ncdialog::s_ncdialog(QWidget *parent) :
    QDialog(parent)
{
//    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    pf["this_close"] = &s_ncdialog::Close;
    pf["this_accept"] = &s_ncdialog::Accept;
    pf["this_refresh"] = &s_ncdialog::Refresh;
    pf["this_new"] = &s_ncdialog::New;
    pf["this_filter"] = &s_ncdialog::Filter;
    pf["this_search"] = &s_ncdialog::Search;
    pf["dir_add"] = &s_ncdialog::dir_Add;
    pf["dir_edit"] = &s_ncdialog::dir_Edit;
    pf["dir_subdel"] = &s_ncdialog::dir_SubDel;
    pf["dir_subedit"] = &s_ncdialog::dir_SubEdit;
    pf["dir_subadd"] = &s_ncdialog::dir_SubAdd;
    Somethingchanged = false;
}

// dialog - имя диалога (поле dialogs в таблице sup.dialogs)
// wallpaper - имя файла с фоновым изображением
// dialogtype - тип диалога (простой с пятью областями или диалог с двумя таблицами)
// id - номер записи по индексу ключевого поля. Равен нулю, если требуется сделать новую запись

void s_ncdialog::setupUI(QString dialog, QString wallpaper, int dialogtype, QString id)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int i;
    QStringList fl;
    this->id = id;
    fl << "dialogsfields" << "dialogslinks" << "fieldslocation" << "field" << "keyfield" << "default";
    vl = sqlc.getmorevaluesfromtablebyfield(sqlc.getdb("sup"), "dialogs", fl, "dialogs", dialog, "fieldasc");
    if (vl.isEmpty())
    {
        QMessageBox::warning(this, "warning!", "Пустой результат для "+dialog+" из таблицы dialogs");
        return;
    }
    for (i = 0; i < vl.size(); i++)
    {
        if (vl.at(i).at(4) == "v")
        {
            keydb = vl.at(i).at(0).split(".").at(0);
            keytble = vl.at(i).at(0).split(".").at(1);
            break;
        }
    }
    this->wallpaper = wallpaper;
    QHBoxLayout *hlayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *ulayout = new QHBoxLayout;
    QHBoxLayout *u2layout = new QHBoxLayout;
    QGridLayout *mlayout = new QGridLayout;
    QHBoxLayout *tlayout = new QHBoxLayout;
    QHBoxLayout *llayout = new QHBoxLayout;
/*    mainmodel = new s_ncmodel;
    s_tqTableView *mainTV = new s_tqTableView;
    mainTV->setObjectName("mainTV");
    s_duniversal *uniDelegate = new s_duniversal;
    mainTV->setItemDelegate(uniDelegate);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->verticalHeader()->setVisible(false);
    mainTV->horizontalHeader()->setVisible(false); */
    int gridcol = 0;
    int layoutcount = 0;
    for (i = 0; i < vl.size(); i++)
    {
        int tmpInt = vl.at(i).at(1).left(2).toInt();
        switch (vl.at(i).at(2).split(".").at(0).toInt())
        {
        case FL_HEADER:
        {
            // установка заголовка диалога
            s_tqLabel *lbl = new s_tqLabel;
            lbl->setText(vl.at(i).at(0));
            lbl->setObjectName("w"+QString::number(i));
            QFont font;
            font.setPointSize(15);
            lbl->setFont(font);
            hlayout->addWidget(lbl, 100, Qt::AlignRight);
            layoutcount = 1;
            break;
        }
        case FL_UPPER1:
        {
            // установка верхней части
            if (tmpInt == FT_STRETCH)
            {
                ulayout->addStretch(vl.at(i).at(1).mid(3,3).toInt());
                layoutcount = 2;
                break;
            }
            QWidget *uwdgt;
            uwdgt = getWidget(i, vl.at(i).at(0), vl.at(i).at(1), vl.at(i).at(5));
            if (uwdgt == 0) break;
            uwdgt->setObjectName("w"+QString::number(i));
            if ((tmpInt == FT_LABEL) || (tmpInt == FT_PB))
                ulayout->addWidget(uwdgt, 0);
            else
                ulayout->addWidget(uwdgt, 100);
            layoutcount = 2;
            break;
        }
        case FL_UPPER2:
        {
            // установка верхней части
            if (tmpInt == FT_STRETCH)
            {
                u2layout->addStretch(vl.at(i).at(1).mid(3,3).toInt());
                layoutcount = 2;
                break;
            }
            QWidget *uwdgt;
            uwdgt = getWidget(i, vl.at(i).at(0), vl.at(i).at(1), vl.at(i).at(5));
            if (uwdgt == 0) break;
            uwdgt->setObjectName("w"+QString::number(i));
            if ((tmpInt == FT_LABEL) || (tmpInt == FT_PB))
                u2layout->addWidget(uwdgt, 0);
            else
                u2layout->addWidget(uwdgt, 100);
            layoutcount = 2;
            break;
        }
        case FL_MIDDLE:
        {
            // установка средней части
            QWidget *mwdgt = new QWidget;
            mwdgt = getWidget(i, vl.at(i).at(0), vl.at(i).at(1), vl.at(i).at(5));
            if (mwdgt == 0) break;
            mwdgt->setObjectName("w"+QString::number(i));
            mlayout->addWidget(mwdgt, vl.at(i).at(2).split(".").at(1).toInt(), gridcol);
            gridcol++;
            layoutcount = 3;
            break;
        }
        case FL_MAIN:
        {
            // установка главной части. Если не таблица (FT_TABLE), то мимо (по крайней мере пока)
            if (tmpInt == FT_STRETCH)
            {
                tlayout->addStretch(vl.at(i).at(1).mid(3,3).toInt());
                layoutcount = 4;
                break;
            }
            QWidget *uwdgt;
            uwdgt = getWidget(i, vl.at(i).at(0), vl.at(i).at(1), vl.at(i).at(5));
            if (uwdgt == 0) break;
            uwdgt->setObjectName("w"+QString::number(i));
            if ((tmpInt == FT_LABEL) || (tmpInt == FT_PB))
                tlayout->addWidget(uwdgt, 0);
            else
                tlayout->addWidget(uwdgt, 100);
            layoutcount = 4;
            break;
        }
        case FL_LOWER:
        {
            // установка нижней части диалога
            if (tmpInt == FT_STRETCH)
            {
                llayout->addStretch(vl.at(i).at(1).mid(3,3).toInt());
                layoutcount = 5;
                break;
            }
            QWidget *lwdgt = new QWidget;
            lwdgt = getWidget(i, vl.at(i).at(0), vl.at(i).at(1), vl.at(i).at(5));
            if (lwdgt == 0) break;
            lwdgt->setObjectName("w"+QString::number(i));
            int tmpInt = vl.at(i).at(1).left(2).toInt();
            if ((tmpInt == FT_SIMPLE) || (tmpInt == FT_PB))
                llayout->addWidget(lwdgt, 0);
            else
                llayout->addWidget(lwdgt, 20);
            layoutcount = 5;
            break;
        }
        default:
            break;
        }
    }
    switch (dialogtype)
    {
    case DT_GENERAL:
    {
        mainLayout->addLayout(hlayout, 0);
        if (layoutcount>1)
        {
            mainLayout->addLayout(ulayout, 0);
            mainLayout->addLayout(u2layout, 0);
        }
        else
        {
            mainLayout->addStretch(200);
            break;
        }
        if (layoutcount>2)
            mainLayout->addLayout(mlayout, 0);
        else
        {
            mainLayout->addStretch(100);
            break;
        }
        if (layoutcount>3)
            mainLayout->addLayout(tlayout, 100);
        else
        {
            mainLayout->addStretch(100);
            break;
        }
        mainLayout->addLayout(llayout, 0);
        break;
    }
    case DT_2TREES:
    {
        mainLayout->addLayout(hlayout, 0);
        mainLayout->addLayout(ulayout, 0);
        mainLayout->addLayout(mlayout, 100);
        mainLayout->addLayout(llayout, 0);
        break;
    }
    default:
        break;
    }
    setLayout(mainLayout);
    QApplication::restoreOverrideCursor();
}

QWidget *s_ncdialog::getWidget(int id, QString ftext, QString link, QString defstr)
{
    QStringList links = link.split(".");
    QStringList tfields = ftext.split(".");
    switch (links.at(0).toInt())
    {
    case FT_LABEL:
    {
        s_tqLabel *lbl = new s_tqLabel;
        lbl->setText(ftext);
        lbl->setObjectName("lbl"+QString::number(id));
        return lbl;
        break;
    }
    case FT_PB:
    {
        s_tqPushButton *pb = new s_tqPushButton;
        pb->setText(ftext);
        pb->setAData(links.at(2)); // вызываемый метод
        if (links.size()>3)
            pb->setIcon(pc.icons[links.at(3).toInt()]);
        if (links.size()>4)
        {
            QPalette pal(pb->palette());
            pal.setColor(QPalette::ButtonText, pc.colors[links.at(4).toInt()]);
            pb->setPalette(pal);
        }
        connect(pb, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(methodpbclicked(s_tqPushButton*)));
        pb->setObjectName("pb"+QString::number(id));
        return pb;
        break;
    }
    case FT_HLINE:
    {
        QFrame *line = new QFrame;
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setMaximumWidth(2);
        return line;
        break;
    }
    case FT_VLINE:
    {
        QFrame *line = new QFrame;
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setMaximumWidth(2);
        return line;
        break;
    }
    }

    if (this->id == "0")
    {
        if (defstr.isEmpty())
            ftext = "";
        else if (defstr == "$pers")
            ftext = pc.Pers;
        else if (defstr == "$date")
            ftext = pc.DateTime.left(10);
        else if (defstr == "$datetime")
            ftext = pc.DateTime;
        else
            ftext = defstr;
    }
    else // берём реальную ссылку на поле в таблице по ссылке в поле dialogfields, которое представляет собой ссылку на tablefields в формате <table>.<header>
    {
        ftext = sqlc.getvaluefromtablebytablefields(ftext, "id"+keytble, this->id);
        if (sqlc.result)
            return 0;
    }
    switch (links.at(0).toInt())
    {
    case FT_SIMPLE:
    {
        s_tqLabel *wdgt = new s_tqLabel(ftext);
        return wdgt;
        break;
    }
    case FT_LINEEDIT:
    {
        s_tqLineEdit *le = new s_tqLineEdit;
        le->setText(ftext);
        connect (le, SIGNAL(textChanged(QString)), this, SLOT(somethingchanged(QString)));
        return le;
        break;
    }
    case FT_CHOOSE:
    {
        s_tqChooseWidget *cw = new s_tqChooseWidget;
        QString dlink;
        if ((links.at(1).toInt() < 2) || (links.at(1).toInt() > 18))
            return 0;
        dlink = links.at(0)+"."+links.at(1)+"."+links.at(2);
        if (links.size()>3)
        {
            for (int i = 3; i < links.size(); i++)
                dlink += "."+links.at(i);
        }
        cw->Setup(dlink,ftext);
        connect(cw, SIGNAL(datachanged()), this, SLOT(somethingchanged()));
        return cw;
        break;
    }
    case FT_COMBO:
    {
        s_tqComboBox *cb = new s_tqComboBox;
        QStringListModel *tmpModel = new QStringListModel;
        QStringList tmpStringList;
        slmodels.append(tmpModel);
        QSqlDatabase db = sqlc.getdb(links.at(2));
        if (db.isValid())
        {
            switch (links.at(1).toInt())
            {
            case L_LINK:
            {
                if (links.size()<5) return 0;
                QString id = sqlc.getvaluefromtablebyfield(db, links.at(3), "id"+links.at(3), "alias", links.at(4));
                tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, links.at(3), "alias", "idalias", id);
                break;
            }
            case L_ALINK:
            {
                if (links.size()<5) return 0;
                tmpStringList = sqlc.getvaluesfromtablebycolumn(db, links.at(3), links.at(4), "id"+links.at(3), true);
                break;
            }
            case L_SMETHOD:
            {
                tmpStringList = sqlc.getvaluesfromtablebycolumn(sqlc.getdb("sup"), "dialogmethods", "dialogmethods", "iddialogmethods",true);
                break;
            }
            default:
            {
                return 0;
                break;
            }
            }
            slmodels.last()->setStringList(tmpStringList);
            cb->setModel(slmodels.last());
            return cb;
            break;
        }
    }
    case FT_SPIN:
    {
        s_tqSpinBox *sb = new s_tqSpinBox;
        sb->setValue(ftext.toInt());
        connect (sb, SIGNAL(valueChanged(QString)), this, SLOT(somethingchanged(QString)));
        return sb;
        break;
    }
    case FT_MASKED:
    {
        s_MaskedLineEdit *le = new s_MaskedLineEdit(links.at(2));
        le->setText(ftext);
        connect(le, SIGNAL(textChanged(QString)), this, SLOT(somethingchanged(QString)));
        return le;
        break;
    }
    case FT_TABLE:
    {
        s_tqTableView *tv = new s_tqTableView;
        s_duniversal *uniDelegate = new s_duniversal;
        tv->setItemDelegate(uniDelegate);
        tv->setEditTriggers(QAbstractItemView::AllEditTriggers);
        tv->verticalHeader()->setVisible(false);
        tv->horizontalHeader()->setVisible(false);
        s_ncmodel *mdl = new s_ncmodel;
        slavemodels.append(mdl);
        setTable(*(slavemodels.last()), links.at(2)); // в links.at(2) находится имя таблицы из pc.sup.tablefields.table
        tv->setModel(slavemodels.last());
        connect (tv, SIGNAL(activated(QModelIndex)), this, SLOT(somethingchanged(QModelIndex)));
        return tv;
        break;
    }
    case FT_CHECK:
    {
        s_tqCheckBox *cb = new s_tqCheckBox;
        cb->setChecked((ftext.toInt())?true:false);
        connect(cb,SIGNAL(clicked()), this, SLOT(somethingchanged()));
        return cb;
        break;
    }
    case FT_AUTO:
    {
        s_tqLineEdit *le = new s_tqLineEdit;
        le->setEnabled(false);
        if (ftext == "")
            le->setText(QString::number(sqlc.getnextfreeindex(sqlc.getdb(tfields.at(0)), tfields.at(1))));
        else
            le->setText(ftext);
        return le;
        break;
    }
    default:
        break;
    }
    return 0;
}

// проверка взаимозависимых виджетов на заполненность и выключение тех, у кого зависящие виджеты пусты

void s_ncdialog::updateDialog()
{
    int i;
    bool ok;
    for (i = 0; i < vl.size(); i++)
    {
        int relate = vl.at(i).at(1).split(".").at(1).toInt(&ok, 10);
        if (ok)
        {
            QWidget *relatedwidget = this->findChild<QWidget *>("w"+QString::number(i));
            relatedwidget->setVisible(true);
            QWidget *relativewidget = this->findChild<QWidget *>("w"+QString::number(relate));
            switch (vl.at(relate).at(1).split(".").at(0).toInt())
            {
            case FT_CHECK:
            case FT_HLINE:
            case FT_PB:
            case FT_SIMPLE:
            case FT_VLINE:
                break;
            case FT_CHOOSE:
            {
                s_tqTableView *tv = static_cast<s_tqTableView *>(relativewidget);
                if (tv->model()->data(tv->model()->index(0, 0, QModelIndex()), Qt::DisplayRole).toString().isEmpty())
                    relatedwidget->setVisible(false);
                break;
            }
            case FT_COMBO:
            {
                s_tqComboBox *cb = static_cast<s_tqComboBox *>(relativewidget);
                if (cb->currentIndex() == -1)
                    relatedwidget->setVisible(false);
                break;
            }
            case FT_LINEEDIT:
            case FT_MASKED:
            {
                s_tqLineEdit *le = static_cast<s_tqLineEdit *>(relativewidget);
                if (le->text().isEmpty())
                    relatedwidget->setVisible(false);
                break;
            }
            case FT_TABLE:
            {
                if (mainmodel->rowCount() == 0)
                    relatedwidget->setVisible(false);
                break;
            }
            default:
                break;
            }
        }
    }
}

void s_ncdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(wallpaper));
    e->accept();
}

void s_ncdialog::methodpbclicked(s_tqPushButton *ptr)
{
    QString tmpString = ptr->getAData().toString();
    (this->*pf[tmpString])();
}

void s_ncdialog::somethingchanged(QString)
{
    Somethingchanged = true;
}

void s_ncdialog::somethingchanged()
{
    Somethingchanged = true;
}

void s_ncdialog::somethingchanged(QModelIndex index)
{
    Q_UNUSED(index);
    Somethingchanged = true;
}

void s_ncdialog::setTable(s_ncmodel &model, QString link)
{
/*    QList<QStringList> lsl;
    QStringList fl;
    QString db = link.split(".").at(0);
    QString tble = link.split(".").at(1);
    QSqlDatabase qdb = sqlc.getdb(db);
    int i;
    fl << "tablefields" << "header" << "links";
    lsl = sqlc.getmorevaluesfromtablebyfield(sqlc.getdb("sup"), "tablefields", fl, "table", db+"."+tble, "fieldsorder", true);
    if (sqlc.result)
    {
        QMessageBox::warning(this, "warning!", "Проблемы с получением значений по таблице "+db+"."+tble+" из таблицы tablefields");
        return;
    }
    QStringList links, headers, vl;
    fl.clear();
    links.clear();
    headers.clear();
    for (i = 0; i < lsl.size(); i++)
    {
        fl << lsl.at(i).at(0);
        links << lsl.at(i).at(2);
        headers << lsl.at(i).at(1);
    }
    lsl.clear();
    if (id != "0") // если требуется изменить элемент
    {
        lsl = sqlc.getmorevaluesfromtablebyfield(qdb, tble, fl, "id"+keytble, id);
        if (lsl.isEmpty())
        {
            QMessageBox::warning(this, "warning!", "Проблемы с получением значений из таблицы "+db+"."+tble+" по полю №"+id);
            return; // проблемы с получением данных по нужному id
        }
    }
    else
    {
        for (i = 0; i < fl.size(); i++)
            vl << "";
        lsl << vl;
    }

    lsl.insert(0, headers);
    model.fillModel(lsl);
    for (i = 0; i < model.rowCount(); i++)
        model.setrowlinks(i, links); // простые поля для первого столбца */
}

// собственные методы

void s_ncdialog::Accept()
{

}

void s_ncdialog::Close()
{
    this->close();
}

void s_ncdialog::Filter()
{

}

void s_ncdialog::New()
{

}

void s_ncdialog::Refresh()
{

}

void s_ncdialog::Search()
{

}

// Методы справочников

void s_ncdialog::dir_Add()
{
/*    dir_adddialog *AddDialog = new dir_adddialog(false); // no update
    AddDialog->exec(); */
}

void s_ncdialog::dir_Edit()
{
/*    QString tmpString = getMainIndex(1);
    QSqlDatabase db = sqlc.getdb("sup");
    QString dirlink = sqlc.getvaluefromtablebyfield(db, "dirlist", "pc", "dirlist", tmpString);
    dir_adddialog *EditDialog = new dir_adddialog(true, dirlink);
    EditDialog->exec(); */
}

void s_ncdialog::dir_SubAdd()
{

}

void s_ncdialog::dir_SubDel()
{

}

void s_ncdialog::dir_SubEdit()
{

}
