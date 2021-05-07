#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include "qcustomplot.h"

#include "types.h"
#include "machine.h"

extern int N;
extern double step;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    double tau = 1;
    double T = 1000;
    int tCount = (T/tau);

    Payload p;
    RLC rlc = {
        .R = 10,
        .L = 250,
        .C = 1,
    };

    p.tau = tau;


    p.I = vector<double>(tCount);
    p.U = vector<double>(tCount);

    double i = 0;
    while (i < tCount) {
        double x = i/tCount;
        if (x > 0.1) {
            p.U[i] = 12.5;
        } else {
            p.U[i] = 12;
        }

        if (x > 0.5) {
            p.I[i] = 0.5;
        } else {
            p.I[i] = 1;
        }
        i = i+1;
    }

    Machine m(10, rlc);

    m.appendPayload(p);

    Payload last = m.processNextPayload();


    for (auto x : last.U) {
        //cout << x << endl;
    }


    QCustomPlot* customPlot = findChild<QCustomPlot*>("plot");

    QVector<double> x(tCount), U(tCount), I(tCount);
    for (int i=0; i<tCount; ++i)
    {
      x[i] = i*tau; // x goes from -1 to 1
      I[i] = last.I[i]; // let's plot a quadratic function
      U[i] = last.U[i]; // let's plot a quadratic function
    }
    customPlot->addGraph();
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, U);
    customPlot->graph(1)->setData(x, I);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(0, T);
    //customPlot->yAxis->setRange(-500, 500);
    customPlot->yAxis->setRange(0, 15);
}

MainWindow::~MainWindow()
{
    delete ui;
}

