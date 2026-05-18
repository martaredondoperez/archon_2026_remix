#include "Voladora.h"
#include <cmath>     
#include <algorithm> 


Voladora::Voladora(Bando b, int f, int c) : Comida(b, VOLADORA, f, c) {
    vidaMax = 50;
    ataque = 15;
    rangoMovimiento = 5;
    volador = true;

    vidaActual = vidaMax; // Al nacer, vida max
    //calibracion para arena 
    velocidadMov = 300.0f; // ¡Las más rápidas de la arena!
    rangoAtaque = 100.0f;   // Ligero alcance cuerpo a cuerpo extendido
    cadencia = 0.5f;       // Cadencia de golpeo fluida
}

bool Voladora::intentarMover(int nuevaFila, int nuevaColumna) {

    // si está en la cárcel no se mueve
    if (encarcelada) return false;

    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    // no puedes ir a tu propia casilla
    if (distFilas == 0 && distColumnas == 0) return false;

    // El coste de moverse en diagonal es igual que en línea recta (el máximo de los dos)
    int costeMovimiento = (std::max)(distFilas, distColumnas);

    if (costeMovimiento > rangoMovimiento) return false;

    return true;
}