#ifndef MYPLOT_H
#define MYPLOT_H

#include <QDialog>
#include "plot.h"

class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class QPolygon;
class Plot;
class QwtPlotCurve;
class QwtSymbol;

enum CurveType {
    SIGN,
    SPEC
};

class PlotWidget : public QWidget
{
Q_OBJECT
public:
    PlotWidget(QWidget *parent = 0);
    ~PlotWidget();
    void setParams(double *t, int N, QString xTitle, QString yTitle);
    void addCurve(const QString &title, double *window);
    void addCurve(const QString &title, double *window, CurveType type);
    void resetCurves();
    void setTitle(QString title);
    void changeLineOrLog(bool log);
signals:

private Q_SLOTS:
//    void moved(const QPoint &);
//    void selected(const QPolygon &);
//   void print();
   void exportDocument();
   void enableZoomMode(bool);

private:
    double *t;
    int N;
    Plot *signPlot;
    int curve_count;
    QColor *colors[15];

    QwtPlotZoomer *d_zoomer;
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};

#endif // MYPLOT_H
