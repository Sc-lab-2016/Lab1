#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <QThread>
#include "quanser.h"
#include <iostream>

class threadControl : public QThread
{
    Q_OBJECT


public:

    explicit threadControl(QObject *parent =0);
    void inicia();    
    void zeraParametros(void);    
    void atualizaParametros(double tipoOnda, double basicoNivel1, double basicoNivel2, double tempo, double amplitude, double offset, double duracaoMax, double duracaoMin);

private:

    //variaveis
    bool malhaFechada; // malha aberta ou fechada
    bool segundoOuFrequencia; // true = s ; false= hz
    bool conectado;

    int tipoOnda; //selecionador de tipo de onda
    int proxtipoOnda;

    double basicoNivel1; // nivel tanque 1
    double basicoNivel2; // nivel tanque 2
    double leituraTanque1;
    double leituraTanque2;
    double tempo; // sempre convertido para hz em atualizar
    double amplitude;
    double offset;
    double duracaoMax;
    double duracaoMin;
    double sinalCalculado, sinalSaturado;
    double erro;
    double lastTimeStamp, timeToNextRandomNumber; // medidas de tempo
    double lastLoopTimeStamp; // medidas de tempo

    //funcoes
    double travaSinal(double sinalCalculado, double leituraTanque1, double leituraTanque2);

signals:
    void plotValues(double,double,double,double,double,double,double);

};

#endif // THREADCONTROL_H
