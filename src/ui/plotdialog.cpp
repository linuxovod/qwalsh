#include <QVBoxLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qpen.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qprinter.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qfiledialog.h>
#include <qimagewriter.h>
#include <qprintdialog.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <QRectF>
#include "plotdialog.h"

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
        QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier);
        setMousePattern(QwtEventPattern::MouseSelect3,
            Qt::RightButton);
    }
};

PlotDialog::PlotDialog(QWidget *parent) :
    QWidget(parent)
{
    signPlot = new Plot(this);
    signPlot->setTitle("Plot");
    curve_count = 0;
    /*for(int i = 0; i<=14;i++)
        colors[i] = new QColor(Qt::black); */
    colors[0] = new QColor(Qt::blue);
    colors[1] = new QColor(Qt::darkGreen);
    colors[2] = new QColor(Qt::red);
    colors[3] = new QColor(Qt::magenta);
    colors[4] = new QColor(Qt::darkCyan);
    colors[5] = new QColor(Qt::black);
    colors[6] = new QColor(Qt::darkBlue);
    colors[7] = new QColor(255,150,0,255);
    colors[8] = new QColor(Qt::green);
    colors[9] = new QColor(Qt::darkRed);
    colors[10] = new QColor(Qt::darkMagenta);
    colors[11] = new QColor(Qt::darkYellow);
    colors[12] = new QColor(Qt::yellow);
    colors[13] = new QColor(20,200,100,255);
    colors[14] = new QColor(200,25,255,255);

    d_zoomer = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        signPlot->canvas());
    d_zoomer->setRubberBand(QwtPicker::RectRubberBand);
    d_zoomer->setRubberBandPen(QColor(Qt::green));
    d_zoomer->setTrackerMode(QwtPicker::ActiveOnly);
    d_zoomer->setTrackerPen(QColor(Qt::white));
    d_zoomer->setEnabled(false);

    d_panner = new QwtPlotPanner(signPlot->canvas());
    d_panner->setMouseButton(Qt::MidButton);

    d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
        signPlot->canvas());
    d_picker->setStateMachine(new QwtPickerDragPointMachine());
    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::black));


    QHBoxLayout *widgetLayout = new QHBoxLayout;
    widgetLayout->addWidget(signPlot);
    setLayout(widgetLayout);

}

PlotDialog::~PlotDialog()
{
    delete signPlot;
}


void PlotDialog::setParams(double *x, int NUM, QString xTitle, QString yTitle){
    t= x;
    N = NUM;

    signPlot->setAxisTitle(QwtPlot::xBottom, xTitle);
    signPlot->setAxisTitle(QwtPlot::yLeft, yTitle);

}

void PlotDialog::addCurve(const QString &title, double *window){
    addCurve(title, window, SIGN);
}

void PlotDialog::addCurve(const QString &title, double *window, CurveType type){
    QwtPlotCurve *c = new QwtPlotCurve(title);
    QPen sPen;
    if(curve_count<=14){
        sPen = QPen(*colors[curve_count]);
        sPen.setWidth(2);
    }else{
       return;
    }
    c->setRenderHint(QwtPlotItem::RenderAntialiased);
    c->setPen(sPen);
    c->setSamples(t,window,N);
    c->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    switch(type){
        case SIGN:
        c->setStyle(QwtPlotCurve::Lines);
        break;
        case SPEC:
        c->setStyle(QwtPlotCurve::Sticks);
        break;
    }

    signPlot->addCurve(c, false);

    QwtPlotCurve *c1 = new QwtPlotCurve(title);
    c1->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1->setPen(sPen);
    double * window1 = new double[N];
    for(int i = 0; i < N; i++){
        if(window[i]>=0){
            window1[i]= window[i];
        }else{
            window1[i]=-window[i];
        }
    }
    c1->setSamples(t,window1,N);
    c1->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
    signPlot->addCurve(c1, true);

    curve_count++;
    signPlot->replot();
    d_zoomer->setZoomBase();
}

void PlotDialog::resetCurves(){
    curve_count = 0;
    d_zoomer->zoom(0);
    d_zoomer->setZoomBase();
    signPlot->resetCurves();


}

#if 0
void PlotDialog::print()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = signPlot->title().text();
    if ( !docName.isEmpty() )
    {
        docName.replace (QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
        printer.setDocName (docName);
    }

    printer.setCreator("Bode example");
    printer.setOrientation(QPrinter::Landscape);

    QPrintDialog dialog(&printer);
    if ( dialog.exec() )
    {
        QwtPlotRenderer renderer;

        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
            renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales);
        }

        renderer.renderTo(signPlot, printer);
    }
}
#endif

void PlotDialog::exportDocument()
{
#ifndef QT_NO_PRINTER
    QString fileName = "graph.pdf";
#else
    QString fileName = "graph.png";
#endif

#ifndef QT_NO_FILEDIALOG
    const QList<QByteArray> imageFormats =
        QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += "PDF Documents (*.pdf)";
#ifndef QWT_NO_SVG
    filter += "SVG Documents (*.svg)";
#endif
    filter += "Postscript Documents (*.ps)";

    if ( imageFormats.size() > 0 )
    {
        QString imageFilter("Images (");
        for ( int i = 0; i < imageFormats.size(); i++ )
        {
            if ( i > 0 )
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", fileName,
        filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);
#endif

    if ( !fileName.isEmpty() )
    {
        QwtPlotRenderer renderer;
#if 0
        // flags to make the document look like the widget
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepMargins, true);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames, true);
#endif
        renderer.renderDocument(signPlot, fileName, QSizeF(300, 100), 85);
    }
}

void PlotDialog::enableZoomMode(bool on)
{
    d_panner->setEnabled(on);

    d_zoomer->setEnabled(on);
    d_zoomer->zoom(0);

    d_picker->setEnabled(!on);
}

void PlotDialog::setTitle(QString title)
{
    signPlot->setTitle(title);
}

void PlotDialog::changeLineOrLog(bool log)
{
    d_zoomer->zoom(0);
    signPlot->changeLineOrLog(log);

    d_zoomer->setZoomBase();
}
#if 0
void PlotDialog::showInfo(QString text)
{
    if ( text == QString::null )
    {
        if ( d_picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }

#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage(text);
#endif
}

void MainWindow::moved(const QPoint &pos)
{
    QString info;
    info.sprintf("Freq=%g, Ampl=%g, Phase=%g",
        d_plot->invTransform(QwtPlot::xBottom, pos.x()),
        d_plot->invTransform(QwtPlot::yLeft, pos.y()),
        d_plot->invTransform(QwtPlot::yRight, pos.y())
    );
    showInfo(info);
}

void MainWindow::selected(const QPolygon &)
{
    showInfo();
}
#endif
