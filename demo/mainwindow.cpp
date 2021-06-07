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
    accuracySpinBox = findChild<QSpinBox*>("accuracySpinBox");
    stepSpinBox = findChild<QDoubleSpinBox*>("stepSpinBox");
    freqSpinBox = findChild<QDoubleSpinBox*>("freqSpinBox");
    bitsLineEdit = findChild<QLineEdit*>("bitsLineEdit");
    startButton = findChild<QPushButton*>("startButton");
    stopButton = findChild<QPushButton*>("stopButton");
    coderComboBox = findChild<QComboBox*>("codeComboBox");
    uStepSpinBox = findChild<QDoubleSpinBox*>("stepUSpinBox");
    uNullSpinBox = findChild<QDoubleSpinBox*>("nullUSpinBox");
    timeSpinBox = findChild<QDoubleSpinBox*>("timeSpinBox");

    bar = findChild<QProgressBar*>("progressBar");
}

void MainWindow::setControlsProps() {
    resistanceSpinBox->setSingleStep(0.1);
    inductanceSpinBox->setSingleStep(0.1);
    capacitySpinBox->setSingleStep(0.1);

    stepSpinBox->setSingleStep(0.0001);
    stepSpinBox->setDecimals(4);
    freqSpinBox->setSingleStep(0.1);
    uStepSpinBox->setSingleStep(0.1);
    uNullSpinBox->setSingleStep(0.1);
    timeSpinBox->setSingleStep(1.0);
    freqSpinBox->setSingleStep(0.01);

    freqSpinBox->setMaximum(std::numeric_limits<double>::max());
    timeSpinBox->setMaximum(std::numeric_limits<double>::max());
    resistanceSpinBox->setMaximum(std::numeric_limits<double>::max());
    inductanceSpinBox->setMaximum(std::numeric_limits<double>::max());
    accuracySpinBox->setMaximum(std::numeric_limits<int>::max());
    capacitySpinBox->setMaximum(std::numeric_limits<double>::max());
    lenSpinBox->setMaximum(1000.0);
    lenSpinBox->setMinimum(1);

    bar->setMaximum(100);
    bar->setVisible(false);

    freqSpinBox->setMaximum(std::numeric_limits<double>::max());
    stepSpinBox->setMaximum(1);
}

void MainWindow::setDemonstrationValues() {
    resistanceSpinBox->setValue(1.0);
    inductanceSpinBox->setValue(25.0);
    capacitySpinBox->setValue(0.1);

    lenSpinBox->setValue(10);

    accuracySpinBox->setValue(500);

    freqSpinBox->setValue(0.25);

    uStepSpinBox->setValue(0.5);
    uNullSpinBox->setValue(12.0);

    timeSpinBox->setValue(1000);

    stepSpinBox->setValue(0.04);

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

    plt->xAxis->setLabel("t");
    plt->yAxis->setLabel("V");

    QCPGraph* graph = plt->addGraph();

    QColor graphColor(0, 0, 255);
    QPen graphPen;
    graphPen.setWidthF(2);
    graphPen.setColor(graphColor);

    graph->setPen(graphPen);

    plt->graph(0)->rescaleAxes(true);


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

    setControlsProps();
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
        bar->setVisible(true);
    } else {
        bar->setVisible(false);
    }
    bar->setValue(0);
}

void MainWindow::setState(int p) {
    bar->setValue(p);
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
        int accuracy = accuracySpinBox->value();
        double length = lenSpinBox->value();
        double step = stepSpinBox->value();
        double frequency = freqSpinBox->value();
        double uStep = uStepSpinBox->value();
        double uNull = uNullSpinBox->value();
        double T = timeSpinBox->value();

        //затычка(или нормально) - число точек на половину такт(?)
        double val = 1/(frequency*step);

        Encoder* enc;
        //mlt-3, rz, manchester2
        size_t codeIndex = coderComboBox->currentIndex();

        switch (codeIndex) {
            case 0:
                enc = new MLT3Encoder(uNull, uStep, 2*val);
            break;
            case 1:
                enc = new RZEncoder(uNull, uStep, val);
            break;
            case 2:
                enc = new ManchesterEncoder(uNull, uStep, val);
            break;
            //to supress warnings?
            default:
                enc = new RZEncoder(uNull, uStep, val);
        }

        vector<double> vals = enc->encode(bitsLineEdit->text().toStdString());

        delete enc;

        int tCount = (T/step);

        Payload p = {
            .tau = step,
            .I = vector<double>(tCount),
            .U = vector<double>(tCount),

        };
        //std::fill(p.I.begin(), p.I.end(), 1);
        std::fill(p.I.begin(), p.I.end(), 1);
        for (int i = 0; (i < vals.size()) && (i < tCount); i++) {
            p.U[i] = vals[i];
        }

        for (int i = vals.size(); i < tCount; i++) {
            p.U[i] = uNull;
        }

        RLC rlc = {
            .R = R*length,
            .L = L*length,
            .C = C*length,
        };

        Machine m(accuracy, rlc);
        auto obj = this;
        m.appendPayload(p);
        optional<Payload> lastOpt = m.processNextPayloadStoppable(ref(stopped), [=](int percent) {
            QMetaObject::invokeMethod(obj, [=]() {
                bar->setValue(percent);
            });
        });

        if (!lastOpt.has_value()) {
            return;
        }

        Payload last = lastOpt.value();
        double maxU = 0;
        double minU = std::numeric_limits<double>::max();
        QVector<double> x(tCount), U(tCount), I(tCount), U_in(tCount);
        for (int i=0; i<tCount; ++i) {
          x[i] = i*step; // x goes from -1 to 1
          U[i] = last.U[i]; // let's plot a quadratic function
          U_in[i] = p.U[i];

          if (U[i] > maxU)
              maxU = U[i];
          if (U[i] < minU)
              minU = U[i];
        }

        outputPlot->graph(0)->setData(x, U);
        outputPlot->xAxis->setRange(0, T);
        outputPlot->yAxis->setRange(minU*0.99, maxU*1.01);
        outputPlot->replot();

        inputPlot->graph(0)->setData(x, U_in);
        inputPlot->xAxis->setRange(0, T);
        inputPlot->yAxis->setRange((uNull-uStep)*0.99 , (uNull+uStep)*1.01);
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

void MainWindow::on_freqSpinBox_valueChanged(double newValue)
{
    double step = stepSpinBox->value();
    double maxFreq = 1/step;
    if (newValue > maxFreq) {
        freqSpinBox->setValue(maxFreq);
    }
}

