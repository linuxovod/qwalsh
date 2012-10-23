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
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(RMS);

    this->hide();
    plotWindow->showMaximized();
}

//Обработка нажатия кнопки выбора эксперимента по оценке СКЗ
// в зависимости от начальной фазы.
void StartWindow::on_rmsPhaseButton_clicked()
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(RMS_PHASE_LIST);

    this->hide();
    plotWindow->showMaximized();
}

void StartWindow::on_firstHarmonicButton_clicked()
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(FIRST_HARMONIC_RMS);

    this->hide();
    plotWindow->showMaximized();
}

void StartWindow::on_signalButton_clicked()
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(TRANSFORM_AND_SIGNAL);

    this->hide();
    plotWindow->showMaximized();
}

void StartWindow::on_frequencyDetectionButton_clicked()
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(FREQUENCE_DETECTION);

    this->hide();
    plotWindow->showMaximized();
}

void StartWindow::on_doubleHarmonicButton_clicked()
{
    if(plotWindow == NULL){
       plotWindow = new PlotWindow(this);
    }

    plotWindow->setType(DOUBLE_HARMONIC);

    this->hide();
    plotWindow->showMaximized();
}
