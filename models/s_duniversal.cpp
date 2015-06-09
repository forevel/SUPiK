#include "s_duniversal.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqspinbox.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqcalendarwidget.h"
#include "../gen/s_sql.h"
#include "s_ncmodel.h"
#include "../dialogs/s_2cdialog.h"
#include "../dialogs/s_2ctdialog.h"
#include "../dialogs/s_accessdialog.h"
#include "../widgets/s_maskedle.h"
#include "../gen/s_tablefields.h"
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
    QString links = index.data(Qt::UserRole).toString();
    ff = pc.getFFfromLinks(links);
    hdr=index.data(Qt::UserRole+1).toString(); // в UserRole+1 должна содержаться aData, в которой находится подзаголовок диалога редактирования, вызываемого по кнопке в делегате
    QWidget *editor = 0;
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        combWidget = new s_tqWidget(parent);
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
        cbWidget = new s_tqWidget(parent);
        cbWidget->setStyleSheet("QWidget {background: khaki};");
        QVBoxLayout *ml = new QVBoxLayout;
        QComboBox *cb;
        cb = new QComboBox(parent);
        cb->setStyleSheet("QComboBox {background: khaki};");
        cb->setObjectName("fdccb");
        QStringListModel *tmpModel = new QStringListModel;
        switch (ff.ftype)
        {
        case FW_ALLINK:
        case FW_LINK:
        {
            tmpStringList = tfl.idtovl(links);
            if (tfl.result)
                return editor;
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
        break;
    }
    case FD_LINEEDIT:
    {
        if (ff.ftype == FW_MASKED)
        {
            QString tmpString = "";
            for (int i = 0; i < ff.link.size(); i++)
                tmpString += ff.link.at(i)+"."; // возвращение строке regexp первоначального вида, "побитого" при getFFfromlinks
            tmpString = tmpString.left(tmpString.size()-1);
            s_MaskedLineEdit *le = new s_MaskedLineEdit(tmpString, parent);
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
        break;
    case FD_SPIN:
    {
        int tmpInt = ff.link.at(0).count("n", Qt::CaseSensitive);
        QString tmpString;
        tmpString.fill('9', tmpInt);
        int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
        QString tmpString2;
        tmpString2.fill('9', tmpInt2);
        tmpString += "." + tmpString2;
        s_tqSpinBox *sb = new s_tqSpinBox(parent);
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
        connect(le,SIGNAL(textChanged(QString)),this,SLOT(commitChanges(QString)));
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
        break;
    case FD_LINEEDIT:
    {
        QLineEdit *le = static_cast<QLineEdit *>(editor);
        le->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = static_cast<s_tqSpinBox *>(editor);
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
        s_tqWidget *wdgt = static_cast<s_tqWidget *>(editor);
        s_tqLineEdit *le = wdgt->findChild<s_tqLineEdit *>("fdcle");
        model->setData(index, QVariant(le->text()), Qt::EditRole);
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
        model->setData(index, QVariant(le->text()), Qt::EditRole);
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = static_cast<s_tqSpinBox *>(editor);
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
    s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit *>("fdcle");
    if (le == 0)
        return;
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        // взять ИД корневого элемента с именем "ff.link.at(1)"
        QStringList tmpStringList = tfl.htovlc(ff.link.at(0),"ИД","Наименование",ff.link.at(1));
        if (tfl.result)
            return;
        // сформировать список выбора из значений по полю "Наименование", для которых idalias равен результату на пред. этапе
        tmpStringList = tfl.htovlc(ff.link.at(0), "Наименование", "ИД_а", tmpStringList.at(0));
        s_2cdialog *chooseDialog = new s_2cdialog;
        chooseDialog->setup(tmpStringList, hdr, QStringList(), le->text());
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->exec();
        break;
    }
    case FW_LINK:
    {
        if (tfl.tableistree(ff.link.at(0))) // это дерево
        {
            s_2ctdialog *chooseDialog = new s_2ctdialog(hdr);
            chooseDialog->setup(ff.link.at(0));
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            chooseDialog->setTvCurrentText(le->text());
            chooseDialog->exec();
        }
        else // это таблица
        {
            s_2cdialog *chooseDialog = new s_2cdialog;
            chooseDialog->setup(ff.link.at(0), MODE_CHOOSE, "", hdr, le->text());
            if (!chooseDialog->result)
            {
                connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
                chooseDialog->exec();
            }
        }
        break;
    }
    case FW_DLINK:
    {
        int count = ff.link.size(); // в поле link - имена таблиц
        if (count == 0)
            break;
        s_2cdialog *dlg = new s_2cdialog;
        dlg->setup(ff.link.at(0),MODE_CHOOSE,"",hdr);
        for (int i = 1; i < count; i++)
            dlg->AddTable(ff.link.at(i));
        connect(dlg,SIGNAL(changeshasbeenMade(QString)),this,SLOT(accepted(QString)));
        dlg->SetTvCurrentText(le->text());
        dlg->exec();
        break;
    }
    case FW_RIGHTS:
    {
        // вызов диалога редактирования прав доступа
        s_accessdialog *dlg = new s_accessdialog;
        dlg->SetupUI(le->text().toLongLong(0, 16));
        connect(dlg, SIGNAL(acceptChanges(long)), this, SLOT(AcceptAccess(long)));
        dlg->exec();
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
    case FW_SPECIAL:
    {

        break;
    }
    default:
        break;
    }
}

// незакончено - дописать другие варианты

void s_duniversal::accepted(QString str)
{
    s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit*>("fdcle");
    if (le == 0)
        return;
    QString tmpString = tfl.idtov(pc.getlinksfromFF(ff),str);
    le->setText(tmpString);
}

void s_duniversal::commitChanges(QString str)
{
    Q_UNUSED(str);
    emit commitData(combWidget);
    emit closeEditor(combWidget);
}

void s_duniversal::dateChoosed(QDate dte)
{
    accepted(dte.toString("dd/MM/yyyy"));
}

void s_duniversal::AcceptAccess(long tmpl)
{
    accepted(QString::number(tmpl, 16));
}

void s_duniversal::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    if (index.isValid())
    {
        QRect rct = option.rect;
        PublicClass::fieldformat tmpff = pc.getFFfromLinks(index.data(Qt::UserRole).toString());
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

void s_duniversal::setTableHeader(QString hdr)
{
    this->hdr = hdr;
}
