#pragma once
#include "freeglut.h"
#include "Vector2D.h" 

class Luchador {
public:
    Vector2D posicion;
    Vector2D velocidad;
    float vida;
    float radio;

    Luchador();
    void dibuja();
    void mueve(float t, float limiteX, float limiteY);
};