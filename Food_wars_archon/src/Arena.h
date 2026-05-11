#pragma once
#include "Comida.h"
#include "Proyectil.h"
#include <vector>
#include <cmath>
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

    // Vectores de velocidad para movimiento suave
    float velJ1_x, velJ1_y;
    float velJ2_x, velJ2_y;

    // Controladores booleanos de estado de teclas
    bool w_pulsada, a_pulsada, s_pulsada, d_pulsada;
    bool arriba_pulsada, abajo_pulsada, izq_pulsada, der_pulsada;

    // Variables de proyeciles 
    std::vector<Proyectil> proyectiles; // El vector que gestiona los disparos activos

    // Sistema de feedback de impacto 
    float hitTimerJ1; // Temporizador para el parpadeo de daño J1
    float hitTimerJ2; // Temporizador para el parpadeo de daño J2
    const float HIT_DURATION = 0.15f; // Duración del efecto visual en segundos

    // Sistema de vel ataque 
    float cooldownJ1; // Tiempo restante para que J1 pueda volver a disparar
    float cooldownJ2; // Tiempo restante para que J2 pueda volver a disparar

public:
    Arena(); // Constructor

    // Función que conecta el Tablero con la Arena (NO TOCAR)
    void iniciarCombate(Comida* atacante, Comida* defensor);
    int getGanadorCombate() { return ganadorCombate; }

    // Funciones tuyas 
    void actualiza();
    void dibuja();

    void teclado(unsigned char tecla); // Para disparar (espacio)
    void tecladoUp(unsigned char tecla); // Para soltar tecla
    
    void teclasEspeciales(int tecla);  // Para mover (flechas)
    void teclasEspecialesUp(int tecla);  // Para soltar flecha
    //tienes que crear el modo dos jugadores, que uno se mueve con WASD y otro con las flechas y se pelean 
};

