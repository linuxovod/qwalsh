#include "calculation.h"

Calculation::Calculation()
{
}


void Calculation::createSignal(int type,  double F, double ph)
{
    if(signal != NULL){
        delete signal;
    }
    signal = new double[N];
    switch(type){
    case 1:
        createSinSignal(signal, F, ph);
        break;
    }

}

void Calculation::createSinSignal(double *sign, double F, double ph)
{
    if(sign != NULL){
        double Td = 1.0 / FD;
        double Ts = (double)(1.0 / F);
        double omega = 2.0*M_PI/Ts;
        double t1 = 0.0;
        for (int i = 0; i < N; i++)
        {
            double tt = (Td *(double)i);
            t1 = fmod(tt, Ts);
            sign[i]= sin(omega*t1 + ph*M_PI/180.0);
        }
    }
}

