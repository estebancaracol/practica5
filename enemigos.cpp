#include "enemigos.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QVector>
#include <QPointF>
#include <QTimer>

enemigo::enemigo(QGraphicsScene* scene, QVector<QPixmap>& sprites)
    : QObject(), QGraphicsPixmapItem(), scene(scene), spritesEnemigos(sprites) {

    setPixmap(spritesEnemigos[0]);
    setFlag(QGraphicsItem::ItemIsMovable);
    posicionAleatoriaSinColision();

    // Configurar temporizador para movimiento aleatorio
    QTimer* timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &enemigo::moverseAleatorio);
    timerMovimiento->start(300);
}

void enemigo::cargarSpritesEnemigos(QVector<QPixmap>& spritesEnemigos) {
    const QSize dimensionDeseada(24, 24);  // Tamaño deseado para cada sprite

    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo1.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo2.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo3.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo4.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo5.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEnemigos.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globo6.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    qDebug() << "Sprites cargados correctamente.";
}

void enemigo::cargarSpritesEnemigosColisionando(QVector<QPixmap>& spritesEliminar) {
    const QSize dimensionDeseada(24, 24);  // Tamaño deseado para cada sprite

    spritesEliminar.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globocolision1.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEliminar.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globocolision2.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEliminar.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globocolision3.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEliminar.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globocolision4.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesEliminar.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/globocolision5.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    qDebug() << "Sprites cargados correctamente.";
}

void enemigo::agregarEnemigos(int cantidad, QGraphicsScene* scene, QVector<enemigo*>& listaEnemigos) {
    QVector<QPixmap> spritesEnemigos;
    cargarSpritesEnemigos(spritesEnemigos);  // Cargar los sprites solo una vez

    for (int i = 0; i < cantidad; ++i) {
        enemigo* nuevoEnemigo = new enemigo(scene, spritesEnemigos);
        nuevoEnemigo->setData(0, "globo");
        scene->addItem(nuevoEnemigo);
        listaEnemigos.append(nuevoEnemigo);
    }
}

void enemigo::animarDestruccion() {
    QVector<QPixmap> spritesEliminar;
    cargarSpritesEnemigosColisionando(spritesEliminar);

    QTimer* timerAnimacion = new QTimer(this);
    int* spriteIndex = new int(0);  // índice para rastrear el sprite actual en la animación

    connect(timerAnimacion, &QTimer::timeout, this, [this, timerAnimacion, spritesEliminar, spriteIndex]() mutable {
        if (*spriteIndex < spritesEliminar.size()) {
            setPixmap(spritesEliminar[*spriteIndex]);  // Cambiar al siguiente sprite
            (*spriteIndex)++;
        } else {
            timerAnimacion->stop();
            delete timerAnimacion;
            delete spriteIndex;

            // Eliminar el globo de la escena al final de la animación
            scene->removeItem(this);
            delete this;
        }
    });

    timerAnimacion->start(100);
}

void enemigo::actualizarMovimientos(QGraphicsItem* jugador, QGraphicsScene* scene) {
    for (auto* enemigo : enemigos) {
        if (distancia(enemigo->pos(), jugador->pos()) < 100) {
            enemigo->buscarJugador(jugador);
        } else if (hayBombaCerca(enemigo, scene)) {
            for (auto* item : scene->items()) {
                if (item->data(0).toString() == "bomba") {
                    enemigo->huirDeBomba(item);
                    break;
                }
            }
        } else {
            enemigo->moverseAleatorio();
        }
    }
}

// Función auxiliar para verificar la distancia entre puntos
double enemigo::distancia(QPointF p1, QPointF p2) {
    return sqrt(pow(p2.x() - p1.x(), 2) + pow(p2.y() - p1.y(), 2));
}

// Función auxiliar para detectar si hay una bomba cerca
bool enemigo::hayBombaCerca(enemigo* enemigo, QGraphicsScene* scene) {
    for (auto* item : scene->items()) {
        if (item->data(0).toString() == "bomba" && distancia(enemigo->pos(), item->pos()) < 100) {
            return true;
        }
    }
    return false;
}

void enemigo::posicionAleatoriaSinColision() {
    int blockSize = 26;
    int rows = 12;
    int cols = 30;
    int maxAttempts = 100;
    int restriccionBloques = 7;

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        int x = QRandomGenerator::global()->bounded(1, cols) * blockSize;
        int y = QRandomGenerator::global()->bounded(1, rows) * blockSize;
        // Verificar si la posición está dentro del área restringida
        if (x < restriccionBloques * blockSize && y < restriccionBloques * blockSize) {
            continue; // Intentar nuevamente si está en la esquina prohibida
        }

        setPos(x, y);

        // Verificar colisiones
        if (!colisionaConBloques()) break;
    }
}

void enemigo::moverseAleatorio() {

    setPixmap(spritesEnemigos[spriteIndex]);
    spriteIndex = (spriteIndex + 1) % spritesEnemigos.size();

    static const QVector<QPointF> direcciones = { {0, 13}, {0, -13}, {13, 0}, {-13, 0} };
    QPointF direccion = direcciones[QRandomGenerator::global()->bounded(direcciones.size())];

    setPos(pos() + direccion);
    if (colisionaConBloques()) setPos(pos() - direccion);
}

bool enemigo::colisionaConBloques() {
    QList<QGraphicsItem*> colisiones = scene->collidingItems(this);
    for (auto* item : colisiones) {
        QString tipo = item->data(0).toString();
        if (tipo == "indestructible" || tipo == "destructible" || tipo == "pared") {
            return true;
        }
    }
    return false;
}

void enemigo::buscarJugador(QGraphicsItem* jugador) {
    QPointF direccionJugador = jugador->pos() - pos();
    if (qAbs(direccionJugador.x()) > qAbs(direccionJugador.y())) direccionJugador.setY(0);
    else direccionJugador.setX(0);

    setPos(pos() + direccionJugador);
    if (colisionaConBloques()) setPos(pos() - direccionJugador);
}

void enemigo::huirDeBomba(QGraphicsItem* bomba) {
    QPointF direccionBomba = pos() - bomba->pos();
    if (qAbs(direccionBomba.x()) > qAbs(direccionBomba.y())) direccionBomba.setY(0);
    else direccionBomba.setX(0);

    setPos(pos() + direccionBomba);
    if (colisionaConBloques()) setPos(pos() - direccionBomba);
}

