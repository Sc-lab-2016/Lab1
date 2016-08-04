
#include "threadControl.h"
#include <QtCore>

using namespace std;

threadControl::threadControl(QObject *parent):
     QThread(parent)
{
zeraParametros();
}

void threadControl::inicia()
{
    // Conecta com os tanques
    Quanser* q = new Quanser("10.13.99.69", 20081);

    //Inicia a contagem de tempo
    lastLoopTimeStamp=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;



    while(1) {

        //Reads Time
        double timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        if(timeStamp-lastLoopTimeStamp > 0.1){
            lastLoopTimeStamp=timeStamp;

            double leituraTanque1 = q->readAD(0) * 6.25;// ler sensor e multiplica pelo ganho
            if (leituraTanque1 < 0) leituraTanque1 = 0;
            double leituraTanque2 = q->readAD(1) * 6.25;
            if (leituraTanque2 < 0) leituraTanque2 = 0;


            switch(tipoOnda)
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



            //Calculates other points
            double sinalSaturado = threadControl::travaSinal(sinalCalculado, basicoNivel1, basicoNivel2 );
            double setPoint =0;
            double erro = 0;


            if(malhaFechada == true){//malha fechada
             setPoint = sinalCalculado;
             erro = setPoint - leituraTanque1;
             // o sinal calculado agora eh o erro
             sinalSaturado = threadControl::travaSinal(erro, basicoNivel1, basicoNivel2 );
            }



            // Escreve no canal 0
            q->writeDA(0, sinalSaturado);

            // Envia valores para o supervisorio
            emit plotValues(timeStamp, sinalCalculado, sinalSaturado, leituraTanque1, leituraTanque2, setPoint, erro);
        }
    }

}

void threadControl::atualizaParametros(double tipoOnda, double basicoNivel1 , double basicoNivel2, double tempo,double amplitude, double offset, double duracaoMax, double duracaoMin )
{
    this->malhaFechada = malhaFechada;
    this->tipoOnda = tipoOnda;
    this->basicoNivel1 = basicoNivel1;
    this->basicoNivel2 = basicoNivel2;
    this->tempo = tempo;
    this->amplitude = amplitude;
    this->offset = offset;
    this->duracaoMax = duracaoMax;
    this->duracaoMin = duracaoMin;




}

double threadControl::travaSinal(double sinalCalculado, double leituraTanque1, double leituraTanque2)
{
    double sinalSaturado=sinalCalculado;

    //Trava 1 ( sinal nao pode ser maior 4 ou menor que -4)
    if(sinalCalculado>4) sinalSaturado=4;
    else if(sinalCalculado<-4) sinalSaturado=-4;

    //Trava 2 ( bomba nao pode sugar ar)
    if(leituraTanque1<8 && sinalCalculado<0) sinalSaturado=0;

    //Trava 3 ( tanque 1 cheio, manter)
    if(leituraTanque1>28 && sinalCalculado>2.97) sinalSaturado=2.97;

    //Trava 4 ( tanque 1 cheio, desligar bomba)
    if(leituraTanque1>29 && sinalCalculado>0) sinalSaturado=0;

    //Trava 5 ( tanque 1 vai transbordar tanque 2 , sugar tanque 1)
    if(leituraTanque2 > 26 && leituraTanque1 > 8) sinalSaturado = -4;
    else if (leituraTanque2 > 26) sinalSaturado = 0;


    return sinalSaturado;

}

void threadControl::zeraParametros()
{
    malhaFechada=true; // malha aberta ou fechada
    segundoOuFrequencia=0;

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

