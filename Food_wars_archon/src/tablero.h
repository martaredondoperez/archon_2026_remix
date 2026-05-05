#pragma once
#include "freeglut.h" 
#include "Comida.h"
#include "ETSIDI.h"
class Tablero {

    Comida* casillas[9][9];
    bool puntosNutricion[9][9];     // Matriz para guardar dónde están los 5 Puntos de Nutrición

    float ladoCasilla;
    //memoria raton
    bool haySeleccion; // ficha agarrada?
    int filaSel;       // Fila de la ficha 
    int colSel;        // Columna de la ficha 
    Bando turnoActual;
   
    ETSIDI::Sprite fondo_tablero;

public:
    Tablero();          // Constructor
    void dibuja(bool pausaActiva);      // Función que llamará Mundo.cpp
    void inicializa();  // Para resetear el tablero al empezar
    void gestionRaton(int boton, int x, int y, bool pausaActiva); //proceso de raton
    void setTurnoInicial(Bando bandoElegido);

};