#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include "plotwidget.h"
#include "signalparamsdialog.h"
#include "../logic/experiment.h"

class PlotWidget;
class MainWindow;

namespace Ui {
    class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow (QWidget *parent = 0);
    ~PlotWindow();

    void setType(ExperimentType type);

private:
    Ui::PlotWindow *ui;
    ExperimentType type;
    SignalParamsDialog *paramsDialog;
    PlotWidget *plot;
    PlotWidget *plot2;


private slots:
    void on_paramsButton_clicked();
    void on_calculateButton_clicked();
    void paramsChanged();
    void on_cleanPushButton_clicked();
    void on_logCheckBox_stateChanged(int state);
};

#endif // PLOTWINDOW_H
