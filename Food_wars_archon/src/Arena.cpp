#include "Arena.h"
#include "freeglut.h" 
#include <cmath>
#include <algorithm> // para max 
Arena::Arena() {
    ganadorCombate = 0;

    jugador1 = nullptr; // Inicialización de seguridad
    jugador2 = nullptr;

    // Inicializamos las teclas sueltas, lo he añadido ya que tengo que forzar las teclkas al 'false' para que no haya lectura mala al empezar 
    // puede haber basura electromagnética que haga que piense que la telca esta pulsada y el muñeco se moveria solo nada mas empezar 
    w_pulsada = a_pulsada = s_pulsada = d_pulsada = false;
    arriba_pulsada = abajo_pulsada = izq_pulsada = der_pulsada = false;

    // Inicialización de temporizadores de feedback
    hitTimerJ1 = 0.0f;
    hitTimerJ2 = 0.0f;

    // Inicializamos cooldowns a cero
    cooldownJ1 = cooldownJ2 = 0.0f;
}

// ZONA DE CONEXIÓN CON EL TABLERO (ESTO LO GESTIONAMOS NOSOTROS  NO TOCAR)

void Arena::iniciarCombate(Comida* atacante, Comida* defensor) {
    jugador1 = atacante;
    jugador2 = defensor;
    ganadorCombate = 0; // Reseteamos el ganador

    // Posiciones iniciales de combate (Adaptadas a 800x600)
    posJ1_x = 200.0f; posJ1_y = 300.0f; // J1 a la izquierda
    posJ2_x = 600.0f; posJ2_y = 300.0f; // J2 a la derecha

    // He tenido que cambair las posiciones porque sino me aparecian fuera del tablero posible para la camara con 800x600. 
    // Las dejo igual las de antes comentadas por si las necesitais. 
    //// Posiciones iniciales de ocmabte
    //posJ1_x = -5.0f; posJ1_y = 0.0f;
    //posJ2_x = 5.0f;  posJ2_y = 0.0f;

    // Reseteamos velocidades, añadido tambien 
    velJ1_x = 0.0f; velJ1_y = 0.0f;
    velJ2_x = 0.0f; velJ2_y = 0.0f;

    // Reseteamos las teclas por si entraste al combate pulsando una
    w_pulsada = a_pulsada = s_pulsada = d_pulsada = false;
    arriba_pulsada = abajo_pulsada = izq_pulsada = der_pulsada = false;

    // Limpiamos los proyectiles de combates anteriores
    proyectiles.clear();

    // Reseteamos feedback visual
    hitTimerJ1 = 0.0f;
    hitTimerJ2 = 0.0f;
}

// ZONA DE COMBATE 


void Arena::actualiza() {
    // Si ya hay ganador, no calculamos nada más
    if (ganadorCombate != 0) return;

    float t = 0.025f; // Diferencial de tiempo (dt)
    
    // ACTUALIZACIÓN DE CRONOMETROS DE VEL ATAQUE
    if (cooldownJ1 > 0) cooldownJ1 -= t;
    if (cooldownJ2 > 0) cooldownJ2 -= t;

    // Velocidad de movimiento
    float v1 = jugador1->velocidadMov; // Será 250
    float v2 = jugador2->velocidadMov; // Será 250

    // 1. CÁLCULO DE VELOCIDAD JUGADOR 1 (WASD)
    velJ1_x = 0; velJ1_y = 0;
    
    if (w_pulsada) velJ1_y = v1;
    if (s_pulsada) velJ1_y = -v1;
    if (a_pulsada) velJ1_x = -v1;
    if (d_pulsada) velJ1_x = v1;

    // 2. CÁLCULO DE VELOCIDAD JUGADOR 2 (FLECHAS)
    velJ2_x = 0; velJ2_y = 0;
    if (arriba_pulsada) velJ2_y = v2;
    if (abajo_pulsada) velJ2_y = -v2;
    if (izq_pulsada) velJ2_x = -v2;
    if (der_pulsada) velJ2_x = v2;

    // 3. MOVIMIENTO Y COLISIONES JUGADOR 1
    posJ1_x += velJ1_x * t;
    posJ1_y += velJ1_y * t;
    // Límites para que no se salga de los 800x600
    if (posJ1_x < 25)  posJ1_x = 25;
    if (posJ1_x > 775) posJ1_x = 775;
    if (posJ1_y < 25)  posJ1_y = 25;
    if (posJ1_y > 575) posJ1_y = 575;

    // 4. MOVIMIENTO Y COLISIONES JUGADOR 2
    posJ2_x += velJ2_x * t;
    posJ2_y += velJ2_y * t;
    if (posJ2_x < 25)  posJ2_x = 25;
    if (posJ2_x > 775) posJ2_x = 775;
    if (posJ2_y < 25)  posJ2_y = 25;
    if (posJ2_y > 575) posJ2_y = 575;

    //  5. MUEVE LOS PROYECTILES 
    for (int i = 0; i < proyectiles.size(); i++) {
        proyectiles[i].mueve(t);
        // Borramos si sale de la pantalla (0-800)
        if (proyectiles[i].posicion.x < 0 || proyectiles[i].posicion.x > 800 ||
            proyectiles[i].distanciaRecorrida >= proyectiles[i].alcanceMaximo) {
            proyectiles.erase(proyectiles.begin() + i);
            i--;
            continue;
        }

        // COmprobacion de colisiones 
        // Colisión contra Jugador 1
        float distJ1 = sqrtf(powf(proyectiles[i].posicion.x - posJ1_x, 2) + powf(proyectiles[i].posicion.y - posJ1_y, 2));
        if (distJ1 < (proyectiles[i].radio + 20.0f)) {
            jugador1->recibirDano(jugador2->ataque); // Quitamos la vida del ataque del jugador2 
            proyectiles.erase(proyectiles.begin() + i);
            hitTimerJ1 = 0.15f;
            i--;
            continue;
        }

        // Colisión contra Jugador 2
        float distJ2 = sqrtf(powf(proyectiles[i].posicion.x - posJ2_x, 2) + powf(proyectiles[i].posicion.y - posJ2_y, 2));
        if (distJ2 < (proyectiles[i].radio + 20.0f)) {
            jugador2->recibirDano(jugador1->ataque);
            proyectiles.erase(proyectiles.begin() + i);
            hitTimerJ2 = 0.15f;
            i--;
        }
    }

    // 6. ACTUALIZACION TEMPORIZADORES DEL DAÑO 
    if (hitTimerJ1 > 0) hitTimerJ1 -= t;
    if (hitTimerJ2 > 0) hitTimerJ2 -= t;

    // 7. Comprobar quién ha muerto (ESTO DÉJALO ASÍ)
    if (jugador1->vidaActual <= 0) {
        ganadorCombate = 2; // Gana el defensor
    }
    else if (jugador2->vidaActual <= 0) {
        ganadorCombate = 1; // Gana el atacante
    }
    // en comida.cpp al final hay una funcion que es recibir daño, utiliza esa. Ademas todas las caracteristicas de cada tipo de ficha estan en comida
    // añade las que sean necesarias, ya qeu cada ficha tne una velocidad, una vida y un tipo de ataque
    // hay algunas que disparan o tras que son cuerpo a cuerpo
    // meter cooldown como caracteristica de alguna fichas para que depende del tipo pueda disparar mas o menos rapido HECHO
}

void Arena::dibuja() {
    //  1. Dibuja el fondo de la arena
    // habra que meter obstaculos en la arena

    // Puedes usar jugador1->getTipo() para saber si dibujar una manzana o un plátano.
    // cada personaje tiene sus graficos en comida, se podra hacer que no te haga falta volver a poner todo si no llamar a comida (nose)
    
    // No he usado jugador1->getTipo, ya que la arena simlemente da la orden cediendo el control al objeto
    // Al hacer esto, la propia ficha recibe sus nuevas coordenadas x e y y ella misma sabe qué imagen tiene que pintar en la pantalla.

    // 2. Dibuja al Jugador  usando posJ1_x y posJ1_y
    // Dibujamos al Jugador 1 (Atacante)
    if (jugador1 != NULL) {
        // Centramos el dibujo restando la mitad del tamaño (20) a la posición central
        jugador1->dibuja(posJ1_x - 20, posJ1_y - 20, 40.0f);
    }
    // ANIMACIÓN DE DAÑO J1: Parpadeo Rojo ---
    if (hitTimerJ1 > 0) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        // Rojo transparente sobre el personaje
        glColor4f(1.0f, 0.0f, 0.0f, 0.4f + (0.3f * (hitTimerJ1 / HIT_DURATION)));
        glBegin(GL_POLYGON);
        glVertex2f(posJ1_x - 22, posJ1_y - 22);
        glVertex2f(posJ1_x + 22, posJ1_y - 22);
        glVertex2f(posJ1_x + 22, posJ1_y + 22);
        glVertex2f(posJ1_x - 22, posJ1_y + 22);
        glEnd();

        glEnable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f); // Reset color
        glEnable(GL_TEXTURE_2D);
    }
    

    // Dibujamos al Jugador 2 (Defensor)
    if (jugador2 != NULL) {
        jugador2->dibuja(posJ2_x - 20, posJ2_y - 20, 40.0f);
    }
    // ANIMACION DAÑO JUGADOR 2
    if (hitTimerJ2 > 0) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        glColor4f(1.0f, 0.0f, 0.0f, 0.4f + (0.3f * (hitTimerJ2 / HIT_DURATION)));
        glBegin(GL_POLYGON);
        glVertex2f(posJ2_x - 22, posJ2_y - 22);
        glVertex2f(posJ2_x + 22, posJ2_y - 22);
        glVertex2f(posJ2_x + 22, posJ2_y + 22);
        glVertex2f(posJ2_x - 22, posJ2_y + 22);
        glEnd();

        glEnable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
    }

    //  4. Dibuja los proyectiles y las barras de vida
    for (auto& p : proyectiles) {
        p.dibuja(); 
    }

    // barra parpadea con un color rojo con opacidad reducida ya integrado arriba
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Cálculo basado en la vida real de cada objeto
    float porcentajeJ1 = (float)jugador1->vidaActual / (float)jugador1->vidaMax;
    float longiBarraJ1 = (std::max)(0.0f, porcentajeJ1 * 200.0f); // 200px es el ancho máximo

    // Barra de Vida Jugador 1 (Atacante)
    glColor3f(0.0f, 1.0f, 0.0f); // Verde para J1
    glBegin(GL_POLYGON);
    glVertex2f(50, 570);
    glVertex2f(50 + longiBarraJ1, 570);
    glVertex2f(50 + longiBarraJ1, 585);
    glVertex2f(50, 585);
    glEnd();

    // Barra de Vida Jugador 2 (Defensor - Derecha)
    float porcentajeJ2 = (float)jugador2->vidaActual / (float)jugador2->vidaMax;
    float longiBarraJ2 = (std::max)(0.0f, porcentajeJ2 * 200.0f);

    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para J2
    glBegin(GL_POLYGON);
    glVertex2f(750, 570);
    glVertex2f(750 - longiBarraJ2, 570);
    glVertex2f(750 - longiBarraJ2, 585);
    glVertex2f(750, 585);
    glEnd();

    // (Opcional) Marco blanco para las barras para saber el máximo
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP); glVertex2f(50, 570); glVertex2f(250, 570); glVertex2f(250, 585); glVertex2f(50, 585); glEnd();
    glBegin(GL_LINE_LOOP); glVertex2f(550, 570); glVertex2f(750, 570); glVertex2f(750, 585); glVertex2f(550, 585); glEnd();
    glEnable(GL_TEXTURE_2D); // Dejamos estados listos
}

void Arena::teclado(unsigned char tecla) {
    //  Controles de movimiento jugador1
    if (tecla == 'w' || tecla == 'W') w_pulsada = true;
    if (tecla == 'a' || tecla == 'A') a_pulsada = true;
    if (tecla == 's' || tecla == 'S') s_pulsada = true;
    if (tecla == 'd' || tecla == 'D') d_pulsada = true;

    //  Controles de disparo o habilidades
    if (tecla == ' ' && cooldownJ1 <= 0) { // JUGADOR 1 DISPARA CON ESPACIO
        if (jugador1->rangoAtaque > 0.0f) {
            Proyectil p;
            p.posicion.x = posJ1_x + 20;
            p.posicion.y = posJ1_y;
            p.velocidad.x = 400.0f; // Disparo hacia la derecha
            p.alcanceMaximo = jugador1->rangoAtaque; // Le pasamos el límite a la bala
            p.distanciaRecorrida = 0.0f;

            proyectiles.push_back(p);
            
        }
        else {
            // ES MELEE: Comprobamos si el rival está cerca en el momento del impacto
            float distJugadores = sqrtf(powf(posJ1_x - posJ2_x, 2) + powf(posJ1_y - posJ2_y, 2));
            if (distJugadores < 40.0f) {
                jugador2->recibirDano(jugador1->ataque);
                hitTimerJ2 = 0.15f; // Animación de daño al rival
            }
            // Si el enemigo no está cerca, da un puñetazo al aire (no hace daño pero gasta el turno)
        }

        // Seteamos el tiempo de espera según la pieza
        cooldownJ1 = jugador1->cadencia;
    }

    if (tecla == 13 && cooldownJ2 <= 0) { // JUGADOR 2 DISPARA CON ENTER (ASCII 13)
        if (jugador2->rangoAtaque > 0.0f) {
            Proyectil p;
            p.posicion.x = posJ2_x - 20;
            p.posicion.y = posJ2_y;
            p.velocidad.x = -400.0f; // Disparo hacia la izquierda
            p.alcanceMaximo = jugador2->rangoAtaque;
            p.distanciaRecorrida = 0.0f;

            proyectiles.push_back(p);  
        }
        else {
            // ES MELEE: Comprobamos si el rival está cerca en el momento del impacto
            float distJugadores = sqrtf(powf(posJ1_x - posJ2_x, 2) + powf(posJ1_y - posJ2_y, 2));
            if (distJugadores < 40.0f) {
                jugador1->recibirDano(jugador2->ataque);
                hitTimerJ1 = 0.15f; // Animación de daño al rival
            }
        }
        cooldownJ2 = jugador2->cadencia;
    }
}

void Arena::tecladoUp(unsigned char tecla) {
    if (tecla == 'w' || tecla == 'W') w_pulsada = false;
    if (tecla == 'a' || tecla == 'A') a_pulsada = false;
    if (tecla == 's' || tecla == 'S') s_pulsada = false;
    if (tecla == 'd' || tecla == 'D') d_pulsada = false;
}

void Arena::teclasEspeciales(int tecla) {
    // Controles de movimiento jugador2
    if (tecla == GLUT_KEY_UP)    arriba_pulsada = true;
    if (tecla == GLUT_KEY_DOWN)  abajo_pulsada = true;
    if (tecla == GLUT_KEY_LEFT)  izq_pulsada = true;
    if (tecla == GLUT_KEY_RIGHT) der_pulsada = true;
}

void Arena::teclasEspecialesUp(int tecla) {
    if (tecla == GLUT_KEY_UP)    arriba_pulsada = false;
    if (tecla == GLUT_KEY_DOWN)  abajo_pulsada = false;
    if (tecla == GLUT_KEY_LEFT)  izq_pulsada = false;
    if (tecla == GLUT_KEY_RIGHT) der_pulsada = false;
}