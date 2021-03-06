#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QFrame>
#include <QCloseEvent>
#include <QShowEvent>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>
#include <QFile>

#include "../../gen/publicclass.h"
#include "sys_settingsdialog.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../gen/messagebox.h"

sys_settingsdialog::sys_settingsdialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void sys_settingsdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/SysWallpaper.png"));

    event->accept();
}

void sys_settingsdialog::SetupUI ()
{
    setMinimumSize(QSize(384, 194));
    setMaximumSize(QSize(16661, 11616));
    QIcon qssdIcon;
    qssdIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(qssdIcon);

    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Настройки");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    QGridLayout *glyout = new QGridLayout;
    glyout->setColumnStretch(0,0);
    glyout->setColumnStretch(1,1);

/*    lbl = new s_tqLabel("Путь к каталогам библиотек Altium:");
    glyout->addWidget(lbl,0,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true); // transparent=true
    cw->Setup("2.16..");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(PathToLibsChoosed(QVariant)));
    cw->SetValue(pc.PathToLibs);
    glyout->addWidget(cw,0,1,1,1);

    lbl = new s_tqLabel("Рабочий каталог СУПиКа:");
    glyout->addWidget(lbl,1,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    cw = new s_tqChooseWidget(true); // transparent=true
    cw->Setup("2.16..");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SupikDirChoosed(QVariant)));
    cw->SetValue(pc.PathToSup);
    glyout->addWidget(cw,1,1,1,1);

    lbl = new s_tqLabel("Адрес FTP-сервера архива СУПиК:");
    glyout->addWidget(lbl,2,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    s_tqLineEdit *le = new s_tqLineEdit;
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(FtpNameChoosed(QString)));
    le->setText(pc.FtpServer);
    glyout->addWidget(le,2,1,1,1); */

    lbl = new s_tqLabel("Адрес сервера СУПиК:");
    glyout->addWidget(lbl,3,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    s_tqLineEdit *le = new s_tqLineEdit;
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SupikServerChoosed(QString)));
    le->setText(pc.SupikServer);
    glyout->addWidget(le,3,1,1,1);

    lbl = new s_tqLabel("Порт сервера СУПиК:");
    glyout->addWidget(lbl,4,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    le = new s_tqLineEdit;
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SupikPortChoosed(QString)));
    le->setText(pc.SupikPort);
    glyout->addWidget(le,4,1,1,1);

    lbl = new s_tqLabel("Адрес SQL-сервера:");
    glyout->addWidget(lbl,5,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    le = new s_tqLineEdit;
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SqlPathChoosed(QString)));
    le->setText(pc.SQLPath);
    glyout->addWidget(le, 5, 1, 1, 1);

    lbl = new s_tqLabel("Язык системы:");
    glyout->addWidget(lbl,6,0,1,1);
    glyout->setAlignment(lbl,Qt::AlignRight);
    s_tqComboBox *LangCB = new s_tqComboBox;
    connect(LangCB,SIGNAL(currentIndexChanged(QString)),this,SLOT(LangChoosed(QString)));
    LangCB->addItem(QIcon(":/res/LangRU.png"), "RU");
    LangCB->addItem(QIcon(":/res/langGB.png"), "EN");
    LangCB->setCurrentText(pc.CurLang);
    glyout->addWidget(LangCB, 6, 1, 1, 1);

    s_tqPushButton *isOKPB = new s_tqPushButton (QColor(0,135,0), "Ага");
    s_tqPushButton *CancelPB = new s_tqPushButton (QColor(185,0,0), "Неа");
    isOKPB->setMaximumSize(75, 23);
    CancelPB->setMaximumSize(75, 23);

    lbl = new s_tqLabel("Период обновления информации (мин)");
    glyout->setAlignment(lbl,Qt::AlignRight);
    glyout->addWidget(lbl,7,0,1,1);
    s_tqSpinBox *spb = new s_tqSpinBox;
    spb->setMinimum(0.1);
    spb->setMaximum(9999.0);
    spb->setDecimals(1);
    spb->setSingleStep(0.1);
    connect(spb,SIGNAL(valueChanged(double)),this,SLOT(TimerPeriodChoosed(double)));
    spb->setValue(pc.timerperiod);
    glyout->addWidget(spb,7,1,1,1);

    hlyout = new QHBoxLayout;
    hlyout->addWidget(isOKPB);
    hlyout->addWidget(CancelPB);
    glyout->addLayout(hlyout,8,0,1,2);

    lyout->addLayout(glyout);
    lyout->addSpacing(500);
    setLayout(lyout);

    connect (isOKPB, SIGNAL(clicked()), this, SLOT(OKPBClicked()));
    connect (CancelPB, SIGNAL(clicked()), this, SLOT(CancelPBClicked()));\
}

void sys_settingsdialog::PathToLibsChoosed(QVariant value)
{
    pc.PathToLibs = value.toString();
}

void sys_settingsdialog::SupikDirChoosed(QVariant value)
{
    pc.PathToSup = value.toString();
}

void sys_settingsdialog::FtpNameChoosed(QString str)
{
    s_tqLineEdit *le = static_cast<s_tqLineEdit *>(sender());
    pc.FtpServer = le->text();
}

void sys_settingsdialog::SqlPathChoosed(QString str)
{
    pc.SQLPath = str;
}

void sys_settingsdialog::LangChoosed(QString lang)
{
    pc.CurLang = lang;
}

void sys_settingsdialog::TimerPeriodChoosed(double dbl)
{
    pc.timerperiod = dbl;
}

void sys_settingsdialog::SupikServerChoosed(QString ip)
{
    pc.SupikServer = ip;
}

void sys_settingsdialog::SupikPortChoosed(QString port)
{
    pc.SupikPort = port;
}

void sys_settingsdialog::CancelPBClicked()
{
    this->close();
}

void sys_settingsdialog::OKPBClicked()
{
/*    QString tmpString = pc.PathToLibs + "//Symbols//Capasitors.SchLib";
    QFile file;

    file.setFileName(tmpString);
    if (!file.exists())
    {
        SYSSETINFO("Неправильный путь к файлам библиотеки");
        return;
    }
    else
    { */
        pc.SetRegValue("settings/pathtolibs",pc.PathToLibs);
        pc.SetRegValue("settings/pathtosup",pc.PathToSup);
        pc.SetRegValue("settings/lang",pc.CurLang);
        pc.SetRegValue("settings/SQLPath",pc.SQLPath);
        pc.SetRegValue("settings/timerperiod",QString::number(pc.timerperiod));
        pc.SetRegValue("settings/FtpServer",pc.FtpServer);
        pc.SetRegValue("settings/Server",pc.SupikServer);
        pc.SetRegValue("settings/Port",pc.SupikPort);
        if (!pl.InitLang())
            pl.SetDefaultLang();

        MessageBox2::information(this,"Внимание","Некоторые настройки вступят в силу \nтолько после перезапуска СУПиКа");
        this->close();
//    }
}
