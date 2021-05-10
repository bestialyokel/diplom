#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include "qcustomplot.h"

#include "types.h"
#include "machine.h"

void MainWindow::initControls() {
    inputPlot = findChild<QCustomPlot*>("inputPlot");
    outputPlot = findChild<QCustomPlot*>("outputPlot");

    resistanceSpinBox = findChild<QDoubleSpinBox*>("rSpinBox");
    inductanceSpinBox = findChild<QDoubleSpinBox*>("lSpinBox");
    capacitySpinBox = findChild<QDoubleSpinBox*>("cSpinBox");
    lenSpinBox = findChild<QDoubleSpinBox*>("lenSpinBox");
    accuracySpinBox = findChild<QDoubleSpinBox*>("accuracySpinBox");
    minStepSpinBox = findChild<QDoubleSpinBox*>("minStepSpinBox");
    maxStepSpinBox = findChild<QDoubleSpinBox*>("maxStepSpinBox");
    freqSpinBox = findChild<QDoubleSpinBox*>("freqSpinBox");

    bitsLineEdit = findChild<QLineEdit*>("bitsLineEdit");

    startButton = findChild<QPushButton*>("startButton");
    stopButton = findChild<QPushButton*>("stopButton");

    coderComboBox = findChild<QComboBox*>("codeComboBox");
}


void MainWindow::setOscilatorStylePlot(QCustomPlot *plt) {
    plt->setBackground( QColor(0,0,0) );

    QColor color(0, 255, 0);
    QPen pen;
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle::DotLine);

    plt->xAxis->grid()->setPen(pen);
    plt->yAxis->grid()->setPen(pen);


    plt->axisRect()->setAutoMargins(QCP::msNone);
    plt->axisRect()->setMargins(QMargins(-5,0,0,0));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initControls();

    setOscilatorStylePlot(inputPlot);
    setOscilatorStylePlot(outputPlot);

    /*
    double tau = step;
    if (tau == 0) {
        tau = 0.04;
    }

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

    if (N == 0) {
        N=500;
    }
    Machine m(N, rlc);

    m.appendPayload(p);

    Payload last = m.processNextPayload();

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

    customPlot->xAxis->setRange(0, T-1);
    customPlot->yAxis->setRange(0,  15);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

