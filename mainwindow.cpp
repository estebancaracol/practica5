#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "personaje.h"
#include "enemigos.h"

#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    personaje(new Personaje),
    puntaje(0),
    tiempo(0),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    timerColisiones(new QTimer(this))
{
    ui->setupUi(this); 

    // Configurar escena y vista
    scene->setSceneRect(0, 0, 820, 340);
    cargarFondo();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(820, 340);

    // Desactivar las barras de scroll
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Crear el encabezado y agregar bloques
    crearEncabezado();
    agregarBloquesIndestructibles();
    agregarBloquesDestructibles();
    agregarPuerta();

    // Cargar los sprites del personaje
    personaje->cargarSprites();

    // Establecer el primer sprite como imagen inicial del personaje
    personaje->setPixmap(personaje->obtenerSpriteInicial());
    qDebug() << "Sprite inicial del personaje establecido correctamente.";

    // Posicionar el personaje en la esquina superior derecha
    personaje->setPos(26, 26);
    qDebug() << "Personaje posicionado en:" << personaje->pos();

    // Agregar personaje a la escena después de su inicialización
    scene->addItem(personaje);
    personaje->setFlag(QGraphicsItem::ItemIsFocusable);
    personaje->setFocus();

    enemigo::agregarEnemigos(6, scene, enemigos);

    // Actualizar la vista
    ui->graphicsView->update();
}

MainWindow::~MainWindow() {
    delete ui;
    delete timerColisiones;
}

void MainWindow::crearEncabezado() {
    // Crea el widget y layout para el encabezado
    QWidget *widgetEncabezado = new QWidget(this);
    QHBoxLayout *layoutEncabezado = new QHBoxLayout(widgetEncabezado);

    QLabel *labelPuntaje = new QLabel("Puntaje: 0", this);
    QLabel *labelVidas = new QLabel("Vidas: 3", this);
    QLabel *labelTiempo = new QLabel("Tiempo: 300", this);

    layoutEncabezado->addWidget(labelPuntaje);
    layoutEncabezado->addWidget(labelVidas);
    layoutEncabezado->addWidget(labelTiempo);

    widgetEncabezado->setLayout(layoutEncabezado);
    layoutEncabezado->setContentsMargins(10, 10, 10, 10);

    // Añadir el widget de encabezado en el layout principal
    QVBoxLayout *layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->addWidget(widgetEncabezado);
    layoutPrincipal->addWidget(ui->graphicsView);

    QWidget *widgetCentral = new QWidget(this);
    widgetCentral->setLayout(layoutPrincipal);
    setCentralWidget(widgetCentral);
}

void MainWindow::cargarFondo() {
    // Asegurarse de que la escena esté configurada
    if (!scene) return;

    // Establecer las dimensiones de la escena
    scene->setSceneRect(0, 0, 820, 340);

    // Cargar la imagen de fondo
    QPixmap bk("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/backgraund1.png");
    if (bk.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen del fondo.";
        return;  // Salir si no se carga la imagen
    } else {
        qDebug() << "Imagen del fondo cargada correctamente.";
    }

    // Crear el QGraphicsPixmapItem con la imagen escalada
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(bk.scaled(820, 340, Qt::KeepAspectRatio));
    fondo->setPos(0, 0);

    // Agregar el fondo a la escena
    scene->addItem(fondo);


    // Asignar un identificador al fondo para reconocerlo en la detección de colisiones
    fondo->setData(0, "fondo");

    // Configurar el QGraphicsView con la escena
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(820, 340);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::agregarBloquesDestructibles() {
    if (!scene) return;

    const int blockSize = 26;
    const int rows = 12;
    const int cols = 30;

    QPixmap bloqueDestructible("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque2.png");
    if (bloqueDestructible.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen del bloque destructible.";
        return;
    }
    QBrush brush(bloqueDestructible.scaled(blockSize, blockSize, Qt::KeepAspectRatio));

    // Almacena las posiciones ocupadas para evitar superposición
    QVector<QPoint> posicionesOcupadas;

    // Llenamos posicionesOcupadas con los bloques indestructibles
    for (int i = 0; i < rows; i += 2) {
        for (int j = 0; j < cols; j += 2) {
            if (i == 1 || i == rows - 1 || j == 1 || j == cols - 1) {
                continue; // Borde de la escena
            }
            posicionesOcupadas.append(QPoint(j, i));
        }
    }

    int numBloquesDestructibles = 100;
    const int reservedStartRow = 0;
    const int reservedStartCol = 0;
    const int reservedRows = 3;
    const int reservedCols = 3;

    // Añadir aleatoriamente bloques destructibles
    while (numBloquesDestructibles > 0) {
        int i = QRandomGenerator::global()->bounded(0, rows);
        int j = QRandomGenerator::global()->bounded(0, cols);

        // Evita la posición de los bloques indestructibles y las paredes
        if (i % 2 == 0 && j % 2 == 0) continue;
        if (i == 0 || i == rows || j == 0 || j == cols) continue;

        // Evitar el área reservada en la esquina superior derecha
        if (i >= reservedStartRow && i < reservedStartRow + reservedRows &&
            j >= reservedStartCol && j < reservedStartCol + reservedCols) {
            continue;
        }

        QPoint pos(j, i);
        if (!posicionesOcupadas.contains(pos)) {
            scene->addRect(j * blockSize, i * blockSize, blockSize, blockSize, Qt::NoPen, brush)->setData(0, "destructible");
            posicionesOcupadas.append(pos);
            numBloquesDestructibles--;

            qDebug() << "Bloque destructible añadido en posición:" << pos;
        }
    }
}

void MainWindow::agregarBloquesIndestructibles() {
    if (!scene) return;

    // Dimensiones de cada bloque y de la cuadrícula
    const int blockSize = 26;
    const int rows = 12;
    const int cols = 30;

    QPixmap bloqueIndestructible("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque1.png");
    if (bloqueIndestructible.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen del bloque.";
        return; // Salir si la imagen no se carga
    }
    QBrush brush(bloqueIndestructible.scaled(blockSize, blockSize, Qt::KeepAspectRatio));

    // Crear una cuadrícula con bloques indestructibles en posiciones pares
    for (int i = 0; i < rows; i += 2) {
        for (int j = 0; j < cols; j += 2) {
            // Evitar agregar bloques en las posiciones de las paredes exteriores
            if (i == 1 || i == rows - 1 || j == 1 || j == cols - 1) {
                continue; // Saltar bloques que están en el borde de la escena
            }
            QGraphicsRectItem* bloque = scene->addRect(j * blockSize, i * blockSize, blockSize, blockSize, Qt::NoPen, brush);
            bloque->setData(0, "indestructible");
        }
    }

    // Añadir las paredes exteriores
    agregarParedes();
}

void MainWindow::agregarParedes() {
    if (!scene) return;

    const int blockSize = 26;
    QPixmap borde("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/bloque1.png");
    if (borde.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen del borde.";
        return; // Salir si la imagen no se carga
    }
    QBrush bordeBrush(borde.scaled(blockSize, blockSize, Qt::KeepAspectRatio));

    // Paredes exteriores
    for (int x = 0; x <= 30 * blockSize; x += blockSize) {
        QGraphicsRectItem* bloqueSuperior = scene->addRect(x, 0, blockSize, blockSize, Qt::NoPen);
        bloqueSuperior->setBrush(bordeBrush);  // Superior
        bloqueSuperior->setData(0, "pared");

        QGraphicsRectItem* bloqueInferior = scene->addRect(x, 12 * blockSize, blockSize, blockSize, Qt::NoPen);
        bloqueInferior->setBrush(bordeBrush);  // Inferior
        bloqueInferior->setData(0, "pared");
    }

    for (int y = blockSize; y < 12 * blockSize; y += blockSize) {
        QGraphicsRectItem* bloqueIzquierda = scene->addRect(0, y, blockSize, blockSize, Qt::NoPen);
        bloqueIzquierda->setBrush(bordeBrush);  // Izquierda
        bloqueIzquierda->setData(0, "pared");

        QGraphicsRectItem* bloqueDerecha = scene->addRect(30 * blockSize, y, blockSize, blockSize, Qt::NoPen);
        bloqueDerecha->setBrush(bordeBrush);  // Derecha
        bloqueDerecha->setData(0, "pared");
    }
}

void MainWindow::agregarPuerta() {
    if (!scene) return;

    const int blockSize = 26;
    QPixmap puertaImg("C:/Users/esteb/OneDrive/Escritorio/Informatica 2/Practica_5/imgs/puerta.png");

    if (puertaImg.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de la puerta.";
        return;
    }

    QBrush puertaBrush(puertaImg.scaled(blockSize, blockSize, Qt::KeepAspectRatio));

    // Obtener todas las posiciones de bloques destructibles
    QList<QGraphicsItem*> bloquesDestructibles = scene->items();
    QVector<QGraphicsItem*> bloquesDisponibles;

    // Filtrar para obtener solo los bloques destructibles
    for (auto item : bloquesDestructibles) {
        if (item->data(0) == "destructible") {
            bloquesDisponibles.append(item);
        }
    }

    // Elegir un bloque destructible aleatorio para la puerta
    if (!bloquesDisponibles.isEmpty()) {
        int indiceAleatorio = QRandomGenerator::global()->bounded(bloquesDisponibles.size());
        QGraphicsItem* bloqueParaPuerta = bloquesDisponibles[indiceAleatorio];

        // Obtener la posición del bloque para colocar la puerta
        QRectF posicionPuerta = bloqueParaPuerta->sceneBoundingRect();

        // Agregar la puerta encima del bloque destructible, manteniéndola invisible
        puerta = scene->addRect(posicionPuerta, Qt::NoPen, puertaBrush);
        puerta->setData(0, "puerta");
        puerta->setZValue(-1);

        qDebug() << "Puerta añadida en la posición:" << posicionPuerta.topLeft();
    } else {
        qDebug() << "Error: No se encontraron bloques destructibles para colocar la puerta.";
    }
}


void MainWindow::actualizarTiempo() {
    if (ui->labelTiempo)
        ui->labelTiempo->setText("Tiempo: " + QString::number(tiempo));
}

void MainWindow::actualizarPuntaje(int puntos) {
    if (ui->labelPuntaje)
        ui->labelPuntaje->setText("Puntaje: " + QString::number(puntos));
}

void MainWindow::actualizarVidas(int vidas) {
    if (ui->labelVidas)
        ui->labelVidas->setText("Vidas: " + QString::number(vidas));
}



