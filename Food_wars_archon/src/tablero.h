#pragma once
#include "freeglut.h" 
#include "Comida.h"
#include "ETSIDI.h"
class Tablero {

    Comida* casillas[9][9];
   
    float ladoCasilla;
    //memoria raton
    bool haySeleccion; // ficha agarrada?
    int filaSel;       // Fila de la ficha 
    int colSel;        // Columna de la ficha 
    Bando turnoActual;
    int turnosTotales;
    int dificultadIA; // Guardará el nivel elegido
    int numJugadores; // Para saber si activar la IA o no
    ETSIDI::Sprite fondo_tablero;

public:
    Tablero();          // Constructor
    void dibuja(bool pausaActiva);      // Función que llamará Mundo.cpp
    void inicializa();  // Para resetear el tablero al empezar
    void gestionRaton(int boton, int x, int y, bool pausaActiva); //proceso de raton
    void setTurnoInicial(Bando bandoElegido);
    void setDificultad(int d) { dificultadIA = d; }
    void setNumJugadores(int n) { numJugadores = n; }
    bool esPuntoDePoder(int fila, int columna);
    bool esCasillaOscilante(int fila, int columna);

};