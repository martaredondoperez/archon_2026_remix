#pragma once
#include "freeglut.h"
#include "Vector2D.h" 

class Luchador {
public:
    Vector2D posicion;
    Vector2D velocidad;
    float radio;

    int vida;
    int ataque;

    Luchador();
    virtual void dibuja();
    void mueve(float t, float limiteX, float limiteY);
};