#include "experimentparams.h"

ExperimentParams::ExperimentParams()
{
    N = 0;
    windowTypes = new QVector<WindowType>();
    signalTypes = new QVector<SignalType>();
    windowsMap = NULL;
    fmin = 0.0;
    fmax = 0.0;
    fd = 0.0;
    useNoice = false;
    snr = 0.0;
    useFourierTransform = false;
}

ExperimentParams::~ExperimentParams()
{
    if(signalTypes != NULL){
        delete signalTypes;
    }
    if(windowTypes != NULL){
        delete windowTypes;
    }
    if(windowsMap != NULL){
        delete windowsMap;
    }
}

int ExperimentParams::getN()
{
    return N;
}

QVector<WindowType>* ExperimentParams::getWindowTypes()
{
    return windowTypes;
}

QMap<WindowType, Window *>* ExperimentParams::getWindows()
{
    if(windowsMap == NULL || windowsMap->size() == 0){
        windowsMap = new QMap<WindowType, Window *>();
        for(int i = 0; i<windowTypes->size();i++){
            windowsMap->insert(windowTypes->at(i), new Window(windowTypes->at(i), N));
        }
    }
    return windowsMap;
}

QVector<SignalType>* ExperimentParams::getSignalTypes()
{
    return signalTypes;
}

double ExperimentParams::getFmin()
{
    return fmin;
}

double ExperimentParams::getFmax()
{
    return fmax;
}

double ExperimentParams::getFd()
{
    return fd;
}

bool ExperimentParams::isUseNoice()
{
    return useNoice;
}

int ExperimentParams::getSnr()
{
    return snr;
}

int ExperimentParams::getBit()
{
    return bit;
}

int ExperimentParams::getD()
{
    return d;
}

int ExperimentParams::getExCount()
{
    return exCount;
}

bool ExperimentParams::isUseFourierTransform()
{
    return useFourierTransform;
}

bool ExperimentParams::isUseWalshTransform()
{
    return useWalshTransform;
}

void ExperimentParams::setN(int n)
{
    this->N = n;
}

void ExperimentParams::setWindowTypes(QVector<WindowType>* types)
{
    this->windowTypes = types;
}

void ExperimentParams::setSignalTypes(QVector<SignalType>* types)
{
    this->signalTypes = types;
}

void ExperimentParams::setFmin(double fmin)
{
    this->fmin = fmin;
}

void ExperimentParams::setFmax(double fmax)
{
    this->fmax = fmax;
}

void ExperimentParams::setFd(double fd)
{
    this->fd = fd;
}

void ExperimentParams::setUseNoice(bool useNoice)
{
    this->useNoice = useNoice;
}

void ExperimentParams::setBit(int bit)
{
    this->bit = bit;
}

void ExperimentParams::setSnr(int SNR)
{
    this->snr = SNR;
}

void ExperimentParams::setUseFourierTransform(bool useFourierTransform)
{
    this->useFourierTransform = useFourierTransform;
}

void ExperimentParams::setUseWalshTransform(bool useWalshTransform)
{
    this->useWalshTransform = useWalshTransform;
}

void ExperimentParams::setD(int interval)
{
    this->d = interval;
}

void ExperimentParams::setExCount(int count)
{
    this->exCount = count;
}

