#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "plotwindow.h"

class PlotWindow;

namespace Ui {
    class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private:
    Ui::StartWindow *ui;
    PlotWindow *plotWindow;
private slots:
    void on_rmsButton_clicked();
    void on_rmsPhaseButton_clicked();
    void on_firstHarmonicButton_clicked();
    void on_signalButton_clicked();
    void on_frequencyDetectionButton_clicked();
    void on_doubleHarmonicButton_clicked();
    void showPlotWindow(ExperimentType type);
};

#endif // STARTWINDOW_H
