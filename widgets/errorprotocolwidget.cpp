#include <QVBoxLayout>
#include <QHeaderView>
#include "s_tqtableview.h"
#include "s_tqlabel.h"
#include "errorprotocolwidget.h"

ErrorProtocolWidget::ErrorProtocolWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QWidget *w = new QWidget;
    QString ErrWss = "QWidget {background-color: "+QString(ERPROTCLR)+";}";
    w->setStyleSheet(ErrWss);
    s_tqTableView *tv = new s_tqTableView;
    ErrorProtocolModel *erm = new ErrorProtocolModel;
    erm->setHeaderData(0, Qt::Horizontal, "Номер",Qt::EditRole);
    erm->setHeaderData(1, Qt::Horizontal, "Дата/время",Qt::EditRole);
    erm->setHeaderData(2, Qt::Horizontal, "Модуль",Qt::EditRole);
    erm->setHeaderData(3, Qt::Horizontal, "Строка",Qt::EditRole);
    erm->setHeaderData(4, Qt::Horizontal, "Сообщение",Qt::EditRole);
    tv->setModel(erm);
    tv->resizeColumnsToContents();
    connect(erm,SIGNAL(dataChanged(QModelIndex,QModelIndex)),tv,SLOT(resizeColumnsToContents()));
    tv->horizontalHeader()->setStretchLastSection(true);
    tv->horizontalHeader()->setEnabled(false);
    tv->verticalHeader()->setVisible(false);
    tv->verticalHeader()->setDefaultSectionSize(tv->verticalHeader()->fontMetrics().height()+2);
    tv->setGridStyle(Qt::SolidLine);
    tv->setShowGrid(true);
    tv->setObjectName("ertv");
//    tv->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    QHBoxLayout *upperLayout = new QHBoxLayout;
    upperLayout->addWidget(line,2);
    s_tqLabel *lbl = new s_tqLabel("Протокол работы СУПиК");
    lbl->setEnabled(false);
    upperLayout->addWidget(lbl,0);
    upperLayout->setAlignment(lbl,Qt::AlignCenter);
    line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    upperLayout->addWidget(line,2);
    lyout->addLayout(upperLayout,0);
    lyout->addWidget(tv,5);
    w->setLayout(lyout);
    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(w);
    setLayout(l);
}

ErrorProtocolWidget::~ErrorProtocolWidget()
{

}

void ErrorProtocolWidget::AddRowToProt(PublicClass::ermsg ermsg)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("ertv");
    if (tv == 0)
    {
        DBGMSG(PublicClass::ER_SUPIK,__LINE__);
        return;
    }
    ErrorProtocolModel *erm = static_cast<ErrorProtocolModel *>(tv->model());
    erm->AddRow(ermsg);
}

ErrorProtocolModel::ErrorProtocolModel(QObject *parent) : QAbstractTableModel (parent)
{
    MsgCount = 1;
}

QVariant ErrorProtocolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < hdr.size()))
        return hdr.at(section);
    return QVariant();
}

bool ErrorProtocolModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal)
    {
        if (section >= hdr.size())
        {
            while (hdr.size()<=section)
                hdr.append("");
        }
        hdr.replace(section, value.toString());
    }
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

QVariant ErrorProtocolModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (index.row() < erdata.size())
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
                return erdata.at(index.row()).at(index.column());
            else if (role == Qt::ForegroundRole)
            {
                if (ertypedata.at(index.row()) == PublicClass::ER_MSG)
                    return QVariant::fromValue(QColor(255,0,0));
                else if (ertypedata.at(index.row()) == PublicClass::WARN_MSG)
                    return QVariant::fromValue(QColor(255,0,255));
                else if (ertypedata.at(index.row()) == PublicClass::INFO_MSG)
                    return QVariant::fromValue(QColor(0,150,67));
                else if (ertypedata.at(index.row()) == PublicClass::DBG_MSG)
                    return QVariant::fromValue(QColor(0,0,255));
                else
                    return QVariant();
            }
            else if (role == Qt::BackgroundRole)
            {
                if (ertypedata.at(index.row()) == PublicClass::ER_MSG)
                    return QVariant::fromValue(QColor(255,139,137)); // red
                else if (ertypedata.at(index.row()) == PublicClass::WARN_MSG)
                    return QVariant::fromValue(QColor(255,190,255)); // magenta
                else if (ertypedata.at(index.row()) == PublicClass::INFO_MSG)
                    return QVariant::fromValue(QColor(204,253,243)); // green
                else if (ertypedata.at(index.row()) == PublicClass::DBG_MSG)
                    return QVariant::fromValue(QColor(190,255,255)); // blue
                else
                    return QVariant();
            }
            else if ((role == Qt::DecorationRole) && (index.column() == 4))
            {
                if (ertypedata.at(index.row()) == PublicClass::ER_MSG)
                    return QVariant::fromValue(QIcon(":/res/er_msg.png"));
                else if (ertypedata.at(index.row()) == PublicClass::WARN_MSG)
                    return QVariant::fromValue(QIcon(":/res/warn_msg.png"));
                else if (ertypedata.at(index.row()) == PublicClass::INFO_MSG)
                    return QVariant::fromValue(QIcon(":/res/info_msg.png"));
                else if (ertypedata.at(index.row()) == PublicClass::DBG_MSG)
                    return QVariant::fromValue(QIcon(":/res/dbg_msg.png"));
                else
                    return QVariant();
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags ErrorProtocolModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int ErrorProtocolModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return erdata.size();
}

int ErrorProtocolModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 5;
}

void ErrorProtocolModel::AddRow(PublicClass::ermsg msg)
{
    beginResetModel();
    if (rowCount()<MAX_MSG)
        insertRows(0,1,QModelIndex());
    QStringList tmpsl = QStringList() << "#"+QString::number(MsgCount) << msg.DateTime << msg.module << QString::number(msg.line,10) << msg.msg;
    MsgCount++;
    erdata.insert(0,tmpsl);
    if (erdata.size() >= MAX_MSG)
        erdata.removeLast();
    ertypedata.insert(0,msg.type);
    if (ertypedata.size() >= MAX_MSG)
        ertypedata.removeLast();
    endResetModel();
    emit dataChanged(index(0, 0), index(rowCount(),columnCount()));
}
