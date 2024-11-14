#ifndef BOMBA_H
#define BOMBA_H

#include "enemigos.h"

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPixmap>


class Bomba : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Bomba(QGraphicsItem *parent = nullptr);

    void cargarSpritesBomba();
    void cargarSpritesExplosion();
    void cargarSpritesBloqueDestruir();


signals:
    void exploto();

private slots:
    void actualizarEstado();
    void explotar();
    void destruir();
    void actualizarAnimacionExplosion();
    void eliminarExplosion();
    void animarDestruccionBloque(QGraphicsItem* bloque);
    void manejarColisiones(QList<QGraphicsItem*> items);
    void expandirExplosionEnDireccion(const QPoint& direccion, int rango, int blockSize, const QPixmap& frame);
    void iniciarDestruccionBloque(QGraphicsItem* bloque);

private:
    QVector<enemigo*> globosColisionados;
    QVector<QPixmap> spritesBomba;
    QVector<QPixmap> spritesExplosion;
    QVector<QPixmap> spritesBloqueDestruir;
    QTimer *timerExplosion;
    QTimer *animacionExplosionTimer;
    QTimer *timerAnimacionDestruccion;
    QTimer *animacionDestruccionTimer;
    QList<QGraphicsPixmapItem*> explosionItems;
    QList<QGraphicsPixmapItem*> destruirItems;

    int frameIndex = 0;
    int frameIndexDestruccion = 0;
};
#endif // BOMBA_H

