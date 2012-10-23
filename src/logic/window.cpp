#include "window.h"

Window::Window(WindowType type, int N)
{
    this->windowArray = new QVector<double>(N);
    createWindow(type, N);
}

Window::~Window()
{
    if(windowArray != NULL){
        delete windowArray;
    }
}

double Window::get(int index)
{
    if((index >=0) && (index < (windowArray->size()))){
        return windowArray->at(index);
    }
    return 0;
}

void Window::createWindow(WindowType type, int N)
{
    double alph = 0.0;
    int length = N;
    switch(type)
    {
    case WINDOW_RECTANGULAR:
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = 1.0;
        }
        break;
    }
    case WINDOW_HANN: //окно Ханна
    {
        alph = 0.5;
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (alph+(1.0-alph)*cos(2.0*M_PI*i/length+M_PI));
        }
        break;
    }
    case WINDOW_HAMMING: // окно Хемминга
    {

        alph = 0.54; //уточник коэффицент
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (alph+(1.0-alph)*cos(2.0*M_PI*i/length+M_PI));
        }
        break;
    }
    case WINDOW_TUKEY: // окно Тьюкки
    {
        alph = 0.5;
        for(int i = 0;i<length;i++)
        {
            if((i<alph*length/2.0)||(i>(length-alph*length/2.0)))
            {
                (*windowArray)[i] = (1.0+cos(M_PI*(i-alph*length/2.0)/((1.0-alph)*length/2.0)))/2.0;
            }
            else
            {
                (*windowArray)[i] = 1.0;
            }
        }
        break;
    }
    case WINDOW_COSINE: // косинусное окно
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = cos(M_PI*i/(length-1)-M_PI/2.0);
        }
        break;
    }
    case WINDOW_LANCZOS: // окно Ланцоза ???
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = sin(M_PI*(2.0*i/length))/(2.0*M_PI*i/length);
        }
        break;
    }
    case WINDOW_TRIEANGULAR: // треугольное окно
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (length/2.0-fabs(i-(length-1.0)/2.0))/(length/2.0);
        }
        break;
    }
    case WINDOW_GAUSSIAN: // Гаусово окно
    {
        double p;
        alph = 0.25;
        for(int i = 0; i<length;i++)
        {
            p = pow((i-((length-1.0)/2.0))/(alph*((length-1.0)/2.0)), 2.0)/(-2.0);
            (*windowArray)[i] = pow(M_E, p);
        }
        break;
    }
    case WINDOW_BARTLETT_HANN: // Батлетта-Ханна окно
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = 0.62 - 0.48*fabs(i/(length-1.0)- 0.5) - 0.38*cos(2.0*M_PI*i/(length-1.0));
        }
        break;
    }
    case WINDOW_BLACKMAN: // окно Блекмана
    {
        alph = 0.16;
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (1.0-alph)/2.0 - 0.5*cos(2.0*M_PI*i/(length-1.0)) + (alph/2.0)*cos(4.0*M_PI*i/(length-1.0));
        }
        break;
    }
    case WINDOW_KAISER: // окно Кайзера
    {
        //window[i]:=J0(a*sqrt(1-(2*(i-N2)/(n))*(2*(i-N2)/(n))))/J0(a);
        alph = 12.0;
        double n2 = length / 2.0;
        printf("%f", n2);
        for(int i = 0; i<length;i++){

            double b = alph * sqrt(1.0-pow(2.0*((double)i-n2)/length , 2));
            (*windowArray)[i] = J0(b)/J0(alph);
        }

//        alph = 3.0;
//        double bes = 1.0/J0(alph);
//        long odd = length%2;
//        double xi;
//        double xind = (length-1.0)*(length-1.0);
//        for(int i = 0; i<length;i++)
//        {
//            if (odd){
//                xi = i + 0.5;
//            } else {
//                xi = i;
//            }
//            xi = 4.0*xi*xi;
//            (*windowArray)[i] =  J0(alph*sqrt(1.0-xi/xind))*bes;
//            //  signalArray[i] *= J0(alph*sqrt(1-pow(((2*i/(N-1))-1),2)))/J0(alph);
//        }
        break;
    }
    case WINDOW_NUTTALL: // окно Нуттла
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (0.355768 - 0.487396*cos(2.0*M_PI*i/(length-1)) + 0.1444232*cos(4.0*M_PI*i/(length-1)) - 0.012604*cos(6.0*M_PI*i/(length-1.0)));
        }
        break;
    }
    case WINDOW_BLACKMAN_HARRIS: // окно Блекмана-Хариса
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (0.35875 - 0.48829*cos(2*M_PI*i/(length-1)) + 0.14128*cos(4*M_PI*i/(length-1)) - 0.01168*cos(6*M_PI*i/(length-1)));
        }
        break;
    }
    case WINDOW_BLACKMAN_NUTTALL: // окно Блекмана-Нуттела
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (0.3635819 - 0.4891775*cos(2.0*M_PI*i/(length-1.0)) + 0.1365995*cos(4.0*M_PI*i/(length-1.0)) - 0.0106411*cos(6.0*M_PI*i/(length-1.0)));
        }
        break;
    }
    case WINDOW_FLAT_TOP: // окно Flat Top
    {
        for(int i = 0; i<length;i++)
        {
            (*windowArray)[i] = (1.0 - 1.93*cos(2.0*M_PI*i/(length-1.0))+1.29*cos(4.0*M_PI*i/(length-1))-0.388*cos(6.0*M_PI*i/(length-1))+0.032*cos(8.0*M_PI*i/(length-1.0)));
        }
        break;
    }
    }
}

double Window::J0(double x)
{
//result:=1;
//  for i:=1 to 30 do result:=result+IntPower(IntPower(x/2,i)/factorial(i),2);
    double result = 1.0;
    for(int i = 1; i<=30; i++){
        double a = pow(x/2,(double) i)/((double) factorial(i));
        result += pow(a,2);
    }
    return result;
}

long Window::factorial(long i)
{
    if(i>1){
        return i*factorial(i-1);
    }else{
        return 1;
    }
}
