#include <QFont>
#include <QVBoxLayout>
#include "systableeditor.h"
#include "../../widgets/s_tqlabel.h"

systableeditor::systableeditor(QWidget *parent) :
    QDialog(parent)
{
}

void systableeditor::SetupUI(QString tblename)
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Редактор таблиц");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl);
    lyout->setAlignment(lbl, Qt::AlignRight);

    setLayout(lyout);
}
