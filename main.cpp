#include "startwindow.h"
#include <QtWidgets/QApplication>
#include <QObject>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Plastique"));
    StartWindow w;

    w.show();
    
    return a.exec();
}
