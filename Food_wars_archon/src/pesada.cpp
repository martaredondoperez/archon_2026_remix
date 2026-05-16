#include "pesada.h"
#include <cmath>  


pesada::pesada(Bando b, int f, int c) : Comida(b, PESADA, f, c) {
    vidaMax = 150;
    ataque = 20;
    rangoMovimiento = 2;
    volador = false;
    vidaActual = vidaMax;
    //calibracion para arena 
    velocidadMov = 120.0f; // Velocidad lenta 
    rangoAtaque = 30.0f;  // Cuerpo a cuerpo
    cadencia = 0.8f;     // Ataques lentos pero pesados
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