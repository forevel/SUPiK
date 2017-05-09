#include "textwithpicdialog.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqtextedit.h"
#include "../../widgets/waitwidget.h"
#include "../../widgets/wd_func.h"
#include "../../gen/publicclass.h"
#include "../../gen/client.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QFileDialog>
#include <QPainter>
#include <QThread>
#include <QPixmap>

TextWithPicDialog::TextWithPicDialog(int TEType, QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *vlyout = new QVBoxLayout;
    if (TEType != TWPD_PICONLY)
    {
        s_tqTextEdit *te = new s_tqTextEdit;
        te->setObjectName("tedit");
        vlyout->addWidget(te, 95);
    }
    if (TEType != TWPD_TEONLY)
    {
        s_tqLineEdit *le = new s_tqLineEdit;
        le->setObjectName("tefilele");
        QHBoxLayout *hlyout = new QHBoxLayout;
        s_tqLabel *lbl = new s_tqLabel("Имя файла:");
        hlyout->addWidget(lbl, 5);
        hlyout->addWidget(le, 90);
        s_tqPushButton *pb = new s_tqPushButton("...");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(ChooseTEFile()));
        hlyout->addWidget(pb, 5);
        vlyout->addLayout(hlyout);
    }
    s_tqPushButton *pb = new s_tqPushButton("Готово");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(tepbclicked()));
    vlyout->addWidget(pb, 5);
    setLayout(vlyout);
}

void TextWithPicDialog::SetFileField(const QString &filename)
{
    WDFunc::SetLEData(this, "tefilele", filename);
}

void TextWithPicDialog::SetLink(QStringList &link)
{
    this->link = link;
}

void TextWithPicDialog::GetDirStringFromLink(QStringList &link, QString &out)
{
    QString tmps;
    if (!link.isEmpty())
    {
        bool ok;
        int tmpi = link.at(0).toInt(&ok);
        if (ok && (tmpi < Cli->PathPrefixes.size()))
            tmps += Cli->PathPrefixes.at(tmpi);
        if (link.size() > 1) // есть ещё и суффикс
        {
            tmpi = link.at(1).toInt(&ok);
            if (ok && (tmpi < Cli->PathSuffixes.size()))
                tmps += Cli->PathSuffixes.at(tmpi);
        }
    }
    out = pc.HomeDir+"/"+tmps;
}

void TextWithPicDialog::SetTEField(const QString &tefield)
{
    WDFunc::SetTEData(this, "tedit", tefield);
}

void TextWithPicDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void TextWithPicDialog::tepbclicked()
{
    QString tmps, sourcefile, shortfile;
    WDFunc::TEData(this, "tedit", tmps);
    if (!link.isEmpty())
    {
        WDFunc::LEData(this, "tefilele", sourcefile);
        if (!sourcefile.isEmpty())
        {
            // составляем из файла и текста запись
            tmps.insert(0, TOKEN);
            // оставляем только само имя файла
            QStringList sl = sourcefile.split("/");
            shortfile = sl.last();
            // скопируем файл, где бы они ни был, в определённый в links каталог
            QString destfile;
            GetDirStringFromLink(link, destfile);
            destfile += shortfile; // создали путь назначения
            QFile fp(sourcefile);
            fp.copy(destfile);
            fp.close();
            // отправим файл на сервер в нужный каталог
            if (link.size() < 2)
                DBGMSG;
            else if (Cli->PutFile(destfile, link.at(0), link.at(1), shortfile) != Client::CLIER_NOERROR)
                WARNMSG("Can't send file to server:"+destfile);
            tmps.insert(0, shortfile);
        }
    }
    emit ChangesHasBeenMade(tmps);
    this->close();
}

void TextWithPicDialog::ChooseTEFile()
{
    QString CurPath;
    GetDirStringFromLink(link, CurPath);
    QDir *dr = new QDir;
    dr->mkpath(CurPath);
    delete dr;
    QFileDialog *fdlg = new QFileDialog;
//    connect(fdlg,SIGNAL(directoryEntered(QString)),this,SLOT(TEDirCheck(QString)));
    connect(fdlg,SIGNAL(fileSelected(QString)),this,SLOT(TEFileChoosed(QString)));
    fdlg->setAcceptMode(QFileDialog::AcceptOpen);
    fdlg->setDirectory(CurPath);
    fdlg->exec();
}

void TextWithPicDialog::TEFileChoosed(QString filename)
{
    WDFunc::SetLEData(this, "tefilele", filename);
}

/*void s_tqChooseWidget::TEDirCheck(QString dir)
{
    QFileDialog *fdlg = static_cast<QFileDialog *>(sender());
    if (fdlg != 0)
    {
        if (dir != CurPath)
            fdlg->setDirectory(CurPath);
    }
    else
        DBGMSG;
} */
