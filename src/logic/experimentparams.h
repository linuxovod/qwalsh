#ifndef EXPERIMENTPARAMS_H
#define EXPERIMENTPARAMS_H
#include <QVector>
#include <QMap>
#include "signalparams.h"
#include "window.h"

class ExperimentParams
{
public:
    ExperimentParams();
    ~ExperimentParams();
    int getN();
    QVector<WindowType>* getWindowTypes();
    QMap<WindowType, Window*>* getWindows();
    QVector<SignalType>* getSignalTypes();
    double getFmin();
    double getFmax();
    double getFd();
    bool isUseNoice();
    int getSnr();
    int getBit();
    int getD();
    int getExCount();
    bool isUseFourierTransform();
    bool isUseWalshTransform();
    void setN(int n);
    void setWindowTypes(QVector<WindowType>* types);
    void setSignalTypes(QVector<SignalType>* types);
    void setFmin(double fmin);
    void setFmax(double fmax);
    void setFd(double fd);
    void setUseNoice(bool useNoice);
    void setBit(int bit);
    void setSnr(int SNR);
    void setUseFourierTransform(bool useFourierTransform);
    void setUseWalshTransform(bool useWalshTransform);
    void setD(int interval);
    void setExCount(int count);
private:
    int exCount;
    int N;
    QVector<WindowType> *windowTypes;
    QVector<SignalType> *signalTypes;
    QMap<WindowType, Window*>* windowsMap;
    double fmin, fmax, fd;
    bool useNoice;
    int snr;
    int bit;
    bool useFourierTransform;
    bool useWalshTransform;
    int d;

};

#endif // EXPERIMENTPARAMS_H
