#include "startwindow.h"
#include "ui_startwindow.h"

//Конструктор класса стартового окна.
StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    plotWindow = NULL;
}
// Деструктор класса стартового окна.
StartWindow::~StartWindow()
{
    delete ui;
    delete plotWindow;
}

//Обработка нажатия кнопки выбора эксперимента по оценке СКЗ
void StartWindow::on_rmsButton_clicked()
{
    showPlotWindow(RMS);
}

//Обработка нажатия кнопки выбора эксперимента по оценке СКЗ
// в зависимости от начальной фазы.
void StartWindow::on_rmsPhaseButton_clicked()
{
    showPlotWindow(RMS_PHASE_LIST);
}

void StartWindow::on_firstHarmonicButton_clicked()
{
    showPlotWindow(FIRST_HARMONIC_RMS);
}

void StartWindow::on_signalButton_clicked()
{
   showPlotWindow(TRANSFORM_AND_SIGNAL);
}

void StartWindow::on_frequencyDetectionButton_clicked()
{
    showPlotWindow(FREQUENCE_DETECTION);
}

void StartWindow::on_doubleHarmonicButton_clicked()
{
    showPlotWindow(DOUBLE_HARMONIC);
}

void StartWindow::showPlotWindow(ExperimentType type)
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(type);

    this->hide();
    plotWindow->showMaximized();
}
