#include "Personaje.h"
#include "bomba.h"
#include "mainwindow.h"
#include "enemigos.h"

#include <QApplication>
#include <QBrush>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

Personaje::Personaje() : QObject(), QGraphicsPixmapItem() {
    cargarSprites();
    cargarSpritesPersonajeColisionando();
    setPixmap(spritesDerecha[0]);
}

void Personaje::cargarSprites() {
    const QSize dimensionDeseada(24, 24);  // Tamaño deseado para cada sprite

    // Cargar y escalar sprites de movimiento a la derecha
    spritesDerecha.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje7.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesDerecha.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje8.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesDerecha.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje9.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    // Cargar y escalar sprites de movimiento hacia abajo
    spritesAbajo.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje4.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesAbajo.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje5.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesAbajo.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje6.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    // Cargar y escalar sprites de movimiento a la izquierda
    spritesIzquierda.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje1.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesIzquierda.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje2.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesIzquierda.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje3.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    // Cargar y escalar sprites de movimiento hacia arriba
    spritesArriba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje10.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesArriba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje11.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesArriba.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personaje12.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    qDebug() << "Sprites cargados correctamente.";
}

void Personaje::cargarSpritesPersonajeColisionando() {
    const QSize dimensionDeseada(24, 24);  // Tamaño deseado para cada sprite

    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision1.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision2.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision3.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision4.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision5.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision6.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));
    spritesPersonajeColisionando.append(QPixmap("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/personajecolision7.png").scaled(dimensionDeseada, Qt::KeepAspectRatio));

    qDebug() << "Sprites cargados correctamente.";
}

// Función para cargar y mostrar la animación de colisión
void Personaje::mostrarAnimacionColision() {
    const int duracionFrame = 100;  // Duración de cada frame en milisegundos
    const int totalFrames = spritesPersonajeColisionando.size();
    int* frameActual = new int(0);  // Lleva el conteo del frame actual

    // Crear un temporizador que actualice los sprites de colisión en intervalos
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, frameActual, totalFrames, timer]() {
        if (*frameActual < totalFrames) {
            // Mostrar el sprite de colisión correspondiente
            setPixmap(spritesPersonajeColisionando[*frameActual]);
            (*frameActual)++;
        } else {
            // Parar el temporizador cuando la animación termine y eliminar al personaje
            timer->stop();
            delete timer;
            delete frameActual;

            // Remover al personaje de la escena y eliminarlo
            if (scene()) {
                scene()->removeItem(this);
            }
            delete this;  // Liberar la memoria del personaje
        }
    });

    timer->start(duracionFrame);  // Iniciar el temporizador para la animación
}


QPixmap Personaje::obtenerSpriteInicial() {
    // Suponiendo que la dirección inicial sea "Derecha" y los sprites estén cargados correctamente
    if (!spritesAbajo.isEmpty()) {
        return spritesAbajo[1];
    }
    return QPixmap(); // Retorna un pixmap vacío si no hay sprites cargados
}

void Personaje::avanzarSprite(QVector<QPixmap>& sprites, int& indice) {
    // Avanzar al siguiente sprite en el vector, reiniciando si llegamos al final
    if (indice >= sprites.size()) indice = 0;
    setPixmap(sprites[indice]);
    indice++;
}

bool Personaje::puedeMoverseA(int nuevaX, int nuevaY) {
    // Guardar la posición original del personaje
    QPointF posicionOriginal = pos();

    // Mover temporalmente el personaje a la nueva posición para verificar colisiones
    setPos(nuevaX, nuevaY);

    // Obtener la lista de items en colisión en la nueva posición
    QList<QGraphicsItem*> itemsColisionados = collidingItems();

    // Restaurar la posición original del personaje
    setPos(posicionOriginal);

    // Depuración: Mostrar información de los objetos en colisión
    if (!itemsColisionados.isEmpty()) {
        qDebug() << "Colisión detectada en posición:" << nuevaX << nuevaY;

        // Verificar cada item en colisión
        for (QGraphicsItem* item : itemsColisionados) {
            QString tipoItem = item->data(0).toString();  // Obtener el identificador del item

            // Ignorar colisiones con el fondo
            if (tipoItem == "fondo") {
                continue;
            }

            // Registrar en la depuración y bloquear el movimiento para bloques no permitidos
            if (tipoItem == "destructible" || tipoItem == "indestructible" || tipoItem == "pared") {
                qDebug() << "Item en colisión:" << tipoItem;
                return false;  // Movimiento no permitido debido a colisión con bloque
            }
        }
    }
    // No hay colisiones con bloques restrictivos, el personaje puede moverse
    return true;
}

void Personaje::verificarColisiones() {
    QList<QGraphicsItem*> itemsColisionados = collidingItems();
    for (QGraphicsItem* item : itemsColisionados) {
        QString tipoItem = item->data(0).toString();

        if (tipoItem == "explosion" || tipoItem == "globo" || tipoItem == "bomba") {
            mostrarAnimacionColision();  // Muestra la animación de colisión
            emit vidaPerdida();          // Emite una señal para notificar la pérdida de vida
            return;
        }
    }
}


void Personaje::keyPressEvent(QKeyEvent *event) {
    const int desplazamiento = 13;  // Tamaño del paso por cada tecla presionada
    const int blockSize = 26; // Tamaño del bloque de la cuadrícula

    // Verificar si ha pasado suficiente tiempo desde el último movimiento
    if (tiempoUltimoMovimiento.isValid() && tiempoUltimoMovimiento.elapsed() < intervaloMovimiento) {
        return; // No permitir movimiento si el intervalo es muy corto
    }

    // Reiniciar el temporizador para el próximo movimiento
    tiempoUltimoMovimiento.restart();

    int nuevaX = x();
    int nuevaY = y();

    // Determinar la nueva posición según la tecla presionada
    switch (event->key()) {
    case Qt::Key_Right:
        nuevaX += desplazamiento;
        if (puedeMoverseA(nuevaX, nuevaY)) {
            setPos(nuevaX, nuevaY);
            avanzarSprite(spritesDerecha, indiceDerecha);
        }else {
            qDebug() << "Movimiento bloqueado por colisión";
        }
        break;
    case Qt::Key_Left:
        nuevaX -= desplazamiento;
        if (puedeMoverseA(nuevaX, nuevaY)) {
            setPos(nuevaX, nuevaY);
            avanzarSprite(spritesIzquierda, indiceIzquierda);
        }else {
            qDebug() << "Movimiento bloqueado por colisión";
        }
        break;
    case Qt::Key_Up:
        nuevaY -= desplazamiento;
        if (puedeMoverseA(nuevaX, nuevaY)) {
            setPos(nuevaX, nuevaY);
            avanzarSprite(spritesArriba, indiceArriba);
        }else {
            qDebug() << "Movimiento bloqueado por colisión";
        }
        break;
    case Qt::Key_Down:
        nuevaY += desplazamiento;
        if (puedeMoverseA(nuevaX, nuevaY)) {
            setPos(nuevaX, nuevaY);
            avanzarSprite(spritesAbajo, indiceAbajo);
        }else {
            qDebug() << "Movimiento bloqueado por colisión";
        }
        break;
    case Qt::Key_Space:
        if (!bombaActiva) {  // Solo colocar la bomba si no hay otra activa
            Bomba *bomba = new Bomba();

            // Alinear la bomba al centro del bloque más cercano
            int bombaX = static_cast<int>(round(x() / static_cast<double>(blockSize)) * blockSize);
            int bombaY = static_cast<int>(round(y() / static_cast<double>(blockSize)) * blockSize);
            bomba->setPos(bombaX, bombaY);

            if (scene()) {
                scene()->addItem(bomba);
                bombaActiva = true; // Marcar la bomba como activa

                // Conectar señal de explosión para permitir colocar otra bomba
                connect(bomba, &Bomba::exploto, this, &Personaje::bombaExplotada);
            } else {
                qDebug() << "Error: el personaje no está asociado a ninguna escena.";
                delete bomba;
            }
        } else {
            qDebug() << "Ya hay una bomba activa en la escena.";
        }
        break;
    default:
        QGraphicsPixmapItem::keyPressEvent(event);
        break;
    }
}

void Personaje::bombaExplotada() {
    bombaActiva = false; // Restablecer la bandera cuando la bomba explota
}





