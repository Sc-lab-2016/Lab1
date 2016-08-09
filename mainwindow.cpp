#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/qmath.h>
#include <cmath>
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);

   // QMainWindow::showFullScreen();

    malhaFechada=true;
    tipoOnda=0;
    basicoNivel1=0;
    basicoNivel2=0 ;
    tempo=1;
    amplitude=3;
    offset=0;
    duracaoMax=4;
    duracaoMin=4;
    //Cria Threads e conecta signals com slots
    theThread = new threadControl(this);
    connect(theThread,SIGNAL(plotValues(double,double,double,double,double,double, double)),this,SLOT(onPlotValues(double, double,double,double,double,double,double)));


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onPlotValues(double timeStamp, double sinalCalculado, double sinalSaturado, double leituraTanque1, double leituraTanque2, double setPoint, double erro)
{

    if(sinalSaturado<0){
    sinalSaturado=sinalSaturado+(sinalSaturado*12.5);
    }
    else {
    sinalSaturado=(sinalSaturado*12.5)+50;
    }
    ui->progressBar_sinalSaturado1->setValue(sinalSaturado);
    QString s = QString::number(sinalCalculado);
    ui->label_teste->setText(s);


}


void MainWindow::on_basico_nivel1_valueChanged(double arg1)
{
    ui->basico_slider1->setValue(arg1);
    ui->basico_progressBar1->setValue(arg1);
    basicoNivel1=arg1;
}

void MainWindow::on_basico_slider1_valueChanged(int value)
{
    ui->basico_nivel1->setValue(value);
    ui->basico_progressBar1->setValue(value);
    basicoNivel1=value;
}

void MainWindow::on_basico_nivel2_valueChanged(double arg1)
{
    ui->basico_slider2->setValue(arg1);
    ui->basico_progressBar2->setValue(arg1);
    basicoNivel2=arg1;
}

void MainWindow::on_basico_slider2_valueChanged(int value)
{
    ui->basico_nivel2->setValue(value);
    ui->basico_progressBar2->setValue(value);
    basicoNivel2=value;
}

void MainWindow::on_conectar_clicked(bool checked)
{
    conectado=checked;

    if (conectado==true) {
        ui->conectar->setText("Desconectar");
        ui->label_conectar->setText("Conectado");
        theThread->inicia();
        theThread->zeraParametros();
    }
    if(conectado==false){
        ui->conectar->setText("Conectar");
        ui->label_conectar->setText("Desconectado");
        theThread->zeraParametros();
        //Espera a thread ler os valores
        QThread::msleep (1000);
        //Termina a thread
        theThread->terminate();
    }

}

void MainWindow::on_atualizar_clicked()
{
    malhaFechada=ui->malhaFechada->isChecked();
    if(malhaFechada==true){
        ui->label_offset->setText("offset (Cm)");
    }
    if(malhaFechada==false){
       ui->label_offset->setText("offset (Volts)");
    }
    segundoOuFrequencia=ui->tempo_Hz_s->currentIndex();
    basicoNivel1=ui->basico_nivel1->value();
    basicoNivel2=ui->basico_nivel2->value();
    if(ui->tempo_Hz_s->currentIndex()==0){
        tempo=1/tempo;
    }
    tempo=ui->tempo->value();
    amplitude=ui->amplitude->value();
    offset=ui->offset->value();
    duracaoMax=ui->duracao_max->value();
    duracaoMin=ui->duracao_min->value();
    tipoOnda=proxtipoOnda;
    theThread->atualizaParametros(malhaFechada,tipoOnda, basicoNivel1 , basicoNivel2, tempo, amplitude, offset,  duracaoMax,  duracaoMin);

}

void MainWindow::on_tempo_Hz_s_currentIndexChanged(int index)
{
    //periodo = 0
    // hz= 1
    segundoOuFrequencia = (bool)index;
}

void MainWindow::on_sinal_dregrau_clicked()
{
    proxtipoOnda=degrau;
    ui->tempo->setEnabled(false);
    ui->amplitude->setEnabled(false);
    ui->offset->setEnabled(true);
    ui->duracao_max->setEnabled(false);
    ui->duracao_min->setEnabled(false);

}

void MainWindow::on_sinal_quadrada_clicked()
{
    proxtipoOnda=quadrada;
    ui->tempo->setEnabled(true);
    ui->amplitude->setEnabled(true);
    ui->offset->setEnabled(true);
    ui->duracao_max->setEnabled(false);
    ui->duracao_min->setEnabled(false);

}

void MainWindow::on_sinal_senoidal_clicked()
{
    proxtipoOnda=senoidal;
    ui->tempo->setEnabled(true);
    ui->amplitude->setEnabled(true);
    ui->offset->setEnabled(true);
    ui->duracao_max->setEnabled(false);
    ui->duracao_min->setEnabled(false);

}

void MainWindow::on_sinal_serra_clicked()
{
    proxtipoOnda=serra;
    ui->tempo->setEnabled(true);
    ui->amplitude->setEnabled(true);
    ui->offset->setEnabled(true);
    ui->duracao_max->setEnabled(false);
    ui->duracao_min->setEnabled(false);

}

void MainWindow::on_sinal_aleatorio_clicked()
{
    proxtipoOnda=aleatorio;
    ui->tempo->setEnabled(true);
    ui->amplitude->setEnabled(true);
    ui->offset->setEnabled(true);
    ui->duracao_max->setEnabled(true);
    ui->duracao_min->setEnabled(true);

}

void MainWindow::on_zerar_clicked()
{
    malhaFechada=true; // malha aberta ou fechada
    segundoOuFrequencia=0;
    tipoOnda=0; //selecionador de tipo de onda
    basicoNivel1=0;
    ui->basico_nivel1->setValue(basicoNivel1);
    basicoNivel2=0;
    ui->basico_nivel2->setValue(basicoNivel2);
    tempo=0; // segundos ou hz
    ui->tempo->setValue(tempo);
    amplitude=0;
    ui->amplitude->setValue(amplitude);
    offset=0;
    ui->offset->setValue(offset);
    duracaoMax=0;
    ui->duracao_max->setValue(duracaoMax);
    duracaoMin=0;
    ui->duracao_min->setValue(duracaoMin);
    ui->basico_moderada->click();
    ui->leitura_1->clicked();
    ui->leitura_2->clicked();
    ui->leitura_3->setChecked(false);
    ui->leitura_4->setChecked(false);
    ui->leitura_5->setChecked(false);
    ui->leitura_6->setChecked(false);
    ui->leitura_7->setChecked(false);
    ui->escrita_1->setChecked(true);
    ui->malhaFechada->click();
    ui->sinal_dregrau->click();
    on_atualizar_clicked();
}

void MainWindow::on_tempo_valueChanged(double arg1)
{
    tempo=arg1;
}

void MainWindow::on_amplitude_valueChanged(double arg1)
{
    amplitude=arg1;
}

void MainWindow::on_offset_valueChanged(double arg1)
{
    offset=arg1;
}

void MainWindow::on_duracao_max_valueChanged(double arg1)
{
    duracaoMax=arg1;
}

void MainWindow::on_duracao_min_valueChanged(double arg1)
{
    duracaoMin=arg1;
}

void MainWindow::on_pushButton_default_clicked()
{
    offset=2.5;
    if(malhaFechada==true){
        offset=15;
    }
    ui->offset->setValue(offset);
    switch(this->proxtipoOnda)
    {
    case 0://degrau:
        basicoNivel1=0;
        ui->basico_nivel1->setValue(basicoNivel1);
        basicoNivel2=0;
        ui->basico_nivel2->setValue(basicoNivel2);
        tempo=0; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=0;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 1://senoidal:
        basicoNivel1=0;
        ui->basico_nivel1->setValue(basicoNivel1);
        basicoNivel2=0;
        ui->basico_nivel2->setValue(basicoNivel2);
        tempo=0; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=0;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 2://quadrada:
        basicoNivel1=0;
        ui->basico_nivel1->setValue(basicoNivel1);
        basicoNivel2=0;
        ui->basico_nivel2->setValue(basicoNivel2);
        tempo=0; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=0;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 3://serra:
        basicoNivel1=0;
        ui->basico_nivel1->setValue(basicoNivel1);
        basicoNivel2=0;
        ui->basico_nivel2->setValue(basicoNivel2);
        tempo=0; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=0;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 4://aleatorio:
        basicoNivel1=0;
        ui->basico_nivel1->setValue(basicoNivel1);
        basicoNivel2=0;
        ui->basico_nivel2->setValue(basicoNivel2);
        tempo=0; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=0;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    default:
        qDebug() << "ERRO: Nenhuma onda selecionada!";
    }

}

void MainWindow::on_malhaAberta_clicked(bool checked)
{

    if(checked==true){
        ui->label_offset->setText("offset (Volts)");
        this->malhaFechada==false;
    }

    ui->offset->setMinimum(-4.0);
    ui->offset->setMaximum(4.0);
}

void MainWindow::on_malhaFechada_clicked(bool checked)
{
    if(checked==true){
        ui->label_offset->setText("offset (Cm)");
        this->malhaFechada==true;
    }
    ui->offset->setMinimum(0.0);
    ui->offset->setMaximum(30.0);
}
