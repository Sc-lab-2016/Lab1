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
    plotRangeX = 8;

    // Setup plots
    setupPlot1(ui->customPlot);
    setupPlot2(ui->customPlot2);

    //Inicialização para o plot randômico
    timeToNextRandomNumber=0;


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

void MainWindow::setupPlot1(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "Você deve usar a versão > 4.7");
#endif
    plot1Enable[0]=true;//Red Enabled
    plot1Enable[1]=true;//Blue Enabled

    customPlot->addGraph(); // red line
    customPlot->graph(0)->setPen(QPen(Qt::red));
    customPlot->graph(0)->setAntialiasedFill(false);
    customPlot->addGraph(); // blue line
    customPlot->graph(1)->setPen(QPen(Qt::blue));

    customPlot->addGraph(); // red dot
    customPlot->graph(2)->setPen(QPen(Qt::red));
    customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    customPlot->addGraph(); // blue dot
    customPlot->graph(3)->setPen(QPen(Qt::blue));
    customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    //customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    //Valores no eixo X por segundo, proporcao utilizada no exemplo 8/4=2s
    customPlot->xAxis->setTickStep(plotRangeX/4);
    customPlot->yAxis->setTickStep(plotRangeY/4);
    customPlot->axisRect()->setupFullAxesBox();


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));


}

void MainWindow::setupPlot2(QCustomPlot *customPlot2)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "Você deve usar a versão > 4.7");
#endif

  plot2Enable[0]=true;//Red Enabled
  plot2Enable[1]=true;//Blue Enabled
  plot2Enable[2]=true;//Green Enabled
  plot2Enable[3]=true;//Orange Enabled

  customPlot2->addGraph(); // red line
  customPlot2->graph(0)->setPen(QPen(Qt::red));
  customPlot2->graph(0)->setAntialiasedFill(false);
  customPlot2->addGraph(); // blue line
  customPlot2->graph(1)->setPen(QPen(Qt::blue));
  customPlot2->addGraph(); // green line
  customPlot2->graph(2)->setPen(QPen(Qt::green));
  customPlot2->addGraph(); // orange line
  customPlot2->graph(3)->setPen(QPen(qRgb(255,128,0)));

  customPlot2->addGraph(); // red dot
  customPlot2->graph(4)->setPen(QPen(Qt::red));
  customPlot2->graph(4)->setLineStyle(QCPGraph::lsNone);
  customPlot2->graph(4)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot2->addGraph(); // blue dot
  customPlot2->graph(5)->setPen(QPen(Qt::blue));
  customPlot2->graph(5)->setLineStyle(QCPGraph::lsNone);
  customPlot2->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot2->addGraph(); // green dot
  customPlot2->graph(6)->setPen(QPen(Qt::green));
  customPlot2->graph(6)->setLineStyle(QCPGraph::lsNone);
  customPlot2->graph(6)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot2->addGraph(); // orange dot
  customPlot2->graph(7)->setPen(QPen(qRgb(255,128,0)));
  customPlot2->graph(7)->setLineStyle(QCPGraph::lsNone);
  customPlot2->graph(7)->setScatterStyle(QCPScatterStyle::ssDisc);

  //customPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot2->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot2->xAxis->setAutoTickStep(false);
  //Valores no eixo X por segundo, proporcao utilizada no exemplo 8/4=2s
  customPlot2->xAxis->setTickStep(plotRangeX/4);
  customPlot2->yAxis->setTickStep(plotRangeY/4);
  customPlot2->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot2->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::updatePlot1(double timeStamp, double redPlot, double bluePlot)
{

    //Red
    if(plot1Enable[0]) { //sinal calculado
        ui->customPlot->graph(0)->addData(timeStamp, redPlot);
        ui->customPlot->graph(2)->clearData();
        ui->customPlot->graph(2)->addData(timeStamp, redPlot);
        ui->customPlot->graph(0)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot->graph(0)->rescaleValueAxis();
    }

    //Blue
    if(plot1Enable[1]) { //sinal saturado
        ui->customPlot->graph(1)->addData(timeStamp, bluePlot);
        ui->customPlot->graph(3)->clearData();
        ui->customPlot->graph(3)->addData(timeStamp, bluePlot);
        ui->customPlot->graph(1)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot->graph(1)->rescaleValueAxis(true);
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(timeStamp+0.25, plotRangeX, Qt::AlignRight);
    ui->customPlot->yAxis->setRange(0, plotRangeY, Qt::AlignRight);
    ui->customPlot->replot();

}

void MainWindow::updatePlot2(double timeStamp, double redPlot, double bluePlot, double greenPlot, double orangePlot)
{


    //Red
    if(plot2Enable[0]) { // nivel tanque 1
        ui->customPlot2->graph(0)->addData(timeStamp, redPlot);
        ui->customPlot2->graph(4)->clearData();
        ui->customPlot2->graph(4)->addData(timeStamp, redPlot);
        ui->customPlot2->graph(0)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot2->graph(0)->rescaleValueAxis(true);
    }

    //Blue
    if(plot2Enable[1]){ // nivel tanque 2
        ui->customPlot2->graph(1)->addData(timeStamp, bluePlot);
        ui->customPlot2->graph(5)->clearData();
        ui->customPlot2->graph(5)->addData(timeStamp, bluePlot);
        ui->customPlot2->graph(1)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot2->graph(1)->rescaleValueAxis(true);
    }

    //Green
    if(plot2Enable[2]) { // setpoint
        ui->customPlot2->graph(2)->addData(timeStamp, greenPlot);
        ui->customPlot2->graph(6)->clearData();
        ui->customPlot2->graph(6)->addData(timeStamp, greenPlot);
        ui->customPlot2->graph(2)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot2->graph(2)->rescaleValueAxis(true);
    }

    //Orange
    if(plot2Enable[3]) { //erro
        ui->customPlot2->graph(3)->addData(timeStamp, orangePlot);
        ui->customPlot2->graph(7)->clearData();
        ui->customPlot2->graph(7)->addData(timeStamp, orangePlot);
        ui->customPlot2->graph(3)->removeDataBefore(timeStamp-plotRangeX);
        ui->customPlot2->graph(3)->rescaleValueAxis(true);
    }


    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot2->xAxis->setRange(timeStamp+0.25, plotRangeX, Qt::AlignRight);
    ui->customPlot2->yAxis->setRange(0, plotRangeY, Qt::AlignRight);
    ui->customPlot2->replot();

}




void MainWindow::onPlotValues(double timeStamp, double sinalCalculado, double sinalSaturado, double nivelTanque1, double nivelTanque2, double setPoint, double erro)
{

//    qDebug() << "timeStamp";
//    qDebug() <<timeStamp;
//    qDebug() << "sinalCalculado";
//    qDebug() <<sinalCalculado;
//    qDebug() << "sinalSaturado";
//    qDebug() <<sinalSaturado;


    MainWindow::updatePlot1((timeStamp-theThread->runTime),sinalCalculado,sinalSaturado);
    MainWindow::updatePlot2((timeStamp-theThread->runTime),nivelTanque1,nivelTanque2,setPoint,erro);

    //Update Water Level
    //ui->progressBar->setValue(nivelTanque1*100);
//    ui->label_5->setText(QString::number(nivelTanque1,'g',2)+" cm");
//   // ui->progressBar_2->setValue(nivelTanque2*100);
//    ui->label_7->setText(QString::number(nivelTanque2,'g',2)+" cm");

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
        theThread->parar=false;
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
        theThread->parar=true;
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
    if(ui->tempo_Hz_s->currentIndex()==0){
        tempo=1/ui->tempo->value();
    }
    if(ui->tempo_Hz_s->currentIndex()==1){
        tempo=ui->tempo->value();
    }
    amplitude=ui->amplitude->value();
    offset=ui->offset->value();
    duracaoMax=ui->duracao_max->value();
    duracaoMin=ui->duracao_min->value();
    tipoOnda=proxtipoOnda;
    if(ui->aba_controle->currentIndex()==0){// modo basico
        basicoNivel1=ui->basico_nivel1->value();
        basicoNivel2=ui->basico_nivel2->value();
        offset=basicoNivel1;
        malhaFechada=true;
        tipoOnda=0;

    }
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
    theThread->parar=true;
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
        offset=4;
    }
    ui->offset->setValue(offset);
    switch(this->proxtipoOnda)
    {
    case 0://degrau:
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
        offset=3;
        ui->offset->setValue(offset);
        tempo=10; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=1;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 2://quadrada:
        offset=3;
        ui->offset->setValue(offset);
        tempo=10; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=1;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 3://serra:
        offset=3;
        ui->offset->setValue(offset);
        tempo=10; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=1;
        ui->amplitude->setValue(amplitude);
        duracaoMax=0;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=0;
        ui->duracao_min->setValue(duracaoMin);
        break;
    case 4://aleatorio:
        offset=3;
        ui->offset->setValue(offset);
        tempo=10; // segundos ou hz
        ui->tempo->setValue(tempo);
        amplitude=1;
        ui->amplitude->setValue(amplitude);
        duracaoMax=5;
        ui->duracao_max->setValue(duracaoMax);
        duracaoMin=2;
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
    }
    if(checked==false){
        ui->label_offset->setText("offset (Volts)");
    }

    ui->offset->setMinimum(-4.0);
    ui->offset->setMaximum(4.0);
}

void MainWindow::on_malhaFechada_clicked(bool checked)
{
    if(checked==true){
        ui->label_offset->setText("offset (Cm)");
    }
    ui->offset->setMinimum(0.0);
    ui->offset->setMaximum(30.0);
}



void MainWindow::setTickStep()
{
    //Valores no eixo X por segundo, proporcao utilizada no exemplo 8/4=2s
    ui->customPlot->xAxis->setTickStep(plotRangeX/4);
    ui->customPlot2->xAxis->setTickStep(plotRangeX/4);
    ui->customPlot->yAxis->setTickStep(plotRangeY/4);
    ui->customPlot2->yAxis->setTickStep(plotRangeY/4);
}

void MainWindow::on_spinBox_escalaX_valueChanged(int arg1)
{
    ui->horizontalSlider_escalaX->setValue(arg1);
    plotRangeX = arg1;
    setTickStep();
}

void MainWindow::on_horizontalSlider_escalaX_valueChanged(int value)
{
    ui->spinBox_escalaX->setValue(value);
    plotRangeX = value;
    setTickStep();
}

void MainWindow::on_verticalSlider_escalaY_valueChanged(int value)
{
    ui->spinBox_escalaY->setValue(value);
    plotRangeY = value;
    setTickStep();
}

void MainWindow::on_spinBox_escalaY_valueChanged(int arg1)
{
    ui->verticalSlider_escalaY->setValue(arg1);
    plotRangeY = arg1;
    setTickStep();
}
