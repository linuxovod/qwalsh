#include "experiment.h"

Experiment::Experiment(ExperimentType expType, ExperimentParams *params)
{

    t = new QVector<double>();
    curves = new QMap<QString, QVector<double>*>();
    type = expType;
    this->params = params;
}

ExperimentParams * Experiment::getParams()
{
    return params;
}

void Experiment::execute()
{
    switch(type){
    case RMS:
        executeRMS();
        break;
    case RMS_PHASE_LIST:
        //executeRMSDiffFreq();
        executeRMSPhase();
        break;
    case TRANSFORM_AND_SIGNAL:
        executeSignalAndTransform();
        break;

    case FIRST_HARMONIC_RMS:
        executeFirstHarmonicRMS1();
        break;
    case DOUBLE_HARMONIC:
        break;
    case FREQUENCE_DETECTION:
        executeFrequenceDetection();
        break;
    }
}

void Experiment::executeRMS()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();
    double fd = (params->getFmax() - params->getFmin()) / params->getExCount();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
    }


    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }

    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){
        QVector<double> *resultW = NULL;
        QVector<double> *resultF = NULL;

        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));

        //расчет точного значения СКЗ для данного окна
        double fExact = 0.0;
        double rmsExactW = 0.0;
        double rmsExactF = 0.0;
        fExact = (params->getFd()/ 4.0) - (params->getFd()/32.0);
        for(int i =0;i< 100; i++) {
            double dfExact = (params->getFd()/16.0)/100.0;
            sParams->setSignalSamplingFreqRatio((fExact + dfExact*i)/params->getFd());

            Signal *signal = new Signal(sParams);
            if(params->isUseWalshTransform()){
                rmsExactW += rms(signal, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF += rms(signal, FOURIER);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            rmsExactW /= 100.0;
            resultW = new QVector<double>();
        }
        if(params->isUseFourierTransform()){
            rmsExactF /= 100.0;
            resultF = new QVector<double>();
        }


        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
            sParams->setSignalSamplingFreqRatio(f/params->getFd());
            if(params->isUseNoice()){
                sParams->setSNR(params->getSnr());
            }
            Signal *signal = new Signal(sParams);
            //Уолш
            if(params->isUseWalshTransform()){
                //Создание сигнала, расчет мощности
                double rmsValW = rms(signal, WALSH);
                // Расчет погрешности
                double error = (rmsExactW-rmsValW)/rmsExactW*100.0;
                //добавление графика
                resultW->append(error);
            }
            //Фурье
            if(params->isUseFourierTransform()){
                //Создание сигнала, расчет мощности
                double rmsValF = rms(signal, FOURIER);
                // Расчет погрешности
                double errorF = (rmsExactF-rmsValF)/rmsExactF*100.0;
                //добавление графика
                resultF->append(errorF);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }
    delete sParams;
#if 0
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setPhase(0.0);
    sParams->setSignalSamplingFreqRatio(params->getFmin()/params->getFd());
    sParams->setSignalType(params->getSignalTypes()->at(0));
    sParams->setWindowType(params->getWindowTypes()->at(0));
    sParams->setSNR(0.0);
    Signal *signal = new Signal(sParams);
    t->empty();

    signal->signal();
    for(int i = 0; i <= params->getN()/2; i++){
        t->append(i);
    }
    result->empty();
    double *tran = signal->fourierTransform()->data();
//    result->append(tran[0]*tran[0]);
//    printf("%d  %0.3f \n",0, tran[0]*tran[0]);
    for(int i = 0; i< params->getN()/2+1; i++){
        result->append(sqrt(tran[(2*i+1)] * tran[(2*i+1)]
                     + tran[(2*i)] * tran[(2*i)]));
        printf("%d  %0.3f \n",i, tran[(2*i+1)] * tran[(2*i+1)] + tran[(2*i)] * tran[(2*i)]);
    }
//    result->append(tran[params->getN()/2]*tran[params->getN()/2]);
//     printf("%d  %0.3f \n",params->getN()/2, tran[params->getN()/2]*tran[params->getN()/2]);



//    str = QObject::trUtf8("Уолш преобразование");
//    curves->insert(str, signal->walshTransform());
//    str = QObject::trUtf8("Фурье");
//    curves->insert(str, signal->fourierTransform());
#endif
}
//Депонировании в ВНИТИ

// Оценка возможности орт преобразований для оценки параметров сигналов
void Experiment::executeRMSDiffFreq()
{
    if(params->getD() == 0){
        executeRMS();
    }else{
    QMap<WindowType, Window*>* windowMap = params->getWindows();
    double fd = (params->getFmax() - params->getFmin()) / params->getExCount();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
    }


    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }

    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){

        QVector<double> *resultW = NULL;
        QVector<double> *resultF = NULL;

        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));

        //расчет точного значения СКЗ для данного окна
        double fExact = 0.0;
        double rmsExactW = 0.0;
        double rmsExactF = 0.0;
        fExact = (params->getFd()/ 4.0) - (params->getFd()/32.0);
        for(int i =0;i< 100; i++) {
            double dfExact = (params->getFd()/16.0)/100.0;
            sParams->setSignalSamplingFreqRatio((fExact + dfExact*i)/params->getFd());

            Signal *signal = new Signal(sParams);
            if(params->isUseWalshTransform()){
                rmsExactW += rms(signal, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF += rms(signal, FOURIER);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            rmsExactW /= 100.0;
            resultW = new QVector<double>();
        }
        if(params->isUseFourierTransform()){
            rmsExactF /= 100.0;
            resultF = new QVector<double>();
        }


        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){

            double maxError = 0.0;
            double fMin = f/params->getFd() *((100 - params->getD())/100.0);
            double fMax = f/params->getFd() *((100.0+params->getD())/100.0);
            double fMd = (fMax - fMin)/20.0;
            for(double ssfr = fMin; ssfr <= fMax; ssfr+=fMd){
            sParams->setSignalSamplingFreqRatio(ssfr);
            if(params->isUseNoice()){
                sParams->setSNR(params->getSnr());
            }
            Signal *signal = new Signal(sParams);
            //Уолш
            if(params->isUseWalshTransform()){
                //Создание сигнала, расчет мощности
                double rmsValW = rms(signal, WALSH);
                // Расчет погрешности
                double error = (rmsExactW-rmsValW)/rmsExactW*100.0;
                //добавление графика
                if(error > 0){
                    if(maxError< error){
                        maxError = error;
                    }
                }else{
                    if(maxError < -error){
                        maxError = -error;
                    }
                }
            }
            //Фурье
            if(params->isUseFourierTransform()){
                //Создание сигнала, расчет мощности
                double rmsValF = rms(signal, FOURIER);
                // Расчет погрешности
                double errorF = (rmsExactF-rmsValF)/rmsExactF*100.0;
                //добавление графика
                resultF->append(errorF);
            }

            delete signal;
            }
            if(params->isUseWalshTransform()){

                resultW->append(maxError);
            }
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }
    delete sParams;
    }
}

void Experiment::executeRMSPhase()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();
    double fd = (params->getFmax() - params->getFmin()) / params->getExCount();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setPhase(0.0);
    sParams->setSignalType(params->getSignalTypes()->at(0));
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
    }

    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }

    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){
        QVector<double> *resultW;
        QVector<double> *resultF;
        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));


        //расчет точного значения СКЗ для данного окна
        double fExact = 0.0;
        double rmsExactW = 0.0;
        double rmsExactF = 0.0;
        //modf( (params->getFd()/ 4.0), &fExact);
        fExact = (params->getFd()/ 4.0) - (params->getFd()/32.0);
        for(int i =0;i< 100; i++){
            double dfExact = (params->getFd()/16.0)/100.0;
            sParams->setSignalSamplingFreqRatio((fExact + dfExact*i)/params->getFd());
            Signal *signal = new Signal(sParams);
            if(params->isUseWalshTransform()){
                rmsExactW += rms(signal, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF += rms(signal, FOURIER);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            rmsExactW /= 100.0;
            resultW = new QVector<double>();
        }
        if(params->isUseFourierTransform()){
            rmsExactF /= 100.0;
            resultF = new QVector<double>();
        }


        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
            double errorWMax = -100.0;
            double errorFMax = -100.0;
            double error = 0.0;
            for(double ph = 0.0; ph <180.0; ph+=1.0){
                sParams->setSignalSamplingFreqRatio(f/params->getFd());
                sParams->setPhase(ph);
                Signal *signal = new Signal(sParams);
                //Уолш
                if(params->isUseWalshTransform()){
                    //Создание сигнала, расчет мощности
                    double rmsValW = rms(signal, WALSH);
                    // Расчет погрешности
                    error = (rmsExactW-rmsValW)/rmsExactW*100.0;
                    //Поиск максимальной погрешности
                    if(error > errorWMax){
                        errorWMax = error;
                    }
                }
                //Фурье
                if(params->isUseFourierTransform()){
                    //Создание сигнала, расчет мощности
                    double rmsValF = rms(signal, FOURIER);
                    // Расчет погрешности
                    error = (rmsExactF-rmsValF)/rmsExactF*100.0;
                    //Поиск максимальной погрешности
                    if(error > errorFMax){
                        errorFMax = error;
                    }
                }
                delete signal;
            }
            if(params->isUseWalshTransform()){
                resultW->append(errorWMax);
            }
             if(params->isUseFourierTransform()){
                resultF->append(errorFMax);
             }
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS_PHASE_LIST, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS_PHASE_LIST, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }
    delete sParams;
}

void Experiment::executeSignalAndTransform()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setSignalSamplingFreqRatio(params->getFmin()/params->getFd());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
        sParams->setSNR(params->getSnr());
    }

    //Обновление значений оси x
    t->empty();
    double td = 1.0/*/params->getFd()*/;
    for(double ti = 0.0; ti < td*params->getN(); ti+=td){
        t->append(ti);
    }
    x2 = new QVector<double>();
    for(double ti = 0.0; ti < params->getN(); ti+=1.0){
        x2->append(ti);
        printf("%f\n", ti);
    }
    printf("%d\n", x2->size());

    for(int i=0; i < params->getWindowTypes()->size(); i++){
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));
        QVector<double> *resultS = new QVector<double>();
        QVector<double> *resultW;
        QVector<double> *resultF;



        Signal *signal = new Signal(sParams);
        for(int i = 0; i< signal->signal()->size(); i++){
            resultS->append(signal->signal()->at(i));
        }

        QString str = " ";
         str.append(curveView(TRANSFORM_AND_SIGNAL, (WindowType)sParams->getWindowType(), NONE));
        curves->insert(str, resultS);

        if(params->isUseWalshTransform()){
            resultW = new QVector<double>();
            double *tran = signal->walshTransform()->data();
            for(int i = 0; i < params->getN()/2; i++){
                resultW->append(sqrt(tran[(2*i+1)] * tran[(2*i+1)]
                                     + tran[(2*i)] * tran[(2*i)]));
            }
//                        for(int i = 0; i < params->getN(); i++){
//                            resultW->append(tran[(i)]);

//                        }

            QString str = curveView(TRANSFORM_AND_SIGNAL, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);

        }
        if(params->isUseFourierTransform()){
            resultF = new QVector<double>();
            double *tran = signal->fourierTransform()->data();
            for(int i = 0; i < params->getN()/2; i++){
                resultF->append(sqrt(tran[(2*i+1)] * tran[(2*i+1)]
                                     + tran[(2*i)] * tran[(2*i)]));
            }
//                        for(int i = 0; i < params->getN(); i++){
//                            resultW->append(tran[(i)]);

//                        }

            QString str = curveView(TRANSFORM_AND_SIGNAL, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(str, resultF);

        }


    }
}

void Experiment::executeFirstHarmonicRMS()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setSignalSamplingFreqRatio(params->getFmin()/params->getFd());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
        sParams->setSNR(params->getSnr());
    }

    double fd = (params->getFmax() - params->getFmin()) / params->getN();
    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }



    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){
        QVector<double> *resultW = NULL;
        QVector<double> *resultF = NULL;

        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));

        //расчет точного значения СКЗ для данного окна
        double fExact = 0.0;
        double rmsExactW = 0.0;
        double rmsExactF = 0.0;
        fExact = (params->getFd()/ 4.0) - (params->getFd()/32.0);
        for(int i =0;i< 100; i++) {
            double dfExact = (params->getFd()/16.0)/100.0;
            sParams->setSignalSamplingFreqRatio((fExact + dfExact*i)/params->getFd());

            Signal *signal = new Signal(sParams);
            if(params->isUseWalshTransform()){
                rmsExactW += rms(signal, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF += rms(signal, FOURIER);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            rmsExactW /= 100.0;
            resultW = new QVector<double>();
        }
        if(params->isUseFourierTransform()){
            rmsExactF /= 100.0;
            resultF = new QVector<double>();
        }


        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
            sParams->setSignalSamplingFreqRatio(f/params->getFd());
            if(params->isUseNoice()){
                sParams->setSNR(params->getSnr());
            }
            Signal *signal = new Signal(sParams);
            //Уолш
            if(params->isUseWalshTransform()){
                //Создание сигнала, расчет мощности
                double rmsValW = rmsFirstHarmonic(signal,f, WALSH);
                // Расчет погрешности
                double error = (rmsExactW-rmsValW)/rmsExactW*100.0;
                //добавление графика
                resultW->append(rmsValW);
            }
            //Фурье
            if(params->isUseFourierTransform()){
                //Создание сигнала, расчет мощности
                double rmsValF = rmsFirstHarmonic(signal,f, FOURIER);
                // Расчет погрешности
                double errorF = (rmsExactF-rmsValF)/rmsExactF*100.0;
                //добавление графика
                resultF->append(rmsValF);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }


}

void Experiment::executeFirstHarmonicRMS1()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setSignalSamplingFreqRatio(params->getFmin()/params->getFd());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
        sParams->setSNR(params->getSnr());
    }

    double fd = (params->getFmax() - params->getFmin()) / params->getN();
    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }



    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){
        QVector<double> *resultW = NULL;
        QVector<double> *resultF = NULL;

        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));

        //расчет точного значения СКЗ для данного окна
        double fExact = 0.0;
        double rmsExactW = 0.0;
        double rmsExactF = 0.0;
        fExact = (params->getFd()/ 4.0) - (params->getFd()/32.0);
        for(int i =0;i< 100; i++) {
            double dfExact = (params->getFd()/16.0)/100.0;
            sParams->setSignalSamplingFreqRatio((fExact + dfExact*i)/params->getFd());

            Signal *signal = new Signal(sParams);
            if(params->isUseWalshTransform()){
                rmsExactW += rms(signal, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF += rms(signal, FOURIER);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            rmsExactW /= 100.0;
            resultW = new QVector<double>();
        }
        if(params->isUseFourierTransform()){
            rmsExactF /= 100.0;
            resultF = new QVector<double>();
        }

        SignalParams *sParamsEx = new SignalParams();
        sParamsEx->setAplitude(1.0);
        sParamsEx->setN(params->getN());
        sParamsEx->setSignalSamplingFreqRatio(params->getFmin()/params->getFd());
        sParamsEx->setPhase(0.0);
        sParamsEx->setSignalType(SIGNAL_SINUS);
        sParamsEx->setWindowType(params->getWindowTypes()->at(i));
        sParamsEx->setWindow(sParams->getWindow());

        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
            sParamsEx->setSignalSamplingFreqRatio(f/params->getFd());
            Signal *signal = new Signal(sParamsEx);
            if(params->isUseWalshTransform()){
                rmsExactW = rmsFirstHarmonic(signal, f, WALSH);
            }
            if(params->isUseFourierTransform()){
                rmsExactF = rmsFirstHarmonic(signal, f, FOURIER);
            }
            delete signal;
            if(params->isUseNoice()){
                sParams->setSNR(params->getSnr());
            }
            sParams->setSignalSamplingFreqRatio(f/params->getFd());
            signal = new Signal(sParams);
            //Уолш
            if(params->isUseWalshTransform()){
                //Создание сигнала, расчет мощности
                double rmsValW = rmsFirstHarmonic(signal,f, WALSH);
                // Расчет погрешности
                double error = (rmsExactW-rmsValW)/rmsExactW*100.0;
                //добавление графика
                resultW->append(rmsValW);
            }
            //Фурье
            if(params->isUseFourierTransform()){
                //Создание сигнала, расчет мощности
                double rmsValF = rmsFirstHarmonic(signal,f, FOURIER);
                // Расчет погрешности
                double errorF = (rmsExactF-rmsValF)/rmsExactF*100.0;
                //добавление графика
                resultF->append(rmsValF);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }


}

void Experiment::executeDoubleHarmonic()
{
}

void Experiment::executeFrequenceDetection()
{
    QMap<WindowType, Window*>* windowMap = params->getWindows();
    double fd = (params->getFmax() - params->getFmin()) / params->getExCount();

    //Параметры сигнала (не изменяющиеся в эксперименте)
    SignalParams *sParams = new SignalParams();
    sParams->setAplitude(1.0);
    sParams->setN(params->getN());
    sParams->setPhase(0.0);
    sParams->setSignalType(SIGNAL_SINUS);
    if(params->isUseNoice()){
        sParams->setSignalType(SIGNAL_BIT_SINUS);
        sParams->setBit(params->getBit());
    }


    //Обновление значений оси x
    t->empty();
    for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
        t->append(f/ params->getFd());
    }
    printf("%d",t->size());

    //Перебор по типам окон
    for(int i=0; i < params->getWindowTypes()->size(); i++){
        QVector<double> *resultW = new QVector<double>();
        QVector<double> *resultF = new QVector<double>();

        // установка окна в пареметрах сигнала
        sParams->setWindowType(params->getWindowTypes()->at(i));
        sParams->setWindow(windowMap->take(params->getWindowTypes()->at(i)));

        //Перебор по частоте
        for(double f= params->getFmin(); f < params->getFmax(); f+=fd){
            sParams->setSignalSamplingFreqRatio(f/params->getFd());
            if(params->isUseNoice()){
                sParams->setSNR(params->getSnr());
            }
            Signal *signal = new Signal(sParams);
            //Уолш
            if(params->isUseWalshTransform()){
                double s_i = 0.0;
                double top = 0.0;
                double bottom = 0.0;
                double *tran = signal->walshTransform()->data();
                double max_val = 0;
                int max_index = 0;
                QVector<double> *transf = new QVector<double>();
                for(int i = 0; i < params->getN()/2; i++){
                   double val =  tran[(2*i)] * tran[(2*i)] + tran[(2*i+1)] * tran[(2*i+1)];
                   transf->append(val);
                   if(max_val<val){
                       max_val = val;
                       max_index = i;
                   }
                }
                int d = params->getD();
                for(int i = ((max_index-d >= 0)? max_index-d : 0); ((i<=max_index+d) && (max_index+d < params->getN()/2) ); i++){
                    s_i = pow(transf->at(i), 1.5);
                    top += s_i *(i)* (params->getFd()/params->getN());
                    bottom += s_i;

                }
                double error = (f - (top/bottom + params->getFd()/(2*params->getN())))/f*100.0;
               resultW->append( error);
            }
            //Фурье
            if(params->isUseFourierTransform()){
                double s_i = 0.0;
                double top = 0.0;
                double bottom = 0.0;
                double *tran = signal->fourierTransform()->data();
                double max_val = 0;
                int max_index = 0;
                QVector<double> *transf = new QVector<double>();
                for(int i = 0; i < params->getN()/2; i++){
                   double val =  tran[(2*i)] * tran[(2*i)] + tran[(2*i+1)] * tran[(2*i+1)];
                   transf->append(val);
                   if(max_val<val){
                       max_val = val;
                       max_index = i;
                   }
                }
                int d = params->getD();
                for(int i = ((max_index-d >= 0)? max_index-d : 0); ((i<=max_index+d) && (max_index+d < params->getN()/2) ); i++){
                    s_i = pow(transf->at(i), 1.5);
                    top += s_i *(i)* (params->getFd()/params->getN());
                    bottom += s_i;

                }
                double error = (f - (top/bottom))/f*100.0;
               resultF->append( error);
            }
            delete signal;
        }
        if(params->isUseWalshTransform()){
            QString str = curveView(RMS, (WindowType)sParams->getWindowType(), WALSH);
            curves->insert(str, resultW);
        }
        if(params->isUseFourierTransform()){
            QString strF = curveView(RMS, (WindowType)sParams->getWindowType(), FOURIER);
            curves->insert(strF, resultF);
        }
    }
    delete sParams;
}

QVector<double>* Experiment::getT()
{
    return t;
}

QVector<double> *Experiment::getX2()
{
    return x2;
}

QMap<QString, QVector<double> *> * Experiment::getCurves()
{
    return curves;
}

double Experiment::rms(Signal* signal, TransformType type)
{
    QVector<double>* transform;
    switch(type){
    case WALSH:
        transform = signal->walshTransform();
        break;
    case FOURIER:
        transform = signal->fourierTransform();
        break;
    }
    double power = 0.0;
    int min = 0;
    int max = transform->size();
    for(int i = min; i < max; i++){
        power = power + transform->at(i)*transform->at(i);
    }
    return sqrt(power);
}

double Experiment::rmsFirstHarmonic(Signal* signal, double f, TransformType type)
{
    QVector<double>* transform;
    switch(type){
    case WALSH:
        transform = signal->walshTransform();
        break;
    case FOURIER:
        transform = signal->fourierTransform();
        break;
    }
    double flog = round(log2(f));
    double min = pow(2, (flog));
//   double min = pow(2, (flog - 1));
    double max = pow(2, flog)+min;

    double power = 0.0;
    for(int i = 0; (i < (max) && (2*i+1) < transform->size()); i++){
//    for(int i = min; (i < (max) && (2*i+1) < transform->size()); i++){
        power = power + transform->at(2*i)*transform->at(2*i) + transform->at(2*i+1)*transform->at(2*i+1);
      //  printf("%d %f %f \n",i,transform->at(i), power);
    }// Степанов Мотвеев ЦОС
    return sqrt(power);
}

QString Experiment::curveView(ExperimentType expType,
                              WindowType winType,
                              TransformType transType)
{
    QString *result = new QString();
    switch(expType){
    case RMS:
    case RMS_PHASE_LIST:
    case TRANSFORM_AND_SIGNAL:
    case FREQUENCE_DETECTION:
        switch(winType)
        {
        case WINDOW_RECTANGULAR:
            result->append(QObject::trUtf8("Прямоугольное окно"));
            break;
        case WINDOW_HANN: //окно Ханна
            result->append(QObject::trUtf8("окно Ханна"));
            break;
        case WINDOW_HAMMING: // окно Хемминга
           result->append(QObject::trUtf8("окно Хемминга"));
            break;
        case WINDOW_TUKEY: // окно Тьюкки
            result->append(QObject::trUtf8("окно Тьюкки"));
            break;
        case WINDOW_COSINE: // косинусное окно
            result->append(QObject::trUtf8("Косинусное окно"));
            break;
        case WINDOW_LANCZOS: // окно Ланцоза ???
            result->append(QObject::trUtf8("окно Ланцоза"));
            break;
        case WINDOW_TRIEANGULAR: // треугольное окно
            result->append(QObject::trUtf8("Треугольное окно"));
            break;
        case WINDOW_GAUSSIAN: // Гаусово окно
            result->append(QObject::trUtf8("Гаусово окно"));
            break;
        case WINDOW_BARTLETT_HANN: // Батлетта-Ханна окно
            result->append(QObject::trUtf8("окно Батлетта-Ханна"));
            break;
        case WINDOW_BLACKMAN: // окно Блекмана
            result->append(QObject::trUtf8("окно Блекмана"));
            break;
        case WINDOW_KAISER: // окно Кайзера
            result->append(QObject::trUtf8("окно Кайзера"));
            break;
        case WINDOW_NUTTALL: // окно Нуттла
            result->append(QObject::trUtf8("окно Нуттела"));
            break;
        case WINDOW_BLACKMAN_HARRIS: // окно Блекмана-Хариса
            result->append(QObject::trUtf8("окно Блекмана-Хариса"));
            break;
        case WINDOW_BLACKMAN_NUTTALL: // окно Блекмана-Нуттела
            result->append(QObject::trUtf8("окно Блекмана-Нуттела"));
            break;
        case WINDOW_FLAT_TOP: // окно Flat Top
            result->append(QObject::trUtf8("окно Флэт-Топ"));
            break;
        }


            switch(transType){
            case WALSH:
                result->append(" - ");
                result->append(QObject::trUtf8("БПУ"));
                break;
            case FOURIER:
                result->append(" - ");
                result->append(QObject::trUtf8("БПФ"));
                break;
            default:
                break;
            }


        break;
    default:
        break;
    }

    return *result;
}

