#include "signalparamsdialog.h"
#include "ui_signalparamsdialog.h"

SignalParamsDialog::SignalParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignalParamsDialog)
{
    // Установка ui-оболочки.
    ui->setupUi(this);
    // Создание объекта параметров эксперимента.
    params = new ExperimentParams();
    // Инициализация массива с ключами для оконных функций.
    checkboxs_window = new bool[15];
    for(int i = 0;i<15;i++){
        checkboxs_window[i] = false;
    }

    freqChanged = false;
    fMinEditSlotFired = false;
    fMinFdEditSlotFired = false;
    fMaxEditSlotFired = false;
    fMaxFdEditSlotFired = false;

    // Установка валидации полей ввода.
    ui->nLineEdit->setValidator(new QIntValidator(1,16384, this));
    ui->fdLineEdit->setValidator(new QDoubleValidator(this));
    ui->fMaxFdLineEdit->setValidator(new QDoubleValidator(this));
    ui->fMaxLineEdit->setValidator(new QDoubleValidator(this));
    ui->fMinFdLineEdit->setValidator(new QDoubleValidator(this));
    ui->fMinLineEdit->setValidator(new QDoubleValidator(this));
    ui->noiceLineEdit->setValidator(new QIntValidator(0,32, this));
    ui->bitLineEdit->setValidator(new QIntValidator(1,32, this));

    // Подключение сигналов изменения полей ввода.
    connect(ui->fMinLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_fMinLineEdit_textChanged(QString)));
    connect(ui->fMaxLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_fMaxLineEdit_textChanged(QString)));
    connect(ui->fMaxFdLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_fMaxFdLineEdit_textChanged(QString)));
    connect(ui->fMinFdLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_fMinFdLineEdit_textChanged(QString)));
    connect(ui->fdLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_fdLineEdit_textChanged(QString)));
    // Перерерасчет значений отношений частот
    on_fMinLineEdit_textChanged(ui->fMinLineEdit->text());
    on_fMaxLineEdit_textChanged(ui->fMaxLineEdit->text());
}

SignalParamsDialog::~SignalParamsDialog()
{
    delete ui;
    delete params;
}

ExperimentParams * SignalParamsDialog::getParams()
{
    bool ok = false;
    params->setN(ui->nLineEdit->text().toInt(&ok, 10));
    params->setFd(ui->fdLineEdit->text().toDouble(&ok));
    params->setFmin(ui->fMinLineEdit->text().toDouble(&ok));
    params->setFmax(ui->fMaxLineEdit->text().toDouble(&ok));
    params->setUseNoice(ui->noiceCheckBox->isChecked());
    params->setBit(ui->bitLineEdit->text().toInt(&ok, 10));
    params->setSnr(ui->noiceLineEdit->text().toInt(&ok, 10));
    params->setUseFourierTransform(ui->fourierCheckBox->isChecked());
    params->setUseWalshTransform(ui->walshCheckBox->isChecked());

    params->setExCount(ui->lineEdit->text().toInt(&ok, 10));
    params->setD(ui->dLineEdit->text().toInt(&ok, 10));

    QVector<SignalType> *types = new QVector<SignalType>();
    types->append(SIGNAL_SINUS);
    params->setSignalTypes(types);

    QVector<WindowType> *wTypes = new QVector<WindowType>();
    for(int i = 0;i<15;i++){
        switch(i){
        case 0:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_RECTANGULAR);
            }
            break;
        case 1:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_HANN);
            }
            break;
        case 2:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_HAMMING);
            }
            break;
        case 3:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_TUKEY);
            }
            break;
        case 4:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_COSINE);
            }
            break;
        case 5:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_LANCZOS);
            }
            break;
        case 6:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_TRIEANGULAR);
            }
            break;
        case 7:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_GAUSSIAN);
            }
            break;
        case 8:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_BARTLETT_HANN);
            }
            break;
        case 9:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_BLACKMAN);
            }
            break;
        case 10:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_KAISER);
            }
            break;
        case 11:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_NUTTALL);
            }
            break;
        case 12:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_BLACKMAN_HARRIS);
            }
            break;
        case 13:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_BLACKMAN_NUTTALL);
            }
            break;
        case 14:
            if(checkboxs_window[i]){
                wTypes->append(WINDOW_FLAT_TOP);
            }
            break;
        }
    }
    params->setWindowTypes(wTypes);
    return params;
}


void SignalParamsDialog::on_checkBox_clicked(bool checked)
{
    checkboxs_window[0] = checked;
}

void SignalParamsDialog::on_checkBox_2_clicked(bool checked)
{
    checkboxs_window[1] = checked;
}

void SignalParamsDialog::on_checkBox_3_clicked(bool checked)
{
    checkboxs_window[2] = checked;
}

void SignalParamsDialog::on_checkBox_4_clicked(bool checked)
{
    checkboxs_window[3] = checked;
}

void SignalParamsDialog::on_checkBox_5_clicked(bool checked)
{
    checkboxs_window[4] = checked;
}

void SignalParamsDialog::on_checkBox_6_clicked(bool checked)
{
    checkboxs_window[5] = checked;
}

void SignalParamsDialog::on_checkBox_7_clicked(bool checked)
{
    checkboxs_window[6] = checked;
}

void SignalParamsDialog::on_checkBox_8_clicked(bool checked)
{
    checkboxs_window[7] = checked;
}

void SignalParamsDialog::on_checkBox_9_clicked(bool checked)
{
    checkboxs_window[8] = checked;
}

void SignalParamsDialog::on_checkBox_10_clicked(bool checked)
{
    checkboxs_window[9] = checked;
}

void SignalParamsDialog::on_checkBox_11_clicked(bool checked)
{
    checkboxs_window[10] = checked;
}

void SignalParamsDialog::on_checkBox_12_clicked(bool checked)
{
    checkboxs_window[11] = checked;
}

void SignalParamsDialog::on_checkBox_13_clicked(bool checked)
{
    checkboxs_window[12] = checked;
}

void SignalParamsDialog::on_checkBox_14_clicked(bool checked)
{
    checkboxs_window[13] = checked;
}

void SignalParamsDialog::on_checkBox_15_clicked(bool checked)
{
    checkboxs_window[14] = checked;
}

void SignalParamsDialog::on_fMinLineEdit_textChanged(QString str){
    freqChanged = true;
     if(ui->fdLineEdit->text().toDouble() > 0 && str.toDouble() != 0){
         double fmin = str.toDouble() / ui->fdLineEdit->text().toDouble();
         ui->fMinFdLineEdit->setText((new QVariant(fmin))->toString());
     }
}

void SignalParamsDialog::on_fMaxLineEdit_textChanged(QString str){
    freqChanged = true;
    if(ui->fdLineEdit->text().toDouble() > 0 && str.toDouble() != 0){
        double fmax = str.toDouble() / ui->fdLineEdit->text().toDouble();
        ui->fMaxFdLineEdit->setText((new QVariant(fmax))->toString());
    }
}

void SignalParamsDialog::on_fMinFdLineEdit_textChanged(QString str)
{
    if(ui->fdLineEdit->text().size() > 0 ){
        double fmin = str.toDouble() * ui->fdLineEdit->text().toDouble();
        ui->fMinLineEdit->setText((new QVariant(fmin))->toString());
    }
}

void SignalParamsDialog::on_fMaxFdLineEdit_textChanged(QString str)
{
    if(ui->fdLineEdit->text().size() > 0 ){
        double fmax = str.toDouble() * ui->fdLineEdit->text().toDouble();
        ui->fMaxLineEdit->setText((new QVariant(fmax))->toString());
    }
}

void SignalParamsDialog::on_fdLineEdit_textChanged(QString str)
{
    double fmax = str.toDouble() * ui->fMaxFdLineEdit->text().toDouble();
    ui->fMaxLineEdit->setText((new QVariant(fmax))->toString());
    double fmin = str.toDouble() * ui->fMinFdLineEdit->text().toDouble();
    ui->fMinLineEdit->setText((new QVariant(fmin))->toString());
}

bool SignalParamsDialog::frequencyChanged()
{
    return freqChanged;
}

void SignalParamsDialog::resetFreqChanged()
{
    freqChanged = false;
}

