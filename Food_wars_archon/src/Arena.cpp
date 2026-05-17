#include "Arena.h"
#include "freeglut.h" 
#include <cmath>
#include <algorithm> // para max 
Arena::Arena() : fondo_arena("imagenes/mantel_fondo_arena.png") {

    fondo_arena.setPos(0, 0);
    fondo_arena.setSize(800, 600);
    fondo_arena.setCenter(0, 0);
    // Sprites disparos
    sprite_limon = new ETSIDI::Sprite("imagenes/chorro_limon.png", 0, 0, 30, 13);
    sprite_ketchup = new ETSIDI::Sprite("imagenes/chorro_ketchup.png", 0, 0, 30, 13);

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

    // Zona de colisiones objetos del fondo 
    obstaculos.clear();
    cajas_inclinadas.clear();

    // Caja Junk Food: Centro en (280, 380), tamaño 220x150, girada unos 30 grados
    cajas_inclinadas.push_back({ 255.0f, 405.0f, 200.0f, 140.0f, 30.0f });

    // 2. Caja Healthy Food (Centro estimado en la parte inferior derecha, ángulo negativo)
    cajas_inclinadas.push_back({ 650.0f, 195.0f, 140.0f, 230.0f, -10.0f });

    // Vasos
    obstaculos.push_back({ 105.0f, 155.0f, 150.0f, 200.0f }); // Vaso abajo izquierda ok
    obstaculos.push_back({ 85.0f, 135.0f, 440.0f, 490.0f }); // Vaso arriba izquierda ok
    obstaculos.push_back({ 380.0f, 430.0f, 410.0f, 460.0f }); // Vaso arriba centro ok
    obstaculos.push_back({ 615.0f, 665.0f, 440.0f, 495.0f }); // Vaso arriba drch
    obstaculos.push_back({ 460.0f, 515.0f, 230.0f, 310.0f }); // Vaso abajo derecha

}

// ZONA DE CONEXIÓN CON EL TABLERO (ESTO LO GESTIONAMOS NOSOTROS  NO TOCAR)

void Arena::iniciarCombate(Comida* atacante, Comida* defensor) {
    jugador1 = atacante;
    jugador2 = defensor;
    ganadorCombate = 0; // Reseteamos el ganador

    // Posiciones iniciales de combate (Adaptadas a 800x600)
    posJ1_x = 200.0f; posJ1_y = 200.0f; // J1 a la izquierda
    posJ2_x = 600.0f; posJ2_y = 400.0f; // J2 a la derecha

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
    float futura_x1 = posJ1_x + velJ1_x * t;
    float futura_y1 = posJ1_y + velJ1_y * t;
    
    // Colision vasos eje x
    bool colisionX1 = false;
    for (const auto& obs : obstaculos) {
        if (futura_x1 + 20 > obs.x_min && futura_x1 - 20 < obs.x_max &&
            posJ1_y + 20 > obs.y_min && posJ1_y - 20 < obs.y_max) {
            colisionX1 = true; break;
        }
    }
    // Colision cajas eje X
    for (const auto& caja : cajas_inclinadas) {
        float rad = caja.angulo * 3.14159f / 180.0f; // Pasar a radianes
        // Vector distancia desde el centro de la caja
        float dx = futura_x1 - caja.cx;
        float dy = posJ1_y - caja.cy;

        // Rotación matemática inversa
        float local_x = dx * cos(rad) + dy * sin(rad);
        float local_y = -dx * sin(rad) + dy * cos(rad);

        // Comprobar si entra en los límites (mitad del ancho/alto + radio del jugador)
        if (std::abs(local_x) < (caja.ancho / 2.0f + 20.0f) &&
            std::abs(local_y) < (caja.alto / 2.0f + 20.0f)) {
            colisionX1 = true; break;
        }
    }
    if (!colisionX1) posJ1_x = futura_x1;

    // colision vasos eje y
    bool colisionY1 = false;
    for (const auto& obs : obstaculos) {
        if (posJ1_x + 20 > obs.x_min && posJ1_x - 20 < obs.x_max &&
            futura_y1 + 20 > obs.y_min && futura_y1 - 20 < obs.y_max) {
            colisionY1 = true; break;
        }
    }
    // Colision cajas eje y
    for (const auto& caja : cajas_inclinadas) {
        float rad = caja.angulo * 3.14159f / 180.0f;
        float dx = posJ1_x - caja.cx; // Cuidado aquí: usamos posJ1_x actual
        float dy = futura_y1 - caja.cy; // Cuidado aquí: usamos futura_y1

        float local_x = dx * cos(rad) + dy * sin(rad);
        float local_y = -dx * sin(rad) + dy * cos(rad);

        if (std::abs(local_x) < (caja.ancho / 2.0f + 20.0f) &&
            std::abs(local_y) < (caja.alto / 2.0f + 20.0f)) {
            colisionY1 = true; break;
        }
    }
    if (!colisionY1) posJ1_y = futura_y1;

    // Límites para que no se salga de los 800x600
    if (posJ1_x < 25)  posJ1_x = 25;
    if (posJ1_x > 775) posJ1_x = 775;
    if (posJ1_y < 25)  posJ1_y = 25;
    if (posJ1_y > 555) posJ1_y = 555;

    // 4. MOVIMIENTO Y COLISIONES JUGADOR 2
    float futura_x2 = posJ2_x + velJ2_x * t;
    float futura_y2 = posJ2_y + velJ2_y * t;

    bool colisionX2 = false;
    for (const auto& obs : obstaculos) {
        if (futura_x2 + 20 > obs.x_min && futura_x2 - 20 < obs.x_max &&
            posJ2_y + 20 > obs.y_min && posJ2_y - 20 < obs.y_max) {
            colisionX2 = true; break;
        }
    }
    for (const auto& caja : cajas_inclinadas) {
        float rad = caja.angulo * 3.14159f / 180.0f;
        float dx = futura_x2 - caja.cx;
        float dy = posJ2_y - caja.cy; // Usamos pos actual en Y

        float local_x = dx * cos(rad) + dy * sin(rad);
        float local_y = -dx * sin(rad) + dy * cos(rad);

        if (std::abs(local_x) < (caja.ancho / 2.0f + 20.0f) &&
            std::abs(local_y) < (caja.alto / 2.0f + 20.0f)) {
            colisionX2 = true; break;
        }
    }
    if (!colisionX2) posJ2_x = futura_x2;

    bool colisionY2 = false;
    for (const auto& obs : obstaculos) {
        if (posJ2_x + 20 > obs.x_min && posJ2_x - 20 < obs.x_max &&
            futura_y2 + 20 > obs.y_min && futura_y2 - 20 < obs.y_max) {
            colisionY2 = true; break;
        }
    }
    for (const auto& caja : cajas_inclinadas) {
        float rad = caja.angulo * 3.14159f / 180.0f;
        float dx = posJ2_x - caja.cx; // Usamos pos actual en X
        float dy = futura_y2 - caja.cy;

        float local_x = dx * cos(rad) + dy * sin(rad);
        float local_y = -dx * sin(rad) + dy * cos(rad);

        if (std::abs(local_x) < (caja.ancho / 2.0f + 20.0f) &&
            std::abs(local_y) < (caja.alto / 2.0f + 20.0f)) {
            colisionY2 = true; break;
        }
    }
    if (!colisionY2) posJ2_y = futura_y2;

    // Limites para que no salga 
    if (posJ2_x < 25)  posJ2_x = 25;
    if (posJ2_x > 775) posJ2_x = 775;
    if (posJ2_y < 25)  posJ2_y = 25;
    if (posJ2_y > 555) posJ2_y = 555;

    //  5. MUEVE LOS PROYECTILES Y GESTIONA SU CICLO DE VIDA
    for (int i = 0; i < proyectiles.size(); i++) {
        proyectiles[i].mueve(t);

        // A. Borramos si sale de la pantalla (0-800) o supera el alcance
        if (proyectiles[i].posicion.x < 0 || proyectiles[i].posicion.x > 800 ||
            proyectiles[i].distanciaRecorrida >= proyectiles[i].alcanceMaximo) {
            proyectiles.erase(proyectiles.begin() + i);
            i--;
            continue;
        }

        // B. Comprobación de colisiones contra OBSTÁCULOS RECTANGULARES
        bool chocaObstaculo = false;
        for (const auto& obs : obstaculos) {
            if (proyectiles[i].posicion.x > obs.x_min && proyectiles[i].posicion.x < obs.x_max &&
                proyectiles[i].posicion.y > obs.y_min && proyectiles[i].posicion.y < obs.y_max) {
                chocaObstaculo = true;
                break;
            }
        }

        // C. Comprobación contra OBSTÁCULOS INCLINADOS
        if (!chocaObstaculo) {
            for (const auto& caja : cajas_inclinadas) {
                float rad = caja.angulo * 3.14159f / 180.0f;
                float dx = proyectiles[i].posicion.x - caja.cx;
                float dy = proyectiles[i].posicion.y - caja.cy;

                float local_x = dx * cos(rad) + dy * sin(rad);
                float local_y = -dx * sin(rad) + dy * cos(rad);

                if (std::abs(local_x) < (caja.ancho / 2.0f + 10.0f) &&
                    std::abs(local_y) < (caja.alto / 2.0f + 10.0f)) {
                    chocaObstaculo = true;
                    break;
                }
            }
        }

        if (chocaObstaculo) {
            proyectiles.erase(proyectiles.begin() + i);
            i--;
            continue;
        }

        // D. Colisión contra JUGADOR 1 (Solo si el proyectil es del ENEMIGO)
        if (proyectiles[i].bando != jugador1->getBando()) {
            float distJ1 = sqrtf(powf(proyectiles[i].posicion.x - posJ1_x, 2) + powf(proyectiles[i].posicion.y - posJ1_y, 2));
            if (distJ1 < (proyectiles[i].radio + 15.0f)) { // Ajustado a 15.0f para mayor precisión con el sprite
                jugador1->recibirDano(jugador2->ataque);
                proyectiles.erase(proyectiles.begin() + i);
                hitTimerJ1 = 0.15f;
                i--;
                continue; // Limpieza segura
            }
        }

        // E. Colisión contra JUGADOR 2 (Solo si el proyectil es del ENEMIGO)
        if (proyectiles[i].bando != jugador2->getBando()) {
            float distJ2 = sqrtf(powf(proyectiles[i].posicion.x - posJ2_x, 2) + powf(proyectiles[i].posicion.y - posJ2_y, 2));
            if (distJ2 < (proyectiles[i].radio + 15.0f)) {
                jugador2->recibirDano(jugador1->ataque);
                proyectiles.erase(proyectiles.begin() + i);
                hitTimerJ2 = 0.15f;
                i--;
                continue; // Evita el salto del iterador por falta de continue
            }
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
    //  1. Dibuja el fondo de la arena y sus objetos
    glEnable(GL_TEXTURE_2D);         
    fondo_arena.draw();            
    glDisable(GL_TEXTURE_2D);       
    glBindTexture(GL_TEXTURE_2D, 0); 
  
    // OBJETOS
    if (modoDebugHitboxes) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Color amarillo translúcido

        for (auto& obs : obstaculos) {
            glBegin(GL_QUADS);
            glVertex2f(obs.x_min, obs.y_min);
            glVertex2f(obs.x_max, obs.y_min);
            glVertex2f(obs.x_max, obs.y_max);
            glVertex2f(obs.x_min, obs.y_max);
            glEnd();
        }
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Las pinto VERDE para diferenciarlas
        for (const auto& caja : cajas_inclinadas) {
            glPushMatrix(); // Guardamos el sistema de coordenadas actual
            glTranslatef(caja.cx, caja.cy, 0.0f); // Movemos el eje al centro de la caja
            glRotatef(caja.angulo, 0.0f, 0.0f, 1.0f); // Giramos el eje

            // Dibujamos el cuadrado desde su propio centro
            glBegin(GL_QUADS);
            glVertex2f(-caja.ancho / 2.0f, -caja.alto / 2.0f);
            glVertex2f(caja.ancho / 2.0f, -caja.alto / 2.0f);
            glVertex2f(caja.ancho / 2.0f, caja.alto / 2.0f);
            glVertex2f(-caja.ancho / 2.0f, caja.alto / 2.0f);
            glEnd();

            glPopMatrix(); // Restauramos el sistema de coordenadas
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
    }

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
        if (p.bando == SALUDABLE) {
            sprite_limon->setPos(p.posicion.x, p.posicion.y);
            sprite_limon->draw();
        }
        else {
            sprite_ketchup->setPos(p.posicion.x, p.posicion.y);
            sprite_ketchup->draw();
        }
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
            Proyectil p(jugador1->getBando());
            p.posicion.x = posJ1_x + 20;
            p.posicion.y = posJ1_y;
            p.velocidad.x = 250.0f; // Disparo hacia la derecha
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
            Proyectil p(jugador2->getBando());
            p.posicion.x = posJ2_x - 20;
            p.posicion.y = posJ2_y;
            p.velocidad.x = -250.0f; // Disparo hacia la izquierda
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