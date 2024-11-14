#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QVector>
#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QElapsedTimer>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    enum Direccion {
        Izquierda,
        Derecha,
        Arriba,
        Abajo
    };

    Personaje();
    void cargarSprites();
    QPixmap obtenerSpriteInicial();
    bool puedeMoverseA(int nuevaX, int nuevaY);
    Direccion getDireccionActual() const { return direccionActual; }
    void setMoviendo(bool moviendo) { this->moviendo = moviendo; }
    void bombaExplotada();
    void cargarSpritesPersonajeColisionando();
    void verificarColisiones();
    void mostrarAnimacionColision();

signals:
    void vidaPerdida();

protected:

    void keyPressEvent(QKeyEvent *event);

private:

    QVector<QPixmap> spritesDerecha, spritesIzquierda, spritesArriba, spritesAbajo;
    QVector<QPixmap> spritesPersonajeColisionando;
    int indiceDerecha = 0, indiceIzquierda = 0, indiceArriba = 0, indiceAbajo = 0;

    void avanzarSprite(QVector<QPixmap>& sprites, int& indice);

    Direccion direccionActual;
    bool moviendo;
    bool bombaActiva = false;
    QPointF posicionValida;
    const int intervaloMovimiento = 100;
    QElapsedTimer tiempoUltimoMovimiento;

};

#endif // PERSONAJE_H

