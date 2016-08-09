#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <QThread>
#include <QTimer>
#include "quanser.h"

class threadControl : public QThread
{
    Q_OBJECT


public:

    explicit threadControl(QObject *parent =0);
    Quanser* q = new Quanser("10.13.99.69", 20081);
    void inicia();    
    void zeraParametros(void);    
    void atualizaParametros(bool thr_malhaFechada, int thr_tipoOnda, double thr_basicoNivel1, double thr_basicoNivel2, double thr_tempo, double thr_amplitude, double thr_offset, double thr_duracaoMax, double thr_duracaoMin);

private:

    //variaveis
    bool malhaFechada; // malha aberta ou fechada
    bool segundoOuFrequencia; // true = s ; false= hz
    bool conectado;

    int tipoOnda; //selecionador de tipo de onda

    double basicoNivel1; // nivel tanque 1
    double basicoNivel2; // nivel tanque 2
    double leituraTanque1;
    double leituraTanque2;
    double tempo; // sempre convertido para hz em atualizar
    double amplitude;
    double offset;
    double duracaoMax;
    double erro;
    double duracaoMin;
    double sinalCalculado, sinalSaturado;
    double timeToNextRandomNumber; // medidas de tempo
    double lastTimeStamp, lastLoopTimeStamp; // medidas de tempo

    QTimer *timer;

    //funcoes
    double travaSinal(double sinalCalculado, double leituraTanque1, double leituraTanque2);

signals:
    void plotValues(double,double,double,double,double,double,double);

protected slots:
    void run();
};

#endif // THREADCONTROL_H
