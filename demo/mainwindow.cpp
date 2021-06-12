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

    setOscilatorStylePlot(ui->inputPlot);
    setOscilatorStylePlot(ui->outputPlot);

    dispatchAppState(true, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLoading(bool state) {
    if (state) {
        ui->progressBar->setVisible(true);
    } else {
        ui->progressBar->setVisible(false);
    }
    ui->progressBar->setValue(0);
}

void MainWindow::setState(int p) {
    ui->progressBar->setValue(p);
}

Payload MainWindow::getPayload(Encoder* enc) {
    double T = ui->timeSpinBox->value();
    double step = ui->stepSpinBox->value();
    double uNull = ui->nullUSpinBox->value();
    double R = ui->rSpinBox->value();
    double len = ui->lenSpinBox->value();
    string bits = ui->bitsLineEdit->text().toStdString();

    size_t tCount = (T/step);

    Payload p = {
        .tau = step,
        .I = vector<double>(tCount),
        .U = vector<double>(tCount),
    };

    vector<double> vals = enc->encode(bits);

    double fullR = (R*len);
    //std::fill(p.I.begin(), p.I.end(), I_out);
    for (size_t i = 0; (i < vals.size()) && (i < tCount); i++) {
        p.U[i] = vals[i];
    }

    //padding
    for (size_t i = vals.size(); i < tCount; i++) {
        p.U[i] = uNull;
    }

    for (size_t i = 0; i < p.U.size(); i++) {
        //нагрузка
        p.I[i] = 1;
    }

    return std::move(p);
}

RLC MainWindow::getRLC() {
    double R = ui->rSpinBox->value();
    double L = ui->lSpinBox->value();
    double C = ui->cSpinBox->value();
    double length = ui->lenSpinBox->value();
    RLC rlc = {
        .R = R*length,
        .L = L*length,
        .C = C*length,
    };
    return rlc;
}

Encoder* MainWindow::getEncoder() {
    size_t codeIndex = ui->codeComboBox->currentIndex();
    double frequency = ui->freqSpinBox->value();
    double uStep = ui->stepUSpinBox->value();
    double uNull = ui->nullUSpinBox->value();
    double step = ui->stepSpinBox->value();

    //число точек на половину такт(?)
    double val = 1/(frequency*step);

    Encoder* enc;
    //mlt-3, rz, manchester

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

    return enc;
}

void MainWindow::dispatchAppState(bool stopped, bool clearPlots) {

    this->stopped.store(stopped);
    setLoading(!stopped);

    ui->progressBar->setVisible(!stopped);

    ui->status->setVisible(!stopped);

    if (stopped && clearPlots) {
        clearPlot(ui->inputPlot);
        clearPlot(ui->outputPlot);
    }
}

void MainWindow::setLoadingPercent(size_t percent) {
    ui->progressBar->setValue(percent);
}

void MainWindow::invokeOnWindowThread(function<void()> fun) {
    QMetaObject::invokeMethod(this, fun);
}

void MainWindow::displayVoltageResult(QCustomPlot *plt, QVector<double> time, vector<double> voltage) {
    double maxU = 0;
    double minU = std::numeric_limits<double>::max();
    double T = ui->timeSpinBox->value();

    QVector<double> V(voltage.size());

    for (size_t i = 0; i < voltage.size(); ++i) {
       V[i] = voltage[i];
      if (voltage[i] > maxU)
          maxU = voltage[i];
      if (voltage[i] < minU)
          minU = voltage[i];
    }
    plt->graph(0)->setData(time, V);
    plt->xAxis->setRange(0, T);
    plt->yAxis->setRange(minU*0.99, maxU*1.01);
    plt->replot();
}

void MainWindow::on_startButton_clicked()
{
    bool running = !stopped.load();
    if (running)
        return;

    dispatchAppState(false, true);

    auto trd = std::thread([&]() {
        Encoder* enc = getEncoder();
        Payload payload = getPayload(enc);
        RLC rlc = getRLC();
        size_t circuitCount = ui->accuracySpinBox->value();
        Machine machine(circuitCount, rlc);

        double T = ui->timeSpinBox->value();
        double step = ui->stepSpinBox->value();

        optional<Payload> lastOpt = machine.processNextPayloadStoppable(payload, ref(stopped), [=](size_t percent) {
            invokeOnWindowThread([=]() {
                if (percent == 0) {
                    ui->status->setText("Поиск начальных условий..");
                } else if (percent == 1) {
                    ui->status->setText("Моделирование..");
                }
                setLoadingPercent(percent);
            });
        });

        if (!lastOpt.has_value()) {
            return;
        }

        Payload last = lastOpt.value();

        QVector<double> time(T/step);

        for (size_t i = 0; i < time.size(); i++) {
            time[i] = last.tau * i;
        }

        invokeOnWindowThread([=]() {
            displayVoltageResult(ui->inputPlot, time, payload.U);
            displayVoltageResult(ui->outputPlot, time, last.U);
            dispatchAppState(true, false);
        });
    });

    trd.detach();
}


void MainWindow::clearPlot(QCustomPlot *plt) {
    plt->graph(0)->data()->clear();
    plt->replot();
}

void MainWindow::on_stopButton_clicked()
{
    dispatchAppState(true, true);
}

void MainWindow::on_freqSpinBox_valueChanged(double newValue)
{
    double step = ui->stepSpinBox->value();
    double maxFreq = 1/step;
    if (newValue > maxFreq) {
        ui->freqSpinBox->setValue(maxFreq);
    }
}


void MainWindow::on_bitsLineEdit_textChanged(const QString &arg1)
{
    if (arg1.size() == 0) {
        return;
    }
    auto last = arg1[arg1.size() - 1];
    auto newStr = arg1;
    if (last != "0" && last != "1") {
        ui->bitsLineEdit->setText( newStr.remove(newStr.size() - 1, 1) );
    }
}

