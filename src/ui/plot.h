#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotMarker;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget *parent);
    ~Plot();
    void addCurve(QwtPlotCurve *curve, bool log);
    void resetCurves();
    void changeLineOrLog(bool log);

private:
    double *t;
    int N;
    bool axisLogScale;
    QList<QwtPlotCurve*> *curves;
    QList<QwtPlotCurve*> *logCurves;
    QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;
};

#endif // PLOT_H
