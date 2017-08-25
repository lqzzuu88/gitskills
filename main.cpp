#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setHidden(true);
    if(w.isShowMainWin())
        w.show();

    return a.exec();
}
