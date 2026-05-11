#include "Arena.h"
#include "freeglut.h" 
Arena::Arena() {
    ganadorCombate = 0;
}

// ZONA DE CONEXIÓN CON EL TABLERO (ESTO LO GESTIONAMOS NOSOTROS  NO TOCAR)

void Arena::iniciarCombate(Comida* atacante, Comida* defensor) {
    jugador1 = atacante;
    jugador2 = defensor;
    ganadorCombate = 0; // Reseteamos el ganador

    // Posiciones iniciales de ocmabte
    posJ1_x = -5.0f; posJ1_y = 0.0f;
    posJ2_x = 5.0f;  posJ2_y = 0.0f;
}

// ZONA DE COMBATE 


void Arena::actualiza() {
    // Si ya hay ganador, no calculamos nada más
    if (ganadorCombate != 0) return;

    //  1. Mueve los proyectiles

    //  2. Comprueba las colisiones (Hitboxes)

    // 3. Comprobar quién ha muerto (ESTO DÉJALO ASÍ)
    if (jugador1->vidaActual <= 0) {
        ganadorCombate = 2; // Gana el defensor
    }
    else if (jugador2->vidaActual <= 0) {
        ganadorCombate = 1; // Gana el atacante
    }
    // en comida.cpp al final hay una funcion que es recibir daño, utiliza esa. Ademas todas las caracteristicas de cada tipo de fihca estan en commida
    // añade las que sean necesarias, ya qeu cada ficha tne una velocidad, una vida y un tipo de ataque
    // hay algunas que disparan o tras que son cuerpo a cuerpo
    // meter cooldown como caracteristica de alguna fichas para que depende del tipo pueda disparar mas o menos rapido
}

void Arena::dibuja() {
    //  1. Dibuja el fondo de la arena
    // habra que meter obstaculos en la arena

    // 2. Dibuja al Jugador  usando posJ1_x y posJ1_y
    // Puedes usar jugador1->getTipo() para saber si dibujar una manzana o un plátano.
    // cada personaje tiene sus graficos en comida, se podra hacer que no te haga falta volver a poner todo si no llamar a comida (nose)


    //  4. Dibuja los proyectiles y las barras de vida

    //animacion de cuando recibe daño cada eprsonaje o que parpadea con un color rojo con opacidad reducida
}

void Arena::teclado(unsigned char tecla) {
    //  Controles de disparo o habilidades
}

void Arena::teclasEspeciales(int tecla) {
    // Controles de movimiento
    // meter movimiento por booleanos para tener un mov suave
}