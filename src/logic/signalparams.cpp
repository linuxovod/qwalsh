#include "signalparams.h"

SignalParams::SignalParams()
{
    this->N = 1024;
    this->amplitude = 1.f;
    this->phase = 0.f;
    this->signalSamplingFreqRatio = 0.f;
    this->signalType = SIGNAL_SINUS;
    this->windowType = WINDOW_RECTANGULAR;
    this->bit = 0;
    this->snr = 0;
}

void SignalParams::setSignalType(SignalType type)
{
    this->signalType = type;
}

int SignalParams::getSignalType()
{
    return signalType;
}

void SignalParams::setWindowType(WindowType type)
{
    this->windowType = type;
}

int SignalParams::getWindowType()
{
    return windowType;
}

void SignalParams::setWindow(Window *w)
{
    this->window = w;
}

Window *SignalParams::getWindow()
{
    return window;
}

void SignalParams::setN(int n)
{
    this->N = n;
}

int SignalParams::getN()
{
    return N;
}

void SignalParams::setAplitude(double amplitude)
{
    this->amplitude = amplitude;
}

double SignalParams::getAmplitude()
{
    return amplitude;
}

void SignalParams::setPhase(double phase)
{
    this->phase = phase;
}

double SignalParams::getPhase()
{
    return phase;
}

void SignalParams::setSignalSamplingFreqRatio(double SSFR)
{
    this->signalSamplingFreqRatio = SSFR;
}

double SignalParams::getSignalSamplingFreqRatio()
{
    return signalSamplingFreqRatio;
}

void SignalParams::setSNR(int snr)
{
    this->snr = snr;
}

int SignalParams::getSNR()
{
    return snr;
}

void SignalParams::setBit(int bit)
{
    this->bit = bit;
}

int SignalParams::getBit()
{
    return bit;
}
