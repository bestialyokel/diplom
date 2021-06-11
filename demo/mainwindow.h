#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <atomic>
#include <QVector>
#include "encoder.h"
#include "types.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_freqSpinBox_valueChanged(double arg1);

    void on_bitsLineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    atomic_bool stopped;

    void setOscilatorStylePlot(QCustomPlot* plt);

    void clearPlot(QCustomPlot* plt);

    void setLoading(bool);

    void showResult(QCustomPlot* plt, QVector<double> x, QVector<double> y);

    Payload getPayload(Encoder* enc);

    void dispatchAppState(bool stopped, bool clearPlots);

    void setLoadingPercent(size_t percent);

    void invokeOnWindowThread(function<void()> fun);

    void displayVoltageResult(QCustomPlot* plt, QVector<double> time, vector<double> voltage);

    RLC getRLC();

    Encoder* getEncoder();

    void setState(int);
};
#endif // MAINWINDOW_H
