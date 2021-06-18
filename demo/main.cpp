#include "mainwindow.h"
#include <QApplication>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
