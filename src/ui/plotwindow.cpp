#include "plotwindow.h"
#include "ui_plotwindow.h"
#include <stdio.h>

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    plot = new  PlotWidget();

    QLayout *layout = ui->plotLayout->layout();
    layout->addWidget(plot);
    paramsDialog = new SignalParamsDialog(this);
    QObject::connect(paramsDialog, SIGNAL(accepted()), this, SLOT(paramsChanged()));
    QObject::connect(ui->exportButton, SIGNAL(clicked()), plot, SLOT(exportDocument()));
    QObject::connect(ui->zoomPushButton, SIGNAL(toggled(bool)), plot, SLOT(enableZoomMode(bool)));
    on_logCheckBox_stateChanged(ui->logCheckBox->checkState());
}

PlotWindow::~PlotWindow()
{
    delete ui;
    delete plot;
}

void PlotWindow::setType(ExperimentType type)
{
    this->type = type;
    // В зависимости от типа меняется отображаемый заголовок
    switch(type){
    case RMS:
        plot->setTitle(QObject::trUtf8("Погрешность определения СКЗ"));
        break;
    case RMS_PHASE_LIST:
        plot->setTitle(QObject::trUtf8("Погрешность определения СКЗ при изменении начальной фазы"));
        break;
    case FIRST_HARMONIC_RMS:
        plot->setTitle(QObject::trUtf8("Влияние первой гармоники"));
        break;
    case FREQUENCE_DETECTION:
        plot->setTitle(QObject::trUtf8("Оценка частоты"));
        break;
    case DOUBLE_HARMONIC:
    case TRANSFORM_AND_SIGNAL:
        ui->logCheckBox->setChecked(false);
        plot->setTitle(QObject::trUtf8("Спектр Уолша"));
        plot2 = new PlotWidget();
        plot2->setTitle(QObject::trUtf8("Сигнал на начальной частоте"));
        QLayout *layout = ui->plotLayout->layout();
        layout->addWidget(plot2);
        break;

    }
}

void PlotWindow::on_paramsButton_clicked()
{
    paramsDialog->show();
}

void PlotWindow::on_calculateButton_clicked()
{
    if(ui->zoomPushButton->isChecked()){
        ui->zoomPushButton->toggle();
    }
    //Если частота изменилась - очистить графики
    if(paramsDialog->frequencyChanged()){
        plot->resetCurves();
        if(this->type == TRANSFORM_AND_SIGNAL){
            plot2->resetCurves();
        }
        paramsDialog->resetFreqChanged();
    }
    // Проверка на наличие выбранных окон
    if(!paramsDialog->getParams()->getWindowTypes()->isEmpty()){
        //Создание и выполнение эксперимента
        Experiment *exp = new Experiment(this->type, paramsDialog->getParams());
        exp->execute();
        // Передача данных для отрисовки
        switch(this->type){
        case RMS:
        case RMS_PHASE_LIST:
        {
            plot->setParams(exp->getT()->data(), exp->getParams()->getExCount(),
                            QObject::trUtf8("f/fd"), QObject::trUtf8("Погрешность, %"));
            QMap<QString, QVector<double>*> * curves = exp->getCurves();
            QMapIterator<QString, QVector<double>*> i(*curves);
            while (i.hasNext()) {
                i.next();
                plot->addCurve(i.key(), i.value()->data());
            }
        }
            break;
        case FIRST_HARMONIC_RMS:
        {
            plot->setParams(exp->getT()->data(), exp->getParams()->getExCount(),
                            QObject::trUtf8("f/fd"), QObject::trUtf8("СКЗ"));
            QMap<QString, QVector<double>*> * curves = exp->getCurves();
            QMapIterator<QString, QVector<double>*> i(*curves);
            while (i.hasNext()) {
                i.next();
                plot->addCurve(i.key(), i.value()->data());
            }
        }
            break;
        case DOUBLE_HARMONIC:
        case TRANSFORM_AND_SIGNAL:
        {
            plot2->setParams(exp->getT()->data(), exp->getParams()->getN(),
                            QObject::trUtf8("t"), QObject::trUtf8("Амплитуда"));
            plot->setParams(exp->getX2()->data(), exp->getParams()->getN()/2,
                            QObject::trUtf8("n"), QObject::trUtf8("Амплитуда"));
            QMap<QString, QVector<double>*> * curves = exp->getCurves();
            QMapIterator<QString, QVector<double>*> i(*curves);
            while (i.hasNext()) {
                i.next();
                if(i.key().at(0) == ' '){
                    plot2->addCurve(i.key(), i.value()->data());
                }else{
                    plot->addCurve(i.key(), i.value()->data(), SPEC);
                }
            }
        }
            break;

        case FREQUENCE_DETECTION:
        {
            plot->setParams(exp->getT()->data(), exp->getParams()->getExCount(),
                            QObject::trUtf8("f/fd"), QObject::trUtf8("Погрешность определения f, %"));
            QMap<QString, QVector<double>*> * curves = exp->getCurves();
            QMapIterator<QString, QVector<double>*> i(*curves);
            while (i.hasNext()) {
                i.next();
                plot->addCurve(i.key(), i.value()->data());
            }
        }
            break;
        }

        delete exp;
    }
}



void PlotWindow::paramsChanged()
{
    ExperimentParams *params = paramsDialog->getParams();
//    printf("N = %d, fd = %f, fmin = %f, fmax = %f \n", params->getN(), params->getFd(),
//           params->getFmin(), params->getFmax());
    QString paramInfo = tr("N = ");
    paramInfo.append(QString::number(params->getN())).append("\n");
    paramInfo.append("fd = ").append(QString::number(params->getFd())).append("\n");
    paramInfo.append("fmin = ").append(QString::number(params->getFmin())).append("\n");
     paramInfo.append("fmax = ").append(QString::number(params->getFmax())).append("\n");
    ui->label->setText(paramInfo);


}

void PlotWindow::on_cleanPushButton_clicked()
{
    plot->resetCurves();
    if(type == TRANSFORM_AND_SIGNAL){
        plot2->resetCurves();
    }
}

void PlotWindow::on_logCheckBox_stateChanged(int state)
{
    switch(state){
    case Qt::Checked:
        plot->changeLineOrLog(true);
        break;
    case Qt::Unchecked:
        plot->changeLineOrLog(false);
        break;
    }



}
