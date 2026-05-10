#include "Comida.h"
#include "freeglut.h"
#include "Definiciones.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "ETSIDI.h"

// Constructor
Comida::Comida(Bando b, TipoFicha t, int f, int c) {
    bando = b;
    tipo = t;
    fila = f;
    columna = c;
    sprite = NULL;

    // Asignación de estadísticas según el tipo de tropa
    switch (tipo) {
    case LIDER:
        // El Rey de la Hamburguesa / Chef Michelin
        vidaMax = 100;
        ataque = 25;
        rangoMovimiento = 3;
        //carga de sprite sequence
        if (bando == SALUDABLE) sprite = new ETSIDI::SpriteSequence("imagenes/LIDER_Chef_Michelin.png", 4, 1, 150);
        else sprite = new ETSIDI::SpriteSequence("imagenes/LIDER_Rey_Hamburguesa.png", 4, 1, 150);
        break;

    case PESADA:
        // Hamburguesa Triple / Aguacate Escudo (Tanques)
        vidaMax = 150;       // Muchísima vida
        ataque = 20;         // Pega duro cuerpo a cuerpo
        rangoMovimiento = 2; // Muy lentos
        //carga de sprite sequence
        if (bando == SALUDABLE) sprite = new ETSIDI::SpriteSequence("imagenes/PESADA_Aguacate_Escudo.png", 4, 1, 150);
        else sprite = new ETSIDI::SpriteSequence("imagenes/PESADA_Hamburguesa_Triple.png", 4, 1, 150);
        break;

    case VOLADORA:
        // Pizza Pepperoni / Rodaja de Limón
        vidaMax = 50;        // Poca vida 
        ataque = 15;
        rangoMovimiento = 5; // Pueden cruzar medio tablero rápido
        //carga de sprite sequence
        if (bando == SALUDABLE) sprite = new ETSIDI::SpriteSequence("imagenes/VOLADORA_Rodaja_Limon.png", 4, 1, 150);
        else sprite = new ETSIDI::SpriteSequence("imagenes/VOLADORA_Pizza_Pepperoni.png", 4, 1, 150);
        break;

    case DISTANCIA:
        // Disparo de Ketchup / Chorro de Vitaminas (Los peones)
        vidaMax = 40;        // Muy poca vida, mueren rápido
        ataque = 10;         // Daño bajo en tablero
        rangoMovimiento = 3; // Movimiento estándar
        //carga de sprite sequence
        if (bando == SALUDABLE) sprite = new ETSIDI::SpriteSequence("imagenes/DISTANCIA_Chorro_Vitaminas.png", 4, 1, 150);
        else sprite = new ETSIDI::SpriteSequence("imagenes/DISTANCIA_Disparo_Ketchup.png", 4, 1, 150);
        break;

    case ESPECIAL:
        // Helado Shapeshifter / Jengibre Teleport
        vidaMax = 80;        // Vida media
        ataque = 30;         // Tienen el ataque más fuerte 
        rangoMovimiento = 4; // Muy ágiles
        //carga de sprite sequence
        if (bando == SALUDABLE) sprite = new ETSIDI::SpriteSequence("imagenes/ESPECIAL_Jengibre_Teleport.png", 4, 1, 150);
        else sprite = new ETSIDI::SpriteSequence("imagenes/ESPECIAL_Helado_Shapeshifter.png", 4, 1, 150);
        break;
    
    case ELEMENTAL:
        vidaMax = 200;       // Es una mole de energía
        ataque = 40;         // El ataque más alto del juego
        rangoMovimiento = 0; // No importa porque no se mueve por el tablero

        if (bando == SALUDABLE)
            sprite = new ETSIDI::SpriteSequence("imagenes/ELEMENTAL_Fuego.png", 4, 1, 150);
        else
            sprite = new ETSIDI::SpriteSequence("imagenes/ELEMENTAL_Fuego.png", 4, 1, 150);
        break;

    default:
        // Por si acaso hay un error, creamos una ficha genérica
        vidaMax = 50;
        ataque = 10;
        rangoMovimiento = 3;
        break;
    }
    // Ajuste de tamaño del sprite para que encaje en la casilla
    if (sprite) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }

    // Al nacer, la ficha siempre tiene la vida a tope
    vidaActual = vidaMax;
}

Comida::~Comida() {
    delete sprite;
}

// Función de dibujo temporal (cuadrados de colores para hacer pruebas)
void Comida::dibuja(float xMin, float yMin, float lado) {
    // Si por algún motivo no cargó el sprite, dibujamos el cuadrado antiguo para no ver nada vacío
    if (sprite == NULL) {
        float centroX = xMin + (lado / 2.0f);
        float centroY = yMin + (lado / 2.0f);
        sprite->setPos(centroX, centroY);
        sprite->draw();
        float radioFicha = lado * 0.3f;
        if (bando == SALUDABLE) glColor3ub(0, 150, 0);
        else glColor3ub(150, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(centroX - radioFicha, centroY - radioFicha);
        glVertex2f(centroX + radioFicha, centroY - radioFicha);
        glVertex2f(centroX + radioFicha, centroY + radioFicha);
        glVertex2f(centroX - radioFicha, centroY + radioFicha);
        glEnd();
    }
    else {
        glColor3ub(255, 255, 255);
        glEnable(GL_TEXTURE_2D);
        // Dibujamos el Sprite centrado en la casilla
        sprite->setPos(xMin + lado / 2.0f, yMin + lado / 2.0f);
        sprite->draw();
        glDisable(GL_TEXTURE_2D);
    }
}

void Comida::actualiza(float t) {
    if (sprite) {
        sprite->loop(); // Esto cambia al siguiente frame de tus 4 imágenes
    }
}
//  FUNCIONES PENDIENTES 
bool Comida::intentarMover(int nuevaFila, int nuevaColumna) {
    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    // Regla de oro 1: No puedes "moverte" a la casilla donde ya estás
    if (distFilas == 0 && distColumnas == 0) return false;

    // --- EL CÁLCULO DEL COSTE ---
    int costeMovimiento;
    if (tipo == VOLADORA || tipo == LIDER) {
        // Los que vuelan usan el coste máximo entre los ejes.
        // Moverse 3 casillas en diagonal (3, 3) les cuesta solo 3 de rango.
        costeMovimiento = (std::max)(distFilas, distColumnas);
    }
    else {
        // Los terrestres suman sus pasos.
        costeMovimiento = distFilas + distColumnas;
    }

    if (costeMovimiento > rangoMovimiento) return false;
 

    switch (tipo) {

    case PESADA:
        // Solo puede moverse en LÍNEA RECTA 
        if (distFilas == 0 || distColumnas == 0) {
            return true;
        }
        else {
            return false; 
        }
        break;

    case VOLADORA:
    
        // Mientras esté en su rango, puede ir donde quiera.
        return true;
        break;

    case LIDER:
        // rango bajo, donde quiera
        return true;
        break;

    case DISTANCIA:
        if (distColumnas == 0) {
            return true;
        }
        else {
            return false;
        }
        break;

    case ESPECIAL:
        // como el caballo.
       
        if ((distFilas == 3 && distColumnas == 1) || (distFilas == 1 && distColumnas == 3)) {
            return true;
        }
        else {
            return false;
        }
        break;
    }
}
void Comida::recibirDano(int cantidad) {
    // Aquí se programará cómo baja la vida.
    // De momento, restamos la cantidad directamente.
    vidaActual -= cantidad;

    if (vidaActual < 0) {
        vidaActual = 0;
    }
}