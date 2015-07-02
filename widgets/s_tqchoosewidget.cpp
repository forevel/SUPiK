#include "s_tqchoosewidget.h"
#include <QHBoxLayout>
#include "s_tqlineedit.h"
#include "s_tqpushbutton.h"
#include "../gen/s_sql.h"
#include "../dialogs/s_ncdialog.h"
#include "../dialogs/s_2cdialog.h"
#include "s_tqcalendarwidget.h"

s_tqChooseWidget::s_tqChooseWidget(QWidget *parent) :
    QWidget(parent)
{
    ff.ftype = -1; // проверка на то, вызывали ли перед работой с виджетом функцию getlinks
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout *ml = new QHBoxLayout;
    s_tqLineEdit *le = new s_tqLineEdit;
    s_tqPushButton *pb = new s_tqPushButton("...");
    le->setEnabled(false);
    le->setObjectName("cwle");
    pb->setObjectName("cwpb");
    ml->addWidget(le, 80);
    ml->addWidget(pb,0);
    connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
    ml->setContentsMargins(0, 0, 0, 0);
    setLayout(ml);
}

void s_tqChooseWidget::pbclicked()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("cwle");
    QString lestr = le->text();
    if (ff.ftype == -1)
    {
        le->setText("Ошибка! Отсутствует вызов заполнения links! Обратитесь к разработчику");
        return;
    }
    switch (ff.delegate)
    {
    case L_LINK:
    {
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
        {
            QStringList tmpStringList = sqlc.getvaluesfromtablebycolumn(db, ff.link.at(1), ff.link.at(2));
            s_2cdialog *chooseDialog = new s_2cdialog("");
            chooseDialog->setup(tmpStringList, QStringList(), lestr);
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            if (sqlc.result)
            {
                le->setText("Ошибка! Не найдены данные для списка выбора! Обратитесь к разработчику");
                return;
            }
            chooseDialog->exec();
        }
        else
        {
            le->setText("Ошибка! Некорректная ссылка на БД ("+ff.link.at(0)+")!");
            return;
        }
        break;
    }
    case L_ALINK:
    {
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
        {
            QString id = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), "id"+ff.link.at(1), "alias", ff.link.at(2));
            if (sqlc.result)
            {
                le->setText("Ошибка! Не найдены данные для списка выбора! Обратитесь к разработчику");
                return;
            }
            QStringList tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, ff.link.at(1), "alias", "idalias", id);
            if (sqlc.result)
            {
                le->setText("Ошибка! Не найдены данные для списка выбора! Обратитесь к разработчику");
                return;
            }
            s_2cdialog *chooseDialog = new s_2cdialog("");
            chooseDialog->setup(tmpStringList);
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            chooseDialog->exec();
        }
        else
        {
            le->setText("Ошибка! Некорректная ссылка на БД ("+ff.link.at(0)+")!");
            return;
        }
        break;
    }
    case L_DLINK:
    {
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
        {
            QStringList tmpStringList = sqlc.getvaluesfromtablebycolumn(db, ff.link.at(1), ff.link.at(2));
            if (sqlc.result)
            {
                le->setText("Ошибка! Не найдены данные для списка выбора! Обратитесь к разработчику");
                return;
            }
            db = sqlc.getdb(ff.link.at(3)); // вторая таблица
            if (db.isValid())
                tmpStringList.append(sqlc.getvaluesfromtablebycolumn(db, ff.link.at(4), ff.link.at(5)));
            s_2cdialog *chooseDialog = new s_2cdialog("");
            chooseDialog->setup(tmpStringList,QStringList(),lestr);
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            chooseDialog->exec();
        }
        else
        {
            le->setText("Ошибка! Некорректная ссылка на БД ("+ff.link.at(0)+")!");
            return;
        }
        break;
    }
    case L_RIGHTS:
    {
        // вызов диалога редактирования прав доступа
        break;
    }
    case L_DATE:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("cwle");
        QDate dte;
        dte = QDate::fromString(le->text(), "dd/MM/yyyy");
        s_tqCalendarWidget *calWdgt = new s_tqCalendarWidget;
        QPoint pt;
        pt = le->cursor().pos();
        calWdgt->move(pt);
        calWdgt->setSelectedDate(dte);
        connect(calWdgt, SIGNAL(activated(QDate)), this, SLOT(dateChoosed(QDate)));
        connect(calWdgt, SIGNAL(activated(QDate)), calWdgt, SLOT(close()));
        calWdgt->show();
        break;
    }
    default:
        break;
    }
}

void s_tqChooseWidget::accepted(QString str)
{
    QString tmpString = str;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit*>("cwle");
    switch (ff.delegate)
    {
    case L_ALINK:
    {
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
            tmpString = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), ff.link.at(1), "id"+ff.link.at(1), str);
        break;
    }
    case L_DLINK:
    {
        QSqlDatabase db = sqlc.getdb(ff.link.at(2));
        if (db.isValid())
        {
            tmpString = sqlc.getvaluefromtablebyfield(db, ff.link.at(3), ff.link.at(3), "id"+ff.link.at(3), str);
            if (sqlc.result)
            {
                QSqlDatabase db = sqlc.getdb(ff.link.at(0));
                if (db.isValid())
                    tmpString = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), ff.link.at(1), "id"+ff.link.at(1), str);
            }
        }
        break;
    }
    case L_RIGHTS:
    {
        break;
    }
    case L_DATE:
        break;
    default:
        break;
    }
    le->setText(tmpString);
    emit datachanged();
}

void s_tqChooseWidget::setlinks(QString links)
{
    ff = getFFfromLinks(links);
}

void s_tqChooseWidget::setdata(QString data)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("cwle");
    le->setText(data);
}

s_tqChooseWidget::fieldformat s_tqChooseWidget::getFFfromLinks(QString links)
{
    QStringList tmpsl = links.split(".");
    fieldformat ff;
    ff.ftype = 8;
    ff.delegate = 4;
    ff.dependson = -1;
    ff.link.clear();
    if (!tmpsl.size())
        return ff;
    ff.ftype = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    bool ok;
    ff.dependson = tmpsl.at(0).toInt(&ok, 10);
    if (!ok)
        ff.dependson = -1;
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    for (int i = 0; i < tmpsl.size(); i++)
        ff.link << tmpsl.at(i);
    return ff;
}

void s_tqChooseWidget::dateChoosed(QDate dte)
{
    accepted(dte.toString("dd/MM/yyyy"));
}

void s_tqChooseWidget::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqChooseWidget::getAData()
{
    return this->adata;
}
