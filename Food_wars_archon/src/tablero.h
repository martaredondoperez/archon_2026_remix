#pragma once
#include "freeglut.h" 
#include "Comida.h"

class Tablero {

    Comida* casillas[9][9];
    bool puntosNutricion[9][9];     // Matriz para guardar dónde están los 5 Puntos de Nutrición

    float ladoCasilla;
    //memoria raton
    bool haySeleccion; // ficha agarrada?
    int filaSel;       // Fila de la ficha 
    int colSel;        // Columna de la ficha 

public:
    Tablero();          // Constructor
    void dibuja();      // Función que llamará Mundo.cpp
    void inicializa();  // Para resetear el tablero al empezar
    void gestionRaton(int x, int y); //proceso de raton
};