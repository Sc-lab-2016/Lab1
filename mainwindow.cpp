#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/qmath.h>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_zerar_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onPlotValues(double timeStamp, double sinalCalculado, double sinalSaturado, double leituraTanque1, double leituraTanque2, double setPoint, double erro)
{
    if(sinalSaturado<0){
    sinalSaturado=sinalSaturado*=-1;
    }
    sinalSaturado=(sinalSaturado*12.5)+50;

    ui->progressBar_sinalSaturado1->setValue(10);
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
        //theThread->terminate();
    }

}

void MainWindow::on_atualizar_clicked()
{

    ui->tempo_Hz_s->setCurrentIndex(segundoOuFrequencia);
    ui->basico_nivel1->setValue(basicoNivel1);
    ui->basico_nivel2->setValue(basicoNivel2);
    ui->tempo->setValue(tempo);
    ui->amplitude->setValue(amplitude);
    ui->offset->setValue(offset);
    ui->duracao_max->setValue(duracaoMax);
    ui->duracao_min->setValue(duracaoMin);


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
}

void MainWindow::on_sinal_quadrada_clicked()
{
    proxtipoOnda=quadrada;
}

void MainWindow::on_sinal_senoidal_clicked()
{
    proxtipoOnda=senoidal;
}

void MainWindow::on_sinal_serra_clicked()
{
    proxtipoOnda=serra;
}

void MainWindow::on_sinal_aleatorio_clicked()
{
    proxtipoOnda=aleatorio;
}

void MainWindow::on_zerar_clicked()
{
    malhaFechada=true; // malha aberta ou fechada
    segundoOuFrequencia=0;
    tipoOnda=0; //selecionador de tipo de onda
    basicoNivel1=0;
    basicoNivel2=0;
    tempo=0; // segundos ou hz
    amplitude=0;
    offset=0;
    duracaoMax=0;
    duracaoMin=0;
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
