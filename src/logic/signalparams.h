#ifndef SIGNALPARAMS_H
#define SIGNALPARAMS_H

#include "window.h"

enum SignalType{
    SIGNAL_SINUS,
    SIGNAL_BIT_SINUS
};


class SignalParams
{
public:
    SignalParams();

    void setSignalType(SignalType type);
    int getSignalType();
    void setWindowType(WindowType type);
    int getWindowType();

    void setWindow(Window *w);
    Window* getWindow();


    void setN(int n);
    int getN();
    void setAplitude(double amplitude);
    double getAmplitude();
    void setPhase(double phase);
    double getPhase();
    void setSignalSamplingFreqRatio(double SSFR);
    double getSignalSamplingFreqRatio();
    void setSNR(int snr);
    int getSNR();
    void setBit(int bit);
    int getBit();
private:
    SignalType signalType;
    WindowType windowType;
    Window *window;
    int N;
    double amplitude, phase;
    double signalSamplingFreqRatio;
    int bit, snr;
};

#endif // SIGNALPARAMS_H
