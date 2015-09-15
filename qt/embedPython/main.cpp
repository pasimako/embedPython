#include "pyrun.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PyRun *pyRun = new PyRun(a.applicationFilePath());
    MainWindow w(pyRun);
    w.show();

    return a.exec();
}
