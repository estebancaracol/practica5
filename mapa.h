#ifndef MAPA_H
#define MAPA_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QBrush>
#include <QDebug>

class Mapa : public QObject {
    Q_OBJECT
public:
    explicit Mapa(QGraphicsScene* scene, QObject* parent = nullptr);

    void crearEncabezado(QGraphicsView* graphicsView);
    void agregarBloquesDestructibles();
    void agregarBloquesIndestructibles();
    void agregarParedes();
    void agregarPuerta();

private:
    QGraphicsScene* scene;
    bool fondoAgregado = false;
};

#endif // MAPA_H
