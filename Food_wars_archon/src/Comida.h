#pragma once
#include "Definiciones.h"
#include "ETSIDI.h"


class Comida {
public: // Lo ponemos público de momento para facilitaros el trabajo a los 3

   
    int fila, columna;   // Su posición actual en matriz 9x9
    Bando bando;         // A qué equipo pertenece
    TipoFicha tipo;      // Qué pieza exacta es
    int rangoMovimiento; // Cuántas casillas puede avanzar
    bool encarcelada = false; //hechizo
    //  (Combate) 
    int vidaMax;
    int vidaActual;
    int ataque;

    bool volador = false;
    bool estaEncarcelada = false;
    bool seleccionadaParaHechizo = false;
    
    //Sprites
    ETSIDI::SpriteSequence* sprite;

    // Constructor
    Comida(Bando b, TipoFicha t, int f, int c);
    ~Comida();
    // --- MÉTODOS ---
    bool intentarMover(int nuevaFila, int nuevaColumna); // Tu función
    void recibirDano(int cantidad);                      // Función del Gladiador
    void dibuja(float xMin, float yMin, float lado); 
    void actualiza(float t);
    Bando getBando() { return bando; }

};
