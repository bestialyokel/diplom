#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QtConcurrent>

#include "qcustomplot.h"
#include "encoder.h"
#include "machine.h"
#include "types.h"

#include <thread>
#include <atomic>
#include <limits>

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
    uStepSpinBox = findChild<QDoubleSpinBox*>("stepUSpinBox");
    uNullSpinBox = findChild<QDoubleSpinBox*>("nullUSpinBox");
    timeSpinBox = findChild<QDoubleSpinBox*>("timeSpinBox");

    loader = findChild<QLabel*>("loader");

    resistanceSpinBox->setSingleStep(0.1);
    inductanceSpinBox->setSingleStep(0.1);
    capacitySpinBox->setSingleStep(0.1);

    lenSpinBox->setSingleStep(0.1);
    accuracySpinBox->setSingleStep(0.01);
    minStepSpinBox->setSingleStep(0.0001);
    minStepSpinBox->setDecimals(4);
    maxStepSpinBox->setDecimals(4);
    maxStepSpinBox->setSingleStep(0.0001);
    freqSpinBox->setSingleStep(0.1);
    uStepSpinBox->setSingleStep(0.1);
    uNullSpinBox->setSingleStep(0.1);
    timeSpinBox->setSingleStep(1.0);
    freqSpinBox->setSingleStep(10.0);

    freqSpinBox->setMaximum(std::numeric_limits<double>::max());
    timeSpinBox->setMaximum(std::numeric_limits<double>::max());
    resistanceSpinBox->setMaximum(std::numeric_limits<double>::max());
    inductanceSpinBox->setMaximum(std::numeric_limits<double>::max());
    capacitySpinBox->setMaximum(std::numeric_limits<double>::max());
    lenSpinBox->setMaximum(1000.0);
    freqSpinBox->setMaximum(std::numeric_limits<double>::max());
    minStepSpinBox->setMaximum(1);
    maxStepSpinBox->setMaximum(1);
    accuracySpinBox->setMaximum(1);

}

void MainWindow::setDemonstrationValues() {
    resistanceSpinBox->setValue(10.0);
    inductanceSpinBox->setValue(250.0);
    capacitySpinBox->setValue(1.0);

    lenSpinBox->setValue(500.0);

    freqSpinBox->setValue(100.0);

    uStepSpinBox->setValue(0.5);
    uNullSpinBox->setValue(12.0);

    timeSpinBox->setValue(1000);

    accuracySpinBox->setValue(10.0);
    minStepSpinBox->setValue(0.04);
    maxStepSpinBox->setValue(0.04);

    coderComboBox->setCurrentIndex(1);
}

void MainWindow::setOscilatorStylePlot(QCustomPlot *plt) {
    //plt->setBackground( QColor(0,0,0) );

    QColor gridColor(255, 255, 255);
    QPen gridPen;
    gridPen.setWidthF(0.5);
    gridPen.setColor(gridColor);
    gridPen.setStyle(Qt::PenStyle::SolidLine);

    plt->xAxis->grid()->setPen(gridPen);
    plt->yAxis->grid()->setPen(gridPen);

    QCPGraph* graph = plt->addGraph();
    QColor graphColor(0, 0, 255);
    QPen graphPen;
    graphPen.setWidthF(2);
    graphPen.setColor(graphColor);

    graph->setPen(graphPen);

    plt->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //plt->axisRect()->setAutoMargins(QCP::msNone);
    //plt->axisRect()->setMargins(QMargins(-5,0,0,0));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initControls();

    setDemonstrationValues();

    stopped.store(true);

    setOscilatorStylePlot(inputPlot);
    setOscilatorStylePlot(outputPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLoading(bool state) {
    if (state) {
        if (loader->movie())
            return;
        //QMovie* movie = new QMovie(":/Resource/loading.gif");
        //loader->setMovie(movie);
        loader->setText("loading");
        //movie->start();
    } else {
        // loader->movie();
        loader->setText(nullptr);
        //loader->setMovie(nullptr);
    }
}

void MainWindow::on_startButton_clicked()
{
    bool running = !stopped.load();
    if (running)
        return;

    stopped.store(false);
    setLoading(true);
    clearPlot(inputPlot);
    clearPlot(outputPlot);

    auto trd = std::thread([&]() {
        double R = resistanceSpinBox->value();
        double L = inductanceSpinBox->value();
        double C = capacitySpinBox->value();
        double accuracy = accuracySpinBox->value();
        double length = lenSpinBox->value();
        double minStep = minStepSpinBox->value();
        double maxStep = maxStepSpinBox->value();
        double frequency = freqSpinBox->value();
        double uStep = uStepSpinBox->value();
        double uNull = uNullSpinBox->value();
        double T = timeSpinBox->value();


        double h = (minStep+maxStep)/2;

        //nrz, rz, manchester2
        size_t codeIndex = coderComboBox->currentIndex();

        double halfStep = 1/(2*h*frequency);

        RZEncoder rz(uNull, uStep, 100);

        vector<double> vals = rz.encode(bitsLineEdit->text().toStdString());

        int tCount = (T/h);

        Payload p = {
            .tau = h
        };
        RLC rlc = {
            .R = R,
            .L = L,
            .C = C,
        };

        p.I = vector<double>(tCount);
        p.U = vector<double>(tCount);

        std::fill(p.I.begin(), p.I.end(), 1.0);

        for (int i = 0; (i < vals.size()) && (i < tCount); i++) {
            p.U[i] = vals[i];
        }

        for (int i = vals.size(); i < tCount; i++) {
            p.U[i] = 12.0;
        }


        Machine m(length, rlc);

        m.appendPayload(p);
        optional<Payload> lastOpt = m.processNextPayloadStoppable(ref(stopped));

        if (!lastOpt.has_value()) {
            return;
        }

        Payload last = lastOpt.value();
        double maxU = 0;
        QVector<double> x(tCount), U(tCount), I(tCount), U_in(tCount);
        for (int i=0; i<tCount; ++i) {
          x[i] = i*h; // x goes from -1 to 1
          I[i] = last.I[i]; // let's plot a quadratic function
          U[i] = last.U[i]; // let's plot a quadratic function
          U_in[i] = p.U[i];

          if (U[i] > maxU)
              maxU = U[i];
        }

        outputPlot->graph(0)->setData(x, U);
        outputPlot->xAxis->setRange(0, T);
        outputPlot->yAxis->setRange(0, (maxU*1.1));
        outputPlot->replot();

        inputPlot->graph(0)->setData(x, U_in);
        inputPlot->xAxis->setRange(0, T);
        inputPlot->yAxis->setRange(0, (uNull+uStep)*1.1);
        inputPlot->replot();

        stopped.store(true);
        setLoading(false);
    });

    trd.detach();
}

void MainWindow::clearPlot(QCustomPlot *plt) {
    plt->graph(0)->data()->clear();
    plt->replot();
}

void MainWindow::on_stopButton_clicked()
{
    stopped.store(true);
    setLoading(false);
    clearPlot(inputPlot);
    clearPlot(outputPlot);
}
