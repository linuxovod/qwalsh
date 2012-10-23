#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qmath.h>
#include "plot.h"

Plot::Plot(QWidget *parent):
    QwtPlot(parent)
{
    setAutoReplot(false);
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(this);

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::ReadOnlyItem);
    insertLegend(legend, QwtPlot::RightLegend);

    curves = new QList<QwtPlotCurve*>();
    logCurves = new QList<QwtPlotCurve*>();
    setAutoReplot(true);
    setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
   // enableAxis(QwtPlot::yRight);
    axisLogScale = false;
}

Plot::~Plot()
{
    delete curves;
}

void Plot::addCurve(QwtPlotCurve *curve, bool log)
{
    if(!log){
        curves->append(curve);
    }else{
        logCurves->append(curve);
    }
    if(axisLogScale == log){
        curve->attach(this);
    }

}

void Plot::resetCurves()
{
   for(int i = 0; i < curves->length(); i++){
       curves->at(i)->detach();
       delete curves->at(i);
   }
   for(int i = 0; i < logCurves->length(); i++){
       logCurves->at(i)->detach();
       delete logCurves->at(i);
   }
   curves->clear();
   logCurves->clear();
   replot();
}

void Plot::changeLineOrLog(bool log)
{
    axisLogScale = log;
    if(log){
        for(int i = 0; i < curves->length(); i++){
            curves->at(i)->detach();
        }
        for(int i = 0; i < logCurves->length(); i++){
            logCurves->at(i)->attach(this);
        }
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);
    }else{
        for(int i = 0; i < logCurves->length(); i++){
            logCurves->at(i)->detach();
        }
        for(int i = 0; i < curves->length(); i++){
            curves->at(i)->attach(this);
        }
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
    }
     replot();
}


