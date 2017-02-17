#include "startwindow.h"
#include <QtWidgets/QApplication>
#include <QObject>
#include <QStyleFactory>
#include <QStandardPaths>
#include "gen/publicclass.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    switch (type)
    {
    case QtDebugMsg:    pc.AddErrMsg(PublicClass::DBG_MSG, 0, 0, msg); break;
    case QtWarningMsg:  pc.AddErrMsg(PublicClass::WARN_MSG, 0, 0, msg); break;
    case QtCriticalMsg: pc.AddErrMsg(PublicClass::ER_MSG, 0, 0, msg); break;
    case QtFatalMsg:    pc.AddErrMsg(PublicClass::ER_MSG, 0, 0, msg); break;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyle(QStyleFactory::create("Plastique"));
    pc.HomeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    StartWindow w;

    w.show();
    
    return a.exec();
}
