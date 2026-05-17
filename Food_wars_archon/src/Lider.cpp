#include "Lider.h"
#include <cmath>  


Lider::Lider(Bando b, int f, int c) : Comida(b, LIDER, f, c) {
    vidaMax = 100;
    ataque = 25;
    rangoMovimiento = 3;
    volador = false;
    vidaActual = vidaMax;
    //calibracion para arena 
    velocidadMov = 100.0f; // Velocidad estándar fluida
    rangoAtaque = 100.0f;   // Cuerpo a cuerpo extendido 
    cadencia = 0.6f;       // Tiempo de recarga balanceado (0.6 segundos)
}

// Función de movimiento
bool Lider::intentarMover(int nuevaFila, int nuevaColumna) {
    if (encarcelada) return false;

    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    if (distFilas == 0 && distColumnas == 0) return false;

    // Las pesadas suman sus pasos (coste terrestre)
    int costeMovimiento = distFilas + distColumnas;
    if (costeMovimiento > rangoMovimiento) return false;

    return true;
}