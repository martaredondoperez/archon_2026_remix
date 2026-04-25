#pragma once

// Usamos enum para que el código sea muy fácil de leer
enum Bando { SALUDABLE, FAST_FOOD };
enum TipoFicha { LIDER, ESPECIAL, VOLADORA, PESADA, DISTANCIA };

class Comida {
public: // Lo ponemos público de momento para facilitaros el trabajo a los 3

   
    int fila, columna;   // Su posición actual en matriz 9x9
    Bando bando;         // A qué equipo pertenece
    TipoFicha tipo;      // Qué pieza exacta es
    int rangoMovimiento; // Cuántas casillas puede avanzar

    //  (Combate) 
    int vidaMax;
    int vidaActual;
    int ataque;

    // Constructor
    Comida(Bando b, TipoFicha t, int f, int c);

    // --- MÉTODOS ---
    bool intentarMover(int nuevaFila, int nuevaColumna); // Tu función
    void recibirDano(int cantidad);                      // Función del Gladiador
    void dibuja(float xMin, float yMin, float lado);     
};
