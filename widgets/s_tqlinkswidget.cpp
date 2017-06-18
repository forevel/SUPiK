#include <QCoreApplication>
#include <QHBoxLayout>
#include "s_tqlinkswidget.h"
#include "s_tqlineedit.h"
#include "s_tqpushbutton.h"
#include "s_tqcalendarwidget.h"
#include "s_tqspinbox.h"
#include "s_tqcombobox.h"
#include "s_tqcheckbox.h"
#include "s_tqdatetimewidget.h"
#include "s_maskedle.h"
#include "s_tqlabel.h"
#include "s_tqtextedit.h"
#include "wd_func.h"
#include "../gen/s_sql.h"
#include "../gen/client.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../dialogs/gen/twocoldialog.h"
#include "../dialogs/gen/chooseitemdialog.h"
#include "../dialogs/gen/specialdialog.h"
#include "../dialogs/gen/accessdialog.h"
#include "../dialogs/gen/textwithpicdialog.h"

// универсальный класс для вывода значений по описанию links из таблиц БД СУПиК

s_tqLinksWidget::s_tqLinksWidget(const QString &links, const QString &value, QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QWidget {background-color: rgba(0,0,0,0);}");
    PublicClass::FieldFormat ff;
    pc.getFFfromLinks(links, ff);
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *vlyout = new QVBoxLayout;
    vlyout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0,0,0,0);
    switch (ff.ftype)
    {
    case FW_PIXTE:
    {
        QFont font;
        QHBoxLayout *hlyout = new QHBoxLayout;
        if (value.contains(DATA_TOKEN)) // есть составная запись
        {
            bool ok;
            QFile fp;
            QStringList tmpsl = value.split(DATA_TOKEN);
            // вытащим путь к файлу из links
            QString path = pc.HomeDir + "/";
            int fltype = ff.link.at(0).toInt(&ok);
            if ((ok) && (fltype < Cli->PathPrefixes.size()))
                path += Cli->PathPrefixes.at(fltype);
            int flsubtype = ff.link.at(1).toInt(&ok);
            if ((ok) && (flsubtype < Cli->PathSuffixes.size()))
                path += Cli->PathSuffixes.at(flsubtype);
            path += tmpsl.at(0); // имя файла
            fp.setFileName(path);
            if (fp.exists())
            {
                // добавим метку с изображением из файла
                s_tqLabel *lbl = new s_tqLabel;
                QPixmap pm(path);
                lbl->setPixmap(pm);
                hlyout->addWidget(lbl);
            }
            else
            {
                if (Cli->GetFile(fltype, flsubtype, tmpsl.at(0)) == Client::CLIER_NOERROR)
                {
                    // добавим метку с изображением из файла
                    s_tqLabel *lbl = new s_tqLabel;
                    QPixmap pm(path);
                    lbl->setPixmap(pm);
                    hlyout->addWidget(lbl);
                }
            }
            if (tmpsl.size()>1)
            {
                s_tqLabel *lbl = new s_tqLabel(tmpsl.at(1));
                font.setPointSize(13);
                lbl->setFont(font);
                lbl->setWordWrap(true);
                hlyout->addWidget(lbl, 100);
            }
        }
        else
        {
            s_tqLabel *lbl = new s_tqLabel(value);
            font.setPointSize(13);
            lbl->setFont(font);
            lbl->setWordWrap(true);
            hlyout->addWidget(lbl, 100);
        }
        vlyout->addLayout(hlyout);
        break;
    }
    default:
    {
        QString tmps;
        tfl.idtov(links, value, tmps);
        s_tqLabel *lbl = new s_tqLabel(tmps);
        vlyout->addWidget(lbl);
        break;
    }
    }
    setLayout(vlyout);
}
