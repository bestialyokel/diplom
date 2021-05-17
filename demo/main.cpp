#include "mainwindow.h"

#include <QApplication>
#include <stdlib.h>

double step = 0;
int N = 0;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    //w.layout()->setSizeConstraint(QLayout::SetFixedSize);
    w.show();
    return app.exec();


}
