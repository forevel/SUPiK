#include "../inc/s_duniversal.h"
#include "../inc/s_tqlineedit.h"
#include "../inc/s_tqspinbox.h"
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_tqcalendarwidget.h"
#include "../inc/s_sql.h"
#include "../inc/s_ncmodel.h"
#include "../inc/s_2cdialog.h"
#include "../inc/s_2ctdialog.h"
#include "../inc/s_maskedle.h"
#include <QComboBox>
#include <QStringListModel>
#include <QPainter>
#include <QHBoxLayout>
#include <QRect>
#include <QLabel>
#include <QSpinBox>
#include <QPoint>

s_duniversal::s_duniversal(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_duniversal::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QStringList tmpStringList;
    ff = getFFfromLinks(index.data(Qt::UserRole).toString());
    hdr=index.data(Qt::UserRole+1).toString(); // в UserRole+1 должна содержаться aData, в которой находится подзаголовок диалога редактирования, вызываемого по кнопке в делегате
    QWidget *editor = 0;
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        combWidget = new s_tqwidget(parent);
        combWidget->setStyleSheet("QWidget {background: khaki};");
        QHBoxLayout *ml = new QHBoxLayout;
        s_tqLineEdit *le = new s_tqLineEdit(combWidget);
        s_tqPushButton *pb = new s_tqPushButton(combWidget);
        le->setObjectName("fdcle");
        if (ff.delegate == FD_CHOOSE)
            le->setEnabled(false);
        else
            le->setEnabled(true);
        pb->setObjectName("fdcpb");
        ml->addWidget(le, 80);
        ml->addWidget(pb,0);
        connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
        ml->setContentsMargins(0, 0, 0, 0);
        combWidget->setLayout(ml);
        editor = combWidget;
        break;
    }
    case FD_COMBO:
    {
        cbWidget = new s_tqwidget(parent);
        cbWidget->setStyleSheet("QWidget {background: khaki};");
        QVBoxLayout *ml = new QVBoxLayout;
        QComboBox *cb;
        cb = new QComboBox(parent);
        cb->setStyleSheet("QComboBox {background: khaki};");
        cb->setObjectName("fdccb");
        QStringListModel *tmpModel = new QStringListModel;
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
        {
            switch (ff.ftype)
            {
            case FW_ALLINK:
            {
                QString id = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), "id"+ff.link.at(1), "alias", ff.link.at(2));
                tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, ff.link.at(1), "alias", "idalias", id);
                break;
            }
            case FW_LINK:
            {
                tmpStringList = sqlc.getvaluesfromtablebycolumn(db, ff.link.at(1), ff.link.at(2));
                break;
            }
            default:
            {
                return editor;
                break;
            }
            }
            tmpModel->setStringList(tmpStringList);
            cb->setModel(tmpModel);
            ml->addWidget(cb);
            cbWidget->setLayout(ml);
            editor = cbWidget;
        }
        break;
    }
    case FD_LINEEDIT:
    {
        if (ff.link.size()) // masked lineedit
        {
            QString tmpString = "";
            for (int i = 0; i < ff.link.size(); i++)
                tmpString += ff.link.at(i)+".";
            tmpString = tmpString.left(tmpString.size()-1);
            s_maskedle *le = new s_maskedle(tmpString, parent);
            le->setObjectName("msle");
            editor = le;
        }
        else
        {
            QLineEdit *le = new QLineEdit(parent);
            le->setObjectName("lele");
            editor = le;
        }
        break;
    }
    case FD_SIMGRID:
    case FD_SIMPLE:
    case FD_DISABLED:
    {
//        QLabel *lbl = new QLabel(parent);
//        lbl->setStyleSheet("QLabel {background: khaki};");
//        editor = lbl;
        break;
    }
    case FD_SPIN:
    {
        int tmpInt = ff.link.at(0).count("n", Qt::CaseSensitive);
        QString tmpString;
        tmpString.fill('9', tmpInt);
        int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
        QString tmpString2;
        tmpString2.fill('9', tmpInt2);
        tmpString += "." + tmpString2;
        s_tqspinbox *sb = new s_tqspinbox(parent);
        sb->setStyleSheet("QSpinBox {background: khaki};");
        sb->setMinimum(0);
        sb->setDecimals(tmpInt2);
        sb->setMaximum(tmpString.toDouble());
        editor = sb;
        break;
    }
    default:
        break;
    }
    return editor;
}

void s_duniversal::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = editor->findChild<s_tqLineEdit*>("fdcle");
        s_tqPushButton *pb = editor->findChild<s_tqPushButton*>("fdcpb");
        pb->setText("...");
        le->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case FD_COMBO:
    {
        QComboBox *cb = static_cast<QComboBox *>(editor);
        cb->setCurrentText(index.data(Qt::EditRole).toString());
        break;
    }
    case FD_SIMGRID:
    case FD_SIMPLE:
    case FD_DISABLED:
    {
//        QLabel *lbl = static_cast<QLabel *>(editor);
//        lbl->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case FD_LINEEDIT:
    {
        QLineEdit *le = static_cast<QLineEdit *>(editor);
        le->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case FD_SPIN:
    {
        s_tqspinbox *sb = static_cast<s_tqspinbox *>(editor);
        sb->setValue(index.data(Qt::EditRole).toDouble());
        break;
    }
    default:
        break;
    }
}

void s_duniversal::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqwidget *wdgt = static_cast<s_tqwidget *>(editor);
        s_tqLineEdit *le = wdgt->findChild<s_tqLineEdit *>("fdcle");
        model->setData(index, le->text(), Qt::EditRole);
        break;
    }
    case FD_COMBO:
    {
        QComboBox *cb = static_cast<QComboBox  *>(editor);
        model->setData(index, cb->currentText(), Qt::EditRole);
        break;
    }
    case FD_SIMGRID:
    case FD_DISABLED:
    case FD_SIMPLE:
        break;
    case FD_LINEEDIT:
    {
        QLineEdit *le = static_cast<QLineEdit *>(editor);
        model->setData(index, le->text(), Qt::EditRole);
        break;
    }
    case FD_SPIN:
    {
        s_tqspinbox *sb = static_cast<s_tqspinbox *>(editor);
        model->setData(index, QVariant(sb->value()), Qt::EditRole);
        break;
    }
    default:
        break;
    }
}

// незакончено - дописать другие варианты

void s_duniversal::pbclicked()
{
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        s_2cdialog *chooseDialog = new s_2cdialog(hdr);
        QSqlDatabase db = sqlc.getdb(ff.link.at(0));
        if (db.isValid())
        {
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            chooseDialog->setMinimumWidth(500);
            QString id = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), "id"+ff.link.at(1), "alias", ff.link.at(2));
            QStringList tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, ff.link.at(1), "alias", "idalias", id);
            chooseDialog->setup(tmpStringList);
            chooseDialog->exec();
        }
        break;
    }
    case FW_LINK:
    {
        QStringList tmpfl = QStringList() << "tablefields" << "tablename";
        QStringList tmpvl = QStringList() << "alias" << ff.link.at(0);
        sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"),"tablefields","tablefields",tmpfl,tmpvl);
        if (!sqlc.result) // это дерево
        {
            s_2ctdialog *chooseDialog = new s_2ctdialog(hdr);
            chooseDialog->setup(ff.link.at(0));
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit *>("fdcle");
            chooseDialog->setTvCurrentText(le->text());
            chooseDialog->exec();
        }
        else // это таблица
        {
            s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit *>("fdcle");
            if (le == 0)
                break;
            s_2cdialog *chooseDialog = new s_2cdialog(hdr);
            int res = chooseDialog->setup(ff.link.at(0), le->text());
            if (!res)
            {
                connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
                chooseDialog->setMinimumWidth(500);
/*                QList<QStringList> tmpsl;
                QStringList tmpfl=QStringList()<<"id"+ff.link.at(1)<<ff.link.at(2);
                tmpsl=sqlc.getvaluesfromtablebycolumns(db,ff.link.at(1),tmpfl);
                QStringList tmpl1,tmpl2, tmpll1;
                for (int i = 0; i < tmpsl.size(); i++)
                {
                    tmpl1 << QString("%1").arg(tmpsl.at(i).at(0).toInt(0), 7, 10, QChar('0'));
                    tmpll1 << QString::number(FD_SIMGRID);
                    tmpl2 << tmpsl.at(i).at(1);
                }
                chooseDialog->setup(tmpl1, tmpll1, tmpl2, QStringList()); */
                chooseDialog->sortModel();
                chooseDialog->exec();
            }
        }
        break;
    }
    case FW_DLINK:
    {
        break;
    }
    case FW_RIGHTS:
    {
        // вызов диалога редактирования прав доступа
        break;
    }
    case FW_DATE:
    {
        s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit *>("fdcle");
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

// незакончено - дописать другие варианты

void s_duniversal::accepted(QString str)
{
    try
    {
        QString tmpString = str;
        s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit*>("fdcle");
        if (le == 0)
            throw 0x21;
        switch (ff.ftype)
        {
/*        {
            QSqlDatabase db = sqlc.getdb(ff.link.at(0));
            if (db.isValid())
                tmpString = sqlc.getvaluefromtablebyfield(db, ff.link.at(1), ff.link.at(1), "id"+ff.link.at(1), str);
            break;
        } */
        case FW_ALLINK:
        case FW_LINK:
        {
            QStringList tmpStringList = tfl.GetOneValueByOneRowAndId(ff.link.at(0), "Наименование", str);
            tmpString = tmpStringList.at(0);
            break;
        }
        case FW_DLINK:
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
        case FW_RIGHTS:
        {
            break;
        }
        case FW_DATE:
            break;
        default:
            break;
        }
        le->setText(tmpString);
    }
    catch (int res)
    {
        Q_UNUSED(res);
        return;
    }
}

void s_duniversal::dateChoosed(QDate dte)
{
    accepted(dte.toString("dd/MM/yyyy"));
}

void s_duniversal::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    if (index.isValid())
    {
        QRect rct = option.rect;
        s_duniversal::fieldformat tmpff = getFFfromLinks(index.data(Qt::UserRole).toString());
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::SolidLine);
            painter->setPen(QColor(Qt::darkGreen));
            painter->drawRoundedRect(rct, 3, 3);
            painter->restore();
            return;
        }
        if (tmpff.delegate != FD_SIMPLE)
        {
            painter->setPen(Qt::SolidLine);
            painter->setPen(QColor(Qt::darkGray));
            painter->drawRoundedRect(rct, 3, 3);
        }
    }
    painter->restore();
}

/*QSize s_duniversal::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
QStyleOptionViewItemV4 options = option;
initStyleOption(&options, index);

QSize size = QStyledItemDelegate::sizeHint(option, index);
return QSize(size.width(), size.height());
} */

s_duniversal::fieldformat s_duniversal::getFFfromLinks(QString links) const
{
    QStringList tmpsl = links.split(".");
    fieldformat ff;
    ff.ftype = 8;
    ff.delegate = 4;
    ff.dependson = -1;
    ff.link.clear();
    if (!tmpsl.size())
        return ff;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    ff.ftype = tmpsl.at(0).toInt();
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

void s_duniversal::setTableHeader(QString hdr)
{
    this->hdr = hdr;
}
