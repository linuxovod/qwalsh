#ifndef SIGNAL_H
#define SIGNAL_H

#include <QVector>
#include <QString>
#include <QTime>
#include <QDebug>
#include <math.h>
#include <fftw3.h>
#include "signalparams.h"


#define SIGNAL_TYPE_SINUS = 1

enum TransformType{
    NONE,WALSH, FOURIER
};

class Signal
{
public:
    Signal(SignalParams* params);
    ~Signal();

    QVector<double>* signal();
    QVector<double>* walshTransform();
    QVector<double>* fourierTransform();

    double power(TransformType type);



private:
    QVector<double> *signalArray, *walshTransformArray, *fourierTransformArray;
    SignalParams* params;
    bool signalCreated, walshCreated, fourierCreated;


    void createSignal();
    void createSinSignal();
    void createBitSinSignal();
    void createWalshTransform();
    void createFourierTransforn();
    double walshPower();
    void FWT(QVector<double>* in, QVector<double>* out, int n, int dir);
    void FFT(double *in, double *out, int n);
};

#endif // SIGNAL_H
