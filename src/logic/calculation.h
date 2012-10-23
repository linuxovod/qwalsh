#ifndef CALCULATION_H
#define CALCULATION_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

class Calculation
{
public:
    Calculation();
    int N;
    double FD;
    double *window, *noise, *signal, *result;
    void createSignal(int type, double F, double ph);
private:
    void createSinSignal(double *sign, double F, double ph);
};

#endif // CALCULATION_H
