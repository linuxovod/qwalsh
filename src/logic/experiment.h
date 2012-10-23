#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QVector>
#include <QMap>
#include <QString>
#include "experimentparams.h"
#include "signal.h"
#include "window.h"
enum ExperimentType {
    RMS,
    RMS_PHASE_LIST,
    FIRST_HARMONIC_RMS,
    FREQUENCE_DETECTION,
    DOUBLE_HARMONIC,
    TRANSFORM_AND_SIGNAL
};
class Experiment
{
public:
    Experiment(ExperimentType expType, ExperimentParams *params);


    QVector<double>* getT();
    QVector<double>* getX2();
    QMap<QString, QVector<double>*>* getCurves();
    ExperimentParams* getParams();
    void execute();
    static QString curveView(ExperimentType expType,
                             WindowType winType,
                             TransformType transType);

private:
    QVector<double> *t, *x2;
    QMap<QString, QVector<double>*> *curves;
    QString *name;
    ExperimentType type;
    ExperimentParams *params;
    void executeRMS();
    void executeRMSDiffFreq();
    void executeRMSPhase();
    void executeSignalAndTransform();
    void executeFirstHarmonicRMS();
    void executeFirstHarmonicRMS1();
    void executeDoubleHarmonic();
    void executeFrequenceDetection();
    double rms(Signal* signal, TransformType type);
    double rmsFirstHarmonic(Signal* signal, double f, TransformType type);
};

#endif // EXPERIMENT_H
