#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir tmpCurrDir = QDir::current();
    tmpCurrDir.cdUp();
    QString rootDir = tmpCurrDir.path() + "/PauseAbortQt/PythonDemo";
    PythonRunner * runner= new PythonRunner("pauseAbort", rootDir, "pauseAbortDemo");

    MainWindow w(runner);
    w.show();

    return a.exec();
}
