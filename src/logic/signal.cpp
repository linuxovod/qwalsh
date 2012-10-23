#include "signal.h"
#include <fftw3.h>

Signal::Signal(SignalParams* params)
{
    this->params = params;
    this->signalArray = NULL;
    this->walshTransformArray = new QVector<double>(this->params->getN());
    this->fourierTransformArray = new QVector<double>(this->params->getN()*2+2);
    this->signalCreated = false;
    this->walshCreated = false;
    this->fourierCreated = false;
}

Signal::~Signal()
{
    delete signalArray;
    delete walshTransformArray;
    delete fourierTransformArray;
}

QVector<double>* Signal::signal()
{
    if(!signalCreated){
        this->signalArray = new QVector<double>();
        try {
            createSignal();
            signalCreated = true;
        } catch (QString * ex) {
            return NULL;
        }

    }
    return signalArray;
}

QVector<double>* Signal::walshTransform()
{
    signal();
    if(!walshCreated){
        try {
            createWalshTransform();
            walshCreated = true;
        } catch (QString * ex) {
            return NULL;
        }
    }
    return walshTransformArray;
}

QVector<double>* Signal::fourierTransform()
{
    signal();
    if(!fourierCreated){
      try {
            createFourierTransforn();
            fourierCreated = true;
        } catch (QString * ex) {
            return NULL;
        }
    }
    return fourierTransformArray;
}

void Signal::createSignal()
{
    //Добавить валидацию.
    switch(params->getSignalType())
    {
    case SIGNAL_SINUS:
        createSinSignal();
        break;
    case SIGNAL_BIT_SINUS:
        createBitSinSignal();
        break;
    default:
        throw "Unsupported signal type.";
    }

    int length = params->getN();
    if(params->getWindow() != NULL){
        for(int i = 0; i<length;i++)
        {
            (*signalArray)[i] *= params->getWindow()->get(i);
        }
    }
}

void Signal::createWalshTransform()
{
    FWT(signalArray, walshTransformArray, params->getN(), 1);
}

void Signal::createFourierTransforn()
{
    FFT(signalArray->data(), fourierTransformArray->data(), params->getN());
}

void Signal::createSinSignal()
{
//    QTime time = QTime::currentTime();
//    qsrand((uint)time.msec());
    signalArray->empty();
    double omega = 2.0 * M_PI * params->getSignalSamplingFreqRatio();
    for(int i = 0; i < params->getN(); i++){
        signalArray->append(params->getAmplitude()
                            * sin(omega*i + params->getPhase()));
    }
}

void Signal::createBitSinSignal()
{
    int maxNoise = 0;
    double omega = 2.0 * M_PI * params->getSignalSamplingFreqRatio();
    double amp = pow(2, params->getBit());
    if(params->getSNR() > 0){
            QTime time = QTime::currentTime();
            qsrand((uint)time.msec());
            maxNoise = pow(2,params->getSNR());
    }

    for(int i = 0; i < params->getN(); i++){
        double sign_mod = 0;
        double sign = amp * sin(omega*i + params->getPhase());
        if(params->getSNR() > 0){

             double noise = (double) (qrand() % (maxNoise) );
             sign += (noise - (maxNoise/2));
//             printf("amp = %f, sign = %f, noise = %f\n", amp, sign, noise - maxNoise/2);
        }

        modf(sign, &sign_mod);
        sign_mod /= amp;
        signalArray->append(sign_mod);

     //   printf("amp = %f, sign = %f, sign_mod = %f\n", amp, sign, sign_mod);
    }
}

void Signal::FWT(QVector<double>* in, QVector<double>* out, int n, int dir)
{
    int i,b,k;
    int m=1;
    double p,q;
    double *z,*x,r;
    x = (double*)malloc(n*sizeof(double));
    for(i=0;i<n;i++)
        x[i]=(*in)[i];
   // x = out;
    z=(double*)malloc(n*sizeof(double));

    for(i=0;i<(n-1);i+=2)
    {
        p=x[i]+x[i+1];
        q=x[i]-x[i+1];
        x[i]=p;
        x[i+1]=q;
    }


    while((m<<=1)<n)
    {
        b=k=0;

        do {
            for(i=0;i<m;i+=2)
            {
                z[k]=x[b+i]+x[b+i+m];
                z[k+1]=x[b+i]-x[b+i+m];
                z[k+2]=x[b+i+1]-x[b+i+m+1];
                z[k+3]=x[b+i+1]+x[b+i+m+1];
                k+=4;
            }
            b+=2*m;
        } while(k<n);

        for(i=0;i<n;i++) x[i]=z[i];
    }
    if(dir == 1)
    {
        r = 1.0/n;
    }
    else
    {
        r = 1.0;
    }
    for(i=0;i<n;i++)
        (*out)[i]=x[i]*r;
    free(z);
    free(x);
}

void Signal::FFT(double *inArray, double *outArray, int n)
{
    fftw_complex *out;
    int M = n*2+1;
   // in = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * n );
    out  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * M );
//    for(int i = 0; i <n; i++)
//    {
//        in[i][0] = inArray[i];
//        in[i][1] = 0.0;
//    }
    fftw_plan p;
    p =  fftw_plan_dft_r2c_1d(n, inArray, out, FFTW_ESTIMATE );
    fftw_execute( p );
    fftw_destroy_plan(p);
    double r = 1.0/n;
    for(int i = 0; i <n/2+1; i++)
    {
        outArray[2*i] = out[i][0]*r;
        outArray[2*i+1] = out[i][1]*r;
      //  printf("{%0.3f, %0.3f} \n", out[i][0], out[i][1]);
    }
//    fftw_free(in);
    fftw_free(out);
}







