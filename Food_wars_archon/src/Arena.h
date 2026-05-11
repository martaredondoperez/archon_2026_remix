#pragma once
#include "Comida.h"
class Arena
{
private:
    // Punteros a las fichas reales del tablero (NO TOCAR)
    Comida* jugador1; // El que ataca
    Comida* jugador2; // El que defiende

    // Variables de estado del combate
    int ganadorCombate; // 0 = Peleando, 1 = Gana J1, 2 = Gana J2

    //  VARIABLES DE LA ARENA (Aquí puedes añadir lo que quieras) 
    float posJ1_x, posJ1_y;
    float posJ2_x, posJ2_y;

    //  añada aquí tus variables de proyectiles, temporizadores, etc.

public:
    Arena(); // Constructor

    // Función que conecta el Tablero con la Arena (NO TOCAR)
    void iniciarCombate(Comida* atacante, Comida* defensor);
    int getGanadorCombate() { return ganadorCombate; }

    // Funciones tuyas 
    void actualiza();
    void dibuja();
    void teclado(unsigned char tecla); // Para disparar (espacio)
    void teclasEspeciales(int tecla);  // Para mover (flechas)
    //tienes que crear el modo dos jugadores, que uno se mueve con WASD y otro con las flechas y se pelean 
};

