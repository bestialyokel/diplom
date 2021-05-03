#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include "qcustomplot.h"

#include "types.h"
#include "machine.h"

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

    for (int i = 0; i < tCount; i++) {
        const double t = i*tau;
        if (t > 100) {
            p.U[i] = 12.5;
        } else {
            p.U[i] = 12;
        }

        if (t > 500) {
            p.I[i] = 0.5;
        } else {
            p.I[i] = 1;
        }
    }

    Machine m(20, rlc);

    m.appendPayload(p);
    m.init(p.U[0], p.I[0]);

    Payload last = m.processNextPayload();

    for (auto x : last.U) {
        cout << x << endl;
    }

    QCustomPlot* customPlot = findChild<QCustomPlot*>("plot");

    QVector<double> x(tCount), y(tCount);
    for (int i=0; i<tCount; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = last.U[i]; // let's plot a quadratic function
    }
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(0, 1000);
    customPlot->yAxis->setRange(0, 15);


}

MainWindow::~MainWindow()
{
    delete ui;
}

