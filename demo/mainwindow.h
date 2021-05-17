#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <atomic>
#include <QVector>

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

private:
    QCustomPlot* inputPlot;
    QCustomPlot* outputPlot;

    QDoubleSpinBox* resistanceSpinBox;
    QDoubleSpinBox* inductanceSpinBox;
    QDoubleSpinBox* capacitySpinBox;
    QDoubleSpinBox* accuracySpinBox;
    QDoubleSpinBox* minStepSpinBox;
    QDoubleSpinBox* maxStepSpinBox;
    QDoubleSpinBox* lenSpinBox;
    QDoubleSpinBox* freqSpinBox;

    QDoubleSpinBox* uStepSpinBox;
    QDoubleSpinBox* uNullSpinBox;

    QDoubleSpinBox* timeSpinBox;

    QLineEdit* bitsLineEdit;

    QPushButton* startButton;
    QPushButton* stopButton;

    QComboBox* coderComboBox;
    Ui::MainWindow *ui;

    QLabel* loader;
    
    atomic_bool stopped;

    void initControls();
    void setDemonstrationValues();
    void setOscilatorStylePlot(QCustomPlot* plt);

    void clearPlot(QCustomPlot* plt);

    void setLoading(bool);

    void showResult(QCustomPlot* plt, QVector<double> x, QVector<double> y);
};
#endif // MAINWINDOW_H
