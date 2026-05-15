#include "pesada.h"
#include <cmath>  


pesada::pesada(Bando b, int f, int c) : Comida(b, PESADA, f, c) {
    vidaMax = 150;
    ataque = 20;
    rangoMovimiento = 2;
    volador = false;
    vidaActual = vidaMax;
}

// Función de movimiento
bool pesada::intentarMover(int nuevaFila, int nuevaColumna) {
    if (encarcelada) return false;

    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    if (distFilas == 0 && distColumnas == 0) return false;

    // Las pesadas suman sus pasos (coste terrestre)
    int costeMovimiento = distFilas + distColumnas;
    if (costeMovimiento > rangoMovimiento) return false;

    // Regla especial de la Pesada: Solo en LÍNEA RECTA (cruz)
    if (distFilas == 0 || distColumnas == 0) {
        return true;
    }
    else {
        return false;
    }
}