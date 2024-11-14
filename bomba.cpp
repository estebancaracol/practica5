#include "bomba.h"
#include "mainwindow.h"
#include "personaje.h"

#include <QApplication>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <QObject>
#include <QPixmap>

Bomba::Bomba(QGraphicsItem *parent) :
    QObject(),
    QGraphicsPixmapItem(parent),
    timerExplosion(new QTimer(this)),
    timerAnimacionDestruccion(new QTimer(this)),
    animacionExplosionTimer(new QTimer(this)),
    animacionDestruccionTimer(new QTimer(this)),
    frameIndex(0)
{
    // Cargar los sprites de la bomba
    cargarSpritesBomba();
    cargarSpritesExplosion();
    cargarSpritesBloqueDestruir();

    // Configurar el primer sprite como la imagen inicial
    setPixmap(spritesBomba[0]);
    //setPixmap(spritesBloqueDestruir[0]);

    // Conectar el temporizador para cambiar el sprite y manejar la explosión
    connect(timerExplosion, &QTimer::timeout, this, &Bomba::actualizarEstado);
    timerExplosion->start(700);

    // Conectar el temporizador para cambiar el sprite y manejar la destruccion
    //connect(timerAnimacionDestruccion, &QTimer::timeout, this, &Bomba::animarDestruccionBloque);
    //timerAnimacionDestruccion->start(100);

    // Conectar el temporizador de animación de la explosión
    connect(animacionExplosionTimer, &QTimer::timeout, this, &Bomba::actualizarAnimacionExplosion);

    // Conectar el temporizador de animación de la destruccion
    //connect(animacionDestruccionTimer, &QTimer::timeout, this, &Bomba::actualizarAnimacionExplosion);
}

void Bomba::cargarSpritesBomba() {
    const QSize dimensionDeseada(22, 22);

    spritesBomba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bomba1").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBomba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bomba2").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBomba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bomba3").scaled(dimensionDeseada, Qt::KeepAspectRatio));
}

void Bomba::cargarSpritesExplosion() {
    const QSize dimensionDeseada(22, 22);

    spritesExplosion.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/E1").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesExplosion.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/E2").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesExplosion.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/E3").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesExplosion.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/E4").scaled(dimensionDeseada, Qt::KeepAspectRatio));
}

void Bomba::cargarSpritesBloqueDestruir() {
    const QSize dimensionDeseada(24, 24);  // Tamaño deseado para cada sprite

    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque3.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque4.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque5.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque6.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque7.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesBloqueDestruir.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque8.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    qDebug() << "Sprites cargados correctamente.";
}

// void Bomba::iniciarAnimacionDestruccion() {
//     // Configurar y arrancar el temporizador para la animación de destrucción
//     if (!timerAnimacionDestruccion) {
//         timerAnimacionDestruccion = new QTimer(this);
//         connect(timerAnimacionDestruccion, &QTimer::timeout, this, &Bomba::animarDestruccionBloque);
//     }
//     frameIndexDestruccion = 0;  // Reiniciar el índice del frame
//     timerAnimacionDestruccion->start(100);  // Ajusta la velocidad de la animación (ms)
// }

// void Bomba::animarDestruccionBloque() {
//     static int frameIndexDestruccion = 0;
//     setPixmap(spritesBloqueDestruir[frameIndexDestruccion]);
//     frameIndexDestruccion = (frameIndexDestruccion + 1) % spritesBloqueDestruir.size();

//     if (frameIndexDestruccion == 0){
//         destruir();
//     }
// }

void Bomba::animarDestruccionBloque(QGraphicsItem* bloque) {
    static int frameIndexDestruccion = 0;

    // Cambiar al siguiente sprite en la secuencia de destrucción
    setPixmap(spritesBloqueDestruir[frameIndexDestruccion]);
    frameIndexDestruccion = (frameIndexDestruccion + 1) % spritesBloqueDestruir.size();

    // Si la animación ha completado un ciclo, eliminar el bloque
    if (frameIndexDestruccion == 0) {
        destruir();
    }
}

void Bomba::actualizarEstado(){
    static int frameIndex = 0;
    setPixmap(spritesBomba[frameIndex]);
    frameIndex = (frameIndex + 1) % spritesBomba.size();

    // Después de un ciclo completo de animación, detonar la bomba
    if (frameIndex == 0){
        explotar();
    }
}

void Bomba::actualizarAnimacionExplosion() {
    if (frameIndex < spritesExplosion.size()) {
        QPixmap frame = spritesExplosion[frameIndex];

        // Explosión en el centro de la bomba
        QGraphicsPixmapItem* explosionCenter = new QGraphicsPixmapItem(frame);
        explosionCenter->setPos(pos());
        explosionCenter->setData(0, "explosion");
        scene()->addItem(explosionCenter);
        explosionItems.append(explosionCenter);

        // Manejar colisiones en el centro de la explosión
        QList<QGraphicsItem*> itemsEnCentro = scene()->collidingItems(explosionCenter);
        manejarColisiones(itemsEnCentro);

        // Expansión de la explosión en las cuatro direcciones
        const int rango = 1;        // Alcance de la explosión en bloques
        const int blockSize = 26;   // Tamaño de cada bloque
        QList<QPoint> direcciones = {QPoint(0, -1), QPoint(0, 1), QPoint(-1, 0), QPoint(1, 0)};

        for (const QPoint &direccion : direcciones) {
            expandirExplosionEnDireccion(direccion, rango, blockSize, frame);
        }

        frameIndex++;
    } else {
        // Detener la animación y limpiar los ítems de explosión
        animacionExplosionTimer->stop();
        eliminarExplosion();
    }
}

// Función auxiliar para manejar colisiones de la explosión
void Bomba::manejarColisiones(QList<QGraphicsItem*> items) {
    for (QGraphicsItem* item : items) {
        QString tipoObjeto = item->data(0).toString();

        if (tipoObjeto == "globo") {
            enemigo* globo = dynamic_cast<enemigo*>(item);
            if (globo) {
                QVector<QPixmap> spritesEliminar;
                globo->cargarSpritesEnemigosColisionando(spritesEliminar);
                globo->setPixmap(spritesEliminar[0]);
                globosColisionados.append(globo);
            }
        } else if (tipoObjeto == "destructible") {
            iniciarDestruccionBloque(item);
        }
    }
}

// Función auxiliar para expandir la explosión en una dirección dada
void Bomba::expandirExplosionEnDireccion(const QPoint& direccion, int rango, int blockSize, const QPixmap& frame) {
    bool explosionActiva = true;

    for (int i = 1; i <= rango && explosionActiva; ++i) {
        int x = pos().x() + i * blockSize * direccion.x();
        int y = pos().y() + i * blockSize * direccion.y();

        QGraphicsItem* item = scene()->itemAt(x, y, QTransform());
        if (item) {
            QString tipoObjeto = item->data(0).toString();
            if (tipoObjeto == "indestructible" || tipoObjeto == "pared") {
                explosionActiva = false; // Detener si se encuentra un obstáculo indestructible
            } else if (tipoObjeto == "destructible") {
                iniciarDestruccionBloque(item);
                explosionActiva = false; // Detener en el bloque destructible
            } else if (tipoObjeto == "globo") {
                manejarColisiones({item});
            }
        }

        // Crear el efecto de explosión en esta posición si no hay obstáculo
        if (explosionActiva) {
            QGraphicsPixmapItem* explosionItem = new QGraphicsPixmapItem(frame);
            explosionItem->setPos(x, y);
            explosionItem->setData(0, "explosion");
            scene()->addItem(explosionItem);
            explosionItems.append(explosionItem);
        }
    }
}

// Función auxiliar para iniciar la animación de destrucción de un bloque
void Bomba::iniciarDestruccionBloque(QGraphicsItem* bloque) {

    // QPixmap frame = spritesExplosion[frameIndex];

    // QGraphicsPixmapItem* destruirItem = new QGraphicsPixmapItem(frame);
    // destruirItem->setPos(bloque->pos());
    // destruirItem->setData(0, "BloqueDestruir");
    // scene()->addItem(destruirItem);
    // destruirItems.append(destruirItem);

    // Revisar si el bloque ocultaba una puerta y mostrarla
    QGraphicsItem* puerta = nullptr;
    for (QGraphicsItem* otherItem : scene()->items(bloque->sceneBoundingRect())) {
        if (otherItem->data(0) == "puerta") {
            puerta = otherItem;
            break;
        }
    }

    if (puerta) {
        puerta->setVisible(true);
        puerta->setZValue(0);
        qDebug() << "La puerta ha sido revelada por la explosión.";
    }

    // Eliminar el bloque destructible de la escena
    scene()->removeItem(bloque);
    delete bloque;
}


// void Bomba::actualizarAnimacionExplosion() {
//     if (frameIndex < spritesExplosion.size()) {
//         // Frame de la animación de explosión actual
//         QPixmap frame = spritesExplosion[frameIndex];

//         // Dibujar la explosión en el centro de la bomba
//         QGraphicsPixmapItem* explosionCenter = new QGraphicsPixmapItem(frame);
//         explosionCenter->setPos(pos());
//         explosionCenter->setData(0, "explosion");
//         scene()->addItem(explosionCenter);
//         explosionItems.append(explosionCenter);

//         QList<QGraphicsItem*> itemsEnCentro = scene()->collidingItems(explosionCenter);
//         for (QGraphicsItem* item : itemsEnCentro) {
//             if (item->data(0) == "globo") {
//                 enemigo* globoCentro = dynamic_cast<enemigo*>(item);
//                 if (globoCentro) {
//                     QVector<QPixmap> spritesEliminar;
//                     globoCentro->cargarSpritesEnemigosColisionando(spritesEliminar);
//                     globoCentro->setPixmap(spritesEliminar[0]);
//                     globosColisionados.append(globoCentro);
//                 }
//             }
//         }

//         // Dibujar la explosión en cada dirección hasta el rango o hasta encontrar un obstáculo
//         const int rango = 1;  // Alcance de la explosión en bloques
//         const int blockSize = 26; // Tamaño de cada bloque
//         QList<QPoint> direcciones = {QPoint(0, -1), QPoint(0, 1), QPoint(-1, 0), QPoint(1, 0)};

//         for (const QPoint &direccion : direcciones) {
//             bool explosionActiva = true;

//             for (int i = 1; i <= rango && explosionActiva; ++i) {
//                 int x = pos().x() + i * blockSize * direccion.x();
//                 int y = pos().y() + i * blockSize * direccion.y();

//                 // Verificar si hay un obstáculo en esta posición antes de crear el efecto de explosión
//                 QGraphicsItem *item = scene()->itemAt(x, y, QTransform());

//                 if (item) {
//                     QString tipoObjeto = item->data(0).toString();
//                     if (tipoObjeto == "indestructible" || tipoObjeto == "pared") {
//                         // Detener la explosión en esta dirección si hay un obstáculo indestructible
//                         explosionActiva = false;
//                         continue;

//                     }else if (tipoObjeto == "globo") {
//                         // Crear efecto de animación de destrucción
//                         enemigo* globo = dynamic_cast<enemigo*>(item);
//                         if (globo) {
//                             QVector<QPixmap> spritesEliminar;
//                             globo->cargarSpritesEnemigosColisionando(spritesEliminar);
//                             globo->setPixmap(spritesEliminar[0]);  // Fijar el globo en el primer sprite de colisión
//                         }
//                         globosColisionados.append(globo);

//                     }else if (tipoObjeto == "destructible" ) {
//                         // Revelar la puerta si el bloque es destructible y contiene una
//                         QGraphicsItem* puerta = nullptr;
//                         for (auto otherItem : scene()->items(item->sceneBoundingRect())) {
//                             if (otherItem->data(0) == "puerta") {
//                                 puerta = otherItem;
//                                 break;
//                             }
//                         }

//                         if (puerta) {
//                             puerta->setVisible(true);
//                             puerta->setZValue(0);
//                             qDebug() << "La puerta ha sido revelada por la explosión.";
//                         }

//                         QGraphicsPixmapItem* destruirItem = new QGraphicsPixmapItem(frame);
//                         destruirItem->setPos(x, y);
//                         destruirItem->setData(0, "BloqueDestruir");
//                         scene()->addItem(destruirItem);
//                         destruirItems.append(destruirItem);

//                         //Eliminar el bloque destructible
//                         scene()->removeItem(item);
//                         delete item;

//                         // Detener la expansión de la explosión en esta dirección
//                         explosionActiva = false;
//                     }
//                 }

//                 // Solo agregar el efecto de explosión si no hay un obstáculo
//                 if (explosionActiva) {
//                     QGraphicsPixmapItem* explosionItem = new QGraphicsPixmapItem(frame);
//                     explosionItem->setPos(x, y);
//                     explosionItem->setData(0, "explosion");
//                     scene()->addItem(explosionItem);
//                     explosionItems.append(explosionItem);
//                 }
//             }
//         }

//         frameIndex++;
//     } else {
//         // Detener la animación y eliminar los items de explosión
//         animacionExplosionTimer->stop();
//         eliminarExplosion();
//     }
// }

void Bomba::destruir(){
    timerAnimacionDestruccion->stop();

    animacionDestruccionTimer->start(100);

    destruirItems.clear();

    frameIndexDestruccion = 0;
}

void Bomba::explotar() {
    // Detener el temporizador de animación de la bomba
    timerExplosion->stop();

    // Iniciar la animación de la explosión
    animacionExplosionTimer->start(100); // Cambia de frame cada 100 ms

    // Lista para almacenar las posiciones de explosión para animación
    explosionItems.clear();

    // Resetear el frameIndex para la animación de la explosión
    frameIndex = 0;
}

void Bomba::eliminarExplosion() {
    // Eliminar los items de explosión de la escena
    for (auto item : explosionItems) {
        scene()->removeItem(item);
        delete item;
    }
    explosionItems.clear();

    // Eliminar los items de los bloques de la escena
    for (auto bloque : destruirItems) {
        scene()->removeItem(bloque);
        delete bloque ;
    }
    destruirItems.clear();

    // Iniciar la animación de destrucción de los globos colisionados
    for (auto globo : globosColisionados) {
        if (globo) {
            globo->animarDestruccion();
        }
    }
    globosColisionados.clear();

    // Eliminar la bomba de la escena
    scene()->removeItem(this);
    emit exploto(); // Emitir señal de explosión antes de eliminar la bomba
    delete this;
}

