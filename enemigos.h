#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include <QList>
#include <QPixmap>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>

class enemigo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    enemigo(QGraphicsScene* scene, QVector<QPixmap>& spritesEnemigos);

    static void cargarSpritesEnemigos(QVector<QPixmap>& sprites);
    static void agregarEnemigos(int cantidad, QGraphicsScene* scene, QVector<enemigo*>& listaEnemigos);
    void cargarSpritesEnemigosColisionando(QVector<QPixmap>& spritesEliminar);
    void actualizarMovimientos(QGraphicsItem* jugador, QGraphicsScene* scene);
    double distancia(QPointF p1, QPointF p2);
    bool hayBombaCerca(enemigo* enemigo, QGraphicsScene* scene);
    void moverseAleatorio();
    void buscarJugador(QGraphicsItem* jugador);
    void huirDeBomba(QGraphicsItem* bomba);
    void animarDestruccion();

private:
    QGraphicsScene* scene;
    QVector<enemigo*> enemigos;
    QVector<QPixmap> spritesEnemigos;
    QVector<QPixmap> spritesEliminar;
    void posicionAleatoriaSinColision();
    bool colisionaConBloques();
    int spriteIndex;
};

#endif // ENEMIGOS_H
