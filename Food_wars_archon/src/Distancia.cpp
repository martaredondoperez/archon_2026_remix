#include "Distancia.h"
#include <cmath>  


Distancia::Distancia(Bando b, int f, int c) : Comida(b, DISTANCIA, f, c) {
    vidaMax = 40;        // Muy poca vida, mueren rápido
    ataque = 10;         // Daño bajo en tablero
    rangoMovimiento = 3; // Movimiento estándar
    volador = false;
    vidaActual = vidaMax;
}

// Función de movimiento
bool Distancia::intentarMover(int nuevaFila, int nuevaColumna) {
    if (encarcelada) return false;

    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    if (distFilas == 0 && distColumnas == 0) return false;

    int costeMovimiento = distFilas + distColumnas;
    if (costeMovimiento > rangoMovimiento) return false;

    if (distColumnas == 0) {
        return true;
    }
    else {
        return false;
    }
}