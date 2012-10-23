#ifndef WINDOW_H
#define WINDOW_H

#include <QVector>
#include <math.h>
#include <stdio.h>

enum WindowType{
    WINDOW_RECTANGULAR,
    WINDOW_HANN,
    WINDOW_HAMMING,
    WINDOW_TUKEY,
    WINDOW_COSINE,
    WINDOW_LANCZOS,
    WINDOW_TRIEANGULAR,
    WINDOW_GAUSSIAN,
    WINDOW_BARTLETT_HANN,
    WINDOW_BLACKMAN,
    WINDOW_KAISER,
    WINDOW_NUTTALL,
    WINDOW_BLACKMAN_HARRIS,
    WINDOW_BLACKMAN_NUTTALL,
    WINDOW_FLAT_TOP
};

class Window
{
public:
    Window(WindowType type, int N);
    ~Window();
    double get(int index);

private:
    QVector<double> *windowArray;
    void createWindow(WindowType type, int N);
    double J0(double x);
    long factorial(long i);
};

#endif // WINDOW_H
