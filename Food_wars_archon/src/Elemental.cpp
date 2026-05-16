#include "Elemental.h"
#include <cmath>  


Elemental::Elemental(Bando b, int f, int c) : Comida(b, ELEMENTAL, f, c) {
    vidaMax = 200;       // Es una mole de energía
    ataque = 40;         // El ataque más alto del juego
    rangoMovimiento = 0; // No importa porque no se mueve por el tablero
    volador = true;
    vidaActual = vidaMax;
    //calibracion para arena 
    velocidadMov = 0.0f;   // Completamente inmóvil, velocidad nula
    rangoAtaque = 500.0f;  // Rango masivo para castigar desde su posición fija
    cadencia = 1.4f;       // Disparo muy lento pero letal (un impacto cada 1.4s)
}

bool Elemental::intentarMover(int nuevaFila, int nuevaColumna) {
    return false; // El elemental es una torreta, no camina
}