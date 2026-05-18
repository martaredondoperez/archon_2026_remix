#include "Especial.h"
#include <cmath>  


Especial::Especial(Bando b, int f, int c) : Comida(b, ESPECIAL, f, c) {
    vidaMax = 80;        // Vida media
    ataque = 30;         // Tienen el ataque más fuerte 
    rangoMovimiento = 4; // Muy ágiles
    volador = true;
    vidaActual = vidaMax;
    //calibracion para arena 
    velocidadMov = 300.0f; // ¡Extremadamente rápido en carrera!
    rangoAtaque = 35.0f;   // Cuerpo a cuerpo estricto
    cadencia = 0.3f;       // Ráfaga de golpes muy rápida (espera de solo 0.3s)
}

// Función de movimiento
bool Especial::intentarMover(int nuevaFila, int nuevaColumna) {
    if (encarcelada) return false;

    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    if (distFilas == 0 && distColumnas == 0) return false;

    int costeMovimiento = distFilas + distColumnas;
    if (costeMovimiento > rangoMovimiento) return false;

    // Regla especial como el caballo
    if ((distFilas == 3 && distColumnas == 1) || (distFilas == 1 && distColumnas == 3)) {
        return true;
    }
    else {
        return false;
    }
}