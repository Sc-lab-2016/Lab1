#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "threadControl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum tipoOnda { degrau, senoidal, quadrada, serra, aleatorio };
    threadControl *theThread;


public slots:

     void onPlotValues(double timeStamp, double sinalCalculado, double sinalSaturado, double leituraTanque1, double leituraTanque2, double setPoint, double erro);

private slots:

    void on_tempo_valueChanged(double arg1);

    void on_amplitude_valueChanged(double arg1);

    void on_offset_valueChanged(double arg1);

    void on_duracao_max_valueChanged(double arg1);
    void on_duracao_min_valueChanged(double arg1);

    void on_basico_nivel1_valueChanged(double arg1);
    void on_basico_slider1_valueChanged(int value);

    void on_basico_nivel2_valueChanged(double arg1);
    void on_basico_slider2_valueChanged(int value);

    void on_conectar_clicked(bool checked);

    void on_atualizar_clicked();

    void on_tempo_Hz_s_currentIndexChanged(int index);

    void on_sinal_dregrau_clicked();
    void on_sinal_quadrada_clicked();
    void on_sinal_senoidal_clicked();
    void on_sinal_serra_clicked();
    void on_sinal_aleatorio_clicked();

    void on_zerar_clicked();

private:
    Ui::MainWindow *ui;
    QString demoName;
    QTimer dataTimer;

    bool malhaFechada; // malha aberta ou fechada
    bool segundoOuFrequencia; // true = s ; false= hz
    bool conectado;

    int tipoOnda;//selecionador de tipo de onda
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

     void setTickStep();
};

#endif // MAINWINDOW_H
