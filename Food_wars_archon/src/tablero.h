#pragma once
#include "freeglut.h" 

class Tablero {
private:
    int casillas[9][9];

    bool puntosNutricion[9][9];     // Matriz para guardar dónde están los 5 Puntos de Nutrición

    float ladoCasilla;

public:
    Tablero();          // Constructor
    void dibuja();      // Función que llamará Mundo.cpp
    void inicializa();  // Para resetear el tablero al empezar
};