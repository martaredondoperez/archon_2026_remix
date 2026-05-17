#include "Distancia.h"
#include <cmath>  


Distancia::Distancia(Bando b, int f, int c) : Comida(b, DISTANCIA, f, c) {
    vidaMax = 40;        // Muy poca vida, mueren rápido
    ataque = 10;         // Daño bajo en tablero
    rangoMovimiento = 3; // Movimiento estándar
    volador = false;
    vidaActual = vidaMax;
    //calibracion para arena 
    velocidadMov = 110.0f; // Velocidad moderada para posicionarse
    rangoAtaque = 350.0f;  // ¡Gran alcance de proyectil!
    cadencia = 0.4f;       // Disparan ráfagas rápidas (espera de 0.4s)
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