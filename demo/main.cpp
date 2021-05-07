#include "mainwindow.h"

#include <QApplication>
#include <stdlib.h>

double step = 0;
int N = 0;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //N = atoi(argv[1]);
    //step = atof(argv[2]);

    MainWindow w;
    w.show();
    return app.exec();


}
