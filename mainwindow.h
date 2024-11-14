#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QGraphicsScene>
#include "mapa.h"
#include "enemigos.h"
#include "personaje.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timerColisiones;
    Personaje* personaje;
    QGraphicsScene *scene;
    QVector<enemigo*> enemigos;
    int puntaje;
    int tiempo;
    QVector<QPoint> posicionesOcupadas;
    QGraphicsRectItem* puerta = nullptr;
    QGraphicsItem* jugador;

private slots:
    void cargarFondo();
    void crearEncabezado();
    void agregarBloquesDestructibles();
    void agregarBloquesIndestructibles();
    void agregarParedes();
    void agregarPuerta();
    void actualizarTiempo();
    void actualizarPuntaje(int puntos);
    void actualizarVidas(int vidas);
};

#endif // MAINWINDOW_H

