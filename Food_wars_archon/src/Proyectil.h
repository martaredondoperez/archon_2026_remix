#pragma once
#include "Vector2D.h"
#include "Comida.h"

class Proyectil {
public:
    // Atributos físicos
    Vector2D posicion;
    Vector2D velocidad;
    float radio;
    float alcanceMaximo;
    float distanciaRecorrida;
    Bando bando;

    // Métodos
    Proyectil(Bando bandoTirador); // Constructor
    void mueve(float t);   // Para que vuele
};

