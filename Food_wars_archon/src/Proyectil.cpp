#include "Proyectil.h"
#include "freeglut.h"
#include "ETSIDI.h"
#include <iostream>
Proyectil::Proyectil(Bando bandoTirador) {
    radio = 5.0f;        // Tamaño del disparo (más pequeño que el luchador)
    posicion.x = 0.0f;   // Se inicializan a 0
    posicion.y = 0.0f;
    velocidad.x = 0.0f;
    velocidad.y = 0.0f;
    bando = bandoTirador;
}

void Proyectil::mueve(float t) {
    // La misma lógica que el luchador, pero sin límites de pantalla
    // porque el proyectil debe poder salir de ella.
    posicion.x += velocidad.x * t;
    posicion.y += velocidad.y * t;

    // Sumamos lo que ha recorrido en este frame (usando valor absoluto)
    distanciaRecorrida += std::abs(velocidad.x * t);
}