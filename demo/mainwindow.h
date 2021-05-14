#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

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

    QLineEdit* bitsLineEdit;

    QPushButton* startButton;
    QPushButton* stopButton;

    QComboBox* coderComboBox;
    Ui::MainWindow *ui;
    
    void initControls();
    void setOscilatorStylePlot(QCustomPlot* plt);
};
#endif // MAINWINDOW_H
