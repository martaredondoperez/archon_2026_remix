#pragma once
#include "Vector2D.h"

class Proyectil {
public:
    // Atributos físicos
    Vector2D posicion;
    Vector2D velocidad;
    float radio;

    // Métodos
    Proyectil();           // Constructor
    void dibuja();         // Para pintarlo en pantalla
    void mueve(float t);   // Para que vuele
};

