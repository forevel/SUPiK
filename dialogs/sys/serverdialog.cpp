#include "serverdialog.h"
#include <QVBoxLayout>
#include <QFile>
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqtextedit.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/client.h"

ServerDialog::ServerDialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("stw");
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Log files");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLogFiles()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("Server status");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowServerStatus()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("Закрыть");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(close()));
    lyout->addWidget(pb);
    s_tqWidget *w = new s_tqWidget;
    w->setLayout(lyout);
    stw->addWidget(w);
    lyout = new QVBoxLayout;
    lyout->addWidget(stw);
    setLayout(lyout);
}

void ServerDialog::ShowLogFiles()
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
    if (stw == 0)
    {
        WARNMSG("Stacked widget not found");
        return;
    }
    s_tqWidget *w = new s_tqWidget;
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Main log");
    pb->setObjectName("supikd");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLog()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("CT log");
    pb->setObjectName("clog");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLog()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("RP log");
    pb->setObjectName("readparse");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLog()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("SQ log");
    pb->setObjectName("sqlparse");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLog()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("Files log");
    pb->setObjectName("files");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowLog()));
    lyout->addWidget(pb);
    pb = new s_tqPushButton("Закрыть");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(close()));
    lyout->addWidget(pb);
    w->setLayout(lyout);
    stw->addWidget(w);
    stw->setCurrentWidget(w);
}

void ServerDialog::ShowServerStatus()
{
    int res = Cli->SendAndGetResult(M_STATUS);
    if (res == Client::CLIER_EMPTY)
    {
        WARNMSG("Empty response");
        return;
    }
    else if (res != Client::CLIER_NOERROR)
    {
        ERMSG("Error response");
        return;
    }
    QDialog *dlg = new QDialog;
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqTextEdit *te = new s_tqTextEdit;
    te->setPlainText(Cli->ResultStr);
    te->setEnabled(false);
    lyout->addWidget(te);
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked(bool)),dlg,SLOT(close()));
    lyout->addWidget(pb);
    dlg->setLayout(lyout);
    dlg->exec();
}

void ServerDialog::ShowLog()
{
    QString filename = sender()->objectName();
    filename += ".log";
    int res = Cli->GetFile(FLT_LOG, FLST_NONE, filename);
    if (res != Client::CLIER_NOERROR)
    {
        WARNMSG("No such file: " + filename);
        return;
    }
    QFile fp;
    QString path = pc.HomeDir + "/log/";
    fp.setFileName(path + filename);
    if (!fp.open(QIODevice::ReadOnly))
    {
        ERMSG("Невозможно открыть файл "+filename);
        return;
    }
    QString inbuf = QString::fromUtf8(fp.readAll());
    fp.close();
    QDialog *dlg = new QDialog;
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqTextEdit *te = new s_tqTextEdit;
    te->setPlainText(inbuf);
    te->setEnabled(true);
    lyout->addWidget(te);
    s_tqPushButton *pb = new s_tqPushButton("Ага");
    connect(pb,SIGNAL(clicked(bool)),dlg,SLOT(close()));
    lyout->addWidget(pb);
    dlg->setLayout(lyout);
    dlg->exec();
}
