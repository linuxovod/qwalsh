#ifndef SIGNALPARAMSDIALOG_H
#define SIGNALPARAMSDIALOG_H

#include <QDialog>
#include "../logic/experiment.h"
#include "../logic/experimentparams.h"

class QRegExp;

namespace Ui {
    class SignalParamsDialog;
}

class SignalParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignalParamsDialog(QWidget *parent = 0);
    ~SignalParamsDialog();
    ExperimentParams *getParams();
    bool frequencyChanged();
    void resetFreqChanged();

private:
    ExperimentParams *params;
    Ui::SignalParamsDialog *ui;
    //SignalParams * getParamObjFromForm(int windowType);
    bool *checkboxs_window;
    bool freqChanged;

    bool fMinEditSlotFired;
    bool fMinFdEditSlotFired;
    bool fMaxEditSlotFired;
    bool fMaxFdEditSlotFired;
   // QString *window_tytles;

private slots:
    void on_checkBox_15_clicked(bool checked);
    void on_checkBox_14_clicked(bool checked);
    void on_checkBox_13_clicked(bool checked);
    void on_checkBox_12_clicked(bool checked);
    void on_checkBox_11_clicked(bool checked);
    void on_checkBox_10_clicked(bool checked);
    void on_checkBox_9_clicked(bool checked);
    void on_checkBox_8_clicked(bool checked);
    void on_checkBox_7_clicked(bool checked);
    void on_checkBox_6_clicked(bool checked);
    void on_checkBox_5_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_clicked(bool checked);
    void on_fMinLineEdit_textChanged(QString str);
    void on_fMaxLineEdit_textChanged(QString str);
    void on_fMinFdLineEdit_textChanged(QString str);
    void on_fMaxFdLineEdit_textChanged(QString str);
    void on_fdLineEdit_textChanged(QString str);
};

#endif // SIGNALPARAMSDIALOG_H
