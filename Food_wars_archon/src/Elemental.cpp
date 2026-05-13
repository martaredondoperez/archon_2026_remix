#include "Elemental.h"
#include <cmath>  


Elemental::Elemental(Bando b, int f, int c) : Comida(b, ELEMENTAL, f, c) {
    vidaMax = 200;       // Es una mole de energía
    ataque = 40;         // El ataque más alto del juego
    rangoMovimiento = 0; // No importa porque no se mueve por el tablero
    volador = true;
    vidaActual = vidaMax;
}

bool Elemental::intentarMover(int nuevaFila, int nuevaColumna) {
    return false; // El elemental es una torreta, no camina
}