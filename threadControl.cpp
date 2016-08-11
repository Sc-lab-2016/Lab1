#include "threadControl.h"
#include <QtCore>


threadControl::threadControl(QObject *parent):
    QThread(parent)
{


}

void threadControl::run(){

    //Conecta com os tanques
    timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    lastLoopTimeStamp=timeStamp;

    double leituraTanque1 =q->readAD(0) * 6.25;// ler sensor e multiplica pelo ganho
    //double leituraTanque1 =0;// ler sensor e multiplica pelo ganho
    if (leituraTanque1 < 0) leituraTanque1 = 0;
    double leituraTanque2 =q->readAD(1) * 6.25;
    //double leituraTanque2 =0;
    if (leituraTanque2 < 0) leituraTanque2 = 0;
    qDebug() << "tipo de onda:";
    qDebug() << tipoOnda;
    switch(this->tipoOnda)
    {
    case 0://degrau:
        sinalCalculado = offset;
        break;
    case 1://senoidal:
        sinalCalculado = qSin(timeStamp*3.14159265359*tempo)*amplitude+offset;
        break;
    case 2://quadrada:
        sinalCalculado = qSin(timeStamp*3.14159265359*tempo)*amplitude;
        if(sinalCalculado>0)sinalCalculado = amplitude+offset;
        else sinalCalculado = -amplitude+offset;
        break;
    case 3://serra:
        sinalCalculado = (fmod((timeStamp*3.14159265359*tempo), (2*3.14159265359))/(2*3.14159265359))*amplitude*2-amplitude+offset;
        break;
    case 4://aleatorio:
        if((timeStamp-lastTimeStamp)>timeToNextRandomNumber){
            sinalCalculado = (double)rand()/RAND_MAX * amplitude * 2 - amplitude + offset;
            lastTimeStamp=timeStamp;
            timeToNextRandomNumber= ((double)rand()/RAND_MAX) * (duracaoMax-duracaoMin) + duracaoMin;
            if (timeToNextRandomNumber>duracaoMax)timeToNextRandomNumber=duracaoMax;//Isso não deveria acontecer
        }
        break;
    default:
        qDebug() << "ERRO: Nenhuma onda selecionada!";
    }


    //Calculates other points]

//    qDebug() << "leituraTanque1:";
//    qDebug() << leituraTanque1;
//    qDebug() << "leituraTanque2:";
//    qDebug() << leituraTanque2;
    qDebug() << "sinalCalculado:";
    qDebug() << sinalCalculado;

    double sinalSaturado = threadControl::travaSinal(sinalCalculado, leituraTanque1, leituraTanque2 );
    double setPoint =0;
    double erro = 0;


    if(malhaFechada == true){//malha fechada
         qDebug() << "malha fechada";
        setPoint = sinalCalculado;
        erro = setPoint-leituraTanque1;
        // o sinal calculado agora eh o erro
        qDebug() << "erro:";
        qDebug() << erro;
        sinalSaturado = threadControl::travaSinal(erro, leituraTanque1, leituraTanque2 );
    }



    // Escreve no canal 0
     qDebug() << "Sinal  Saturado dentro:";
     qDebug() << sinalSaturado;
     q->writeDA(0, sinalSaturado);

         qDebug() << "dentro thread leituraTanque1";
         qDebug() <<leituraTanque1;
         qDebug() << "dentro thread leituraTanque2";
         qDebug() <<leituraTanque2;
         qDebug() << "dentro thread  erro";
         qDebug() <<erro;


    emit plotValues(timeStamp, this->sinalCalculado,
                    sinalSaturado, leituraTanque1,
                    leituraTanque2, setPoint, erro);



    timer->start(50); // tempo de ciclo

     qDebug() << "________________";
}

void threadControl::inicia()
{
    q = new Quanser("10.13.99.69", 20081);
    runTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    timer = new QTimer(this);
    //incia contagem de tempo
    //timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    parar=false;
    connect(timer,SIGNAL(timeout()),this,SLOT(run()));
    timer->start(10);

}

void threadControl::atualizaParametros(bool thr_malhaFechada, int thr_tipoOnda, double thr_basicoNivel1 , double thr_basicoNivel2, double thr_tempo,double thr_amplitude, double thr_offset, double thr_duracaoMax, double thr_duracaoMin )
{
    this->malhaFechada=thr_malhaFechada;
    this->tipoOnda = thr_tipoOnda;
    this->basicoNivel1 = thr_basicoNivel1;
    this->basicoNivel2 = thr_basicoNivel2;
    this->tempo = thr_tempo;
    this->amplitude = thr_amplitude;
    this->offset = thr_offset;
    this->duracaoMax = thr_duracaoMax;
    this->duracaoMin = thr_duracaoMin;
}

double threadControl::travaSinal(double sinalCalculado, double leituraTanque1, double leituraTanque2)
{
    double sinalSaturado=sinalCalculado;

    //Trava 1 ( sinal nao pode ser maior 4 ou menor que -4)
    if(sinalCalculado>4){
        sinalSaturado=4;
         qDebug() << "Trava 1";
    }
    else if(sinalCalculado<-4){
        sinalSaturado=-4;
         qDebug() << "Trava 1";
    }

    //Trava 2 ( bomba nao pode sugar ar)
    if(leituraTanque1<8 && sinalCalculado<0){
        sinalSaturado=0;
         qDebug() << "Trava 2";

    }

    //Trava 3 ( tanque 1 cheio, manter)
    if(leituraTanque1>28 && sinalCalculado>2.97){
        sinalSaturado=2.97;
         qDebug() << "Trava 3";
    }

    //Trava 4 ( tanque 1 cheio, desligar bomba)
    if(leituraTanque1>29 && sinalCalculado>0){
        sinalSaturado=0;
         qDebug() << "Trava 4";
    }

    //Trava 5 ( tanque 1 vai transbordar tanque 2 , sugar tanque 1)
    if(leituraTanque2 > 24 && leituraTanque1 > 1){
        sinalSaturado = -4;
         qDebug() << "Trava 5";
    }
    else if (leituraTanque2 > 24){
        sinalSaturado = 0;
         qDebug() << "Trava 5";
    }


    return sinalSaturado;

}

void threadControl::zeraParametros()
{
    //    malhaFechada=true; // malha aberta ou fechada
    //    segundoOuFrequencia=0;

    tipoOnda=0; //selecionador de tipo de onda
    basicoNivel1=0; // nivel tanque 1
    basicoNivel2=0; // nivel tanque 2
    tempo=0; // segundos ou hz
    amplitude=0;
    offset=0;
    duracaoMax=0;
    duracaoMin=0;
    sinalCalculado=0;
    sinalSaturado=0;
    erro=0;
    lastTimeStamp=0;
    timeToNextRandomNumber=0;
    lastLoopTimeStamp=0;    

}

