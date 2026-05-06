#include "Mundo.h"
#include "freeglut.h"
#include <iostream>


void Mundo::inicializa() {
    // 1. Configurar la vista (Cámara)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Mapea tus coordenadas a la ventana

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2. Configurar estados iniciales
    estadoActual = MENU_PRINCIPAL;
    infoActual = NINGUNA;

    // 3. CONFIGURACIÓN DE TRANSPARENCIA (Punto 3 que preguntabas)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Mundo::dibuja() {
    // 1. Resetear el Viewport al tamaño actual de la ventana
    int ancho_ventana = glutGet(GLUT_WINDOW_WIDTH);
    int alto_ventana = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, ancho_ventana, alto_ventana);

    // 2. Forzar que nuestro lienzo sea SIEMPRE 800x600
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    switch (estadoActual) {
    case MENU_PRINCIPAL:
        interfaz.dibujaMenu();
        break;
    case SELECCION_BANDO:
        interfaz.dibujaSeleccion();
        // --- LÓGICA DE INFO SUPERPUESTA ---
        // Se dibuja solo si estamos en SELECCION_BANDO y infoActual no es NINGUNA
        if (infoActual == INFO_HEALTHY) {
            interfaz.dibujaPopUp("BROCOLI GUERRERO", "Vida: 120 | Ataque: Vitaminas\nDefensa: Fibra reforzada", true);
        }
        else if (infoActual == INFO_JUNK) {
            interfaz.dibujaPopUp("BURGER BOSS", "Vida: 100 | Ataque: Grasa saturada\nEspecial: Lluvia de Ketchup", false);
        }
        break;
    case INSTRUCCIONES:
        interfaz.dibujaInstrucciones();
        break;
    case TABLERO:
        tablero.dibuja(); // La Persona 2 trabaja aquí
        break;
    case ARENA:
       jugador1.dibuja();   //Dibuja el cuadrado verde de CHefMichelin
       jugador2.dibuja();   // Dibuja cuadrado rojo de ReyHamburguesa

       for (Proyectil& p : disparos) { //Pinta todos los proyectiles 
           p.dibuja();
       }
       break;
    case PAUSA:
        interfaz.dibujaPausa();
        break;
    case GAMEOVER:
        interfaz.dibujaFinal();
        break;
    }
    glutSwapBuffers();
}


void Mundo::gestionaRaton(int boton, int estado, int x, int y) {
    if (estadoActual == TABLERO) {

        if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {

            // 3. Le pasamos las coordenadas al tablero y leemos su respuesta
            bool hayCombate = tablero.gestionRaton(x, y);

            // 4. Si el tablero detecta un ataque (devuelve true), encendemos la arena
            if (hayCombate) {
                estadoActual = ARENA;

                // Opcional: Reiniciamos las posiciones de los luchadores para que no 
                // aparezcan pegados si es el segundo combate de la partida
                jugador1.posicion.x = 200.0f;
                jugador1.posicion.y = 300.0f;
                jugador2.posicion.x = 600.0f;
                jugador2.posicion.y = 300.0f;
            }
            glutPostRedisplay();
        }
    }
}

void Mundo::teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case '1':
        estadoActual = MENU_PRINCIPAL;
        break;
    case '2':
        estadoActual = SELECCION_BANDO;
        break;
    case '3':
        estadoActual = TABLERO;
        break;
    case '4': // LO HE METIO PARA IR COMPROBANDO CONTROLES 
        estadoActual = ARENA;
        break;
        // --- CONTROLES DE INFORMACIÓN (Solo funcionan en SELECCION_BANDO) ---
    case 'h':
        if (estadoActual == SELECCION_BANDO) infoActual = INFO_HEALTHY;
        break;
    case 'j':
        if (estadoActual == SELECCION_BANDO) infoActual = INFO_JUNK;
        break;
    case 'c': // 'c' de Cerrar o Cancelar info
        infoActual = NINGUNA;
        break;
    case 27: // Tecla ESC para salir
        exit(0);
        break;
    }
        //  LÓGICA DE MOVIMIENTO PARA LA ARENA 
        // Solo permito modificar la velocidad si estamos en pleno combate
    if (estadoActual == ARENA) {
        switch (tecla) {
            // Control Jugador1
        case 'a': case 'A':
            jugador1.velocidad.x = -150.0f;
            break;
        case 'd': case 'D':
            jugador1.velocidad.x = 150.0f;
            break;
        case 'w': case 'W':
            jugador1.velocidad.y = 150.0f;
            break;
        case 's': case 'S':
            jugador1.velocidad.y = -150.0f;
            break;
        case ' ':
            jugador1.velocidad.x = 0; // freno de emergencia
            jugador1.velocidad.y = 0;
            break;

            // Control Jugador2
        case 'j': case 'J':
            jugador2.velocidad.x = -150.0f;
            break;
        case 'l': case 'L':
            jugador2.velocidad.x = 150.0f;
            break;
        case 'i': case 'I':
            jugador2.velocidad.y = 150.0f;
            break;
        case 'k': case 'K':
            jugador2.velocidad.y = -150.0f;
            break;

        case 'e': case 'E': {
            Proyectil nuevoDisparo;
            nuevoDisparo.posicion = jugador1.posicion; // Nace en el centro del jugador
            nuevoDisparo.velocidad.x = 300.0f;         // Sale disparado hacia la derecha
            nuevoDisparo.velocidad.y = 0.0f;

            disparos.push_back(nuevoDisparo); // Metemos el disparo en la memoria dinámica
            break;
        }
        case 'u': case 'U': {
            Proyectil nuevoDisparo;
            nuevoDisparo.posicion = jugador2.posicion;
            nuevoDisparo.velocidad.x = -300.0f; // Disparo hacia la IZQUIERDA
            nuevoDisparo.velocidad.y = 0.0f;

            disparos.push_back(nuevoDisparo);
            break;
            }
        }
    }
}

void Mundo::teclaUp(unsigned char tecla, int x, int y) {
    if (estadoActual == ARENA) {
        switch (tecla) {
            // Freno Jugador 1
        case 'a': case 'A': case 'd': case 'D':
            jugador1.velocidad.x = 0; // Freno el eje X al soltar A o D
            break;
        case 'w': case 'W': case 's': case 'S':
            jugador1.velocidad.y = 0; // Freno el eje Y al soltar W o S
            break;
            
            // Freno Jugador 2
        case 'j': case 'J': case 'l': case 'L':
            jugador2.velocidad.x = 0;
            break;
        case 'i': case 'I': case 'k': case 'K':
            jugador2.velocidad.y = 0;
            break;
        }
    }
}


// Esta funcion es el bucle de fisicas
void Mundo::update() {
    float t = 0.025f; // dt

    jugador1.mueve(t, 800.0f, 600.0f); // Movimiento del Jugaoador 1

    // Ejecutamos la IA del Rey Hamburguesa pasándole la Y del Chef
    bool botQuiereDisparar = jugador2.piensaYMueve(jugador1.posicion.y);
    jugador2.mueve(t, 800.0f, 600.0f);

    // --- GESTIÓN DEL ARMA DEL BOT ---
    // Restamos el tiempo transcurrido al cronómetro del arma
    if (jugador2.tiempoRecarga > 0.0f) {
        jugador2.tiempoRecarga -= t;
    }
    // Si la IA decide disparar Y el arma ya se ha enfriado:
    if (botQuiereDisparar && jugador2.tiempoRecarga <= 0.0f) {
        Proyectil nuevoDisparo;
        nuevoDisparo.posicion = jugador2.posicion;
        nuevoDisparo.velocidad.x = -300.0f; // Dispara hacia la izquierda
        nuevoDisparo.velocidad.y = 0.0f;
        disparos.push_back(nuevoDisparo);

        jugador2.tiempoRecarga = 1.0f; // Reiniciamos el cooldown a 1 segundo
    }

    // Sistema de getion de memoria y colisiones 
    for (auto it = disparos.begin(); it != disparos.end(); ) {
        it->mueve(t);

        bool impacto = false;

        // A) SI EL PROYECTIL VA A LA DERECHA (Viene del Chef -> Ataca al Rey)
        if (it->velocidad.x > 0) {
            float dx = it->posicion.x - jugador2.posicion.x;
            float dy = it->posicion.y - jugador2.posicion.y;
            float distCuadrada = (dx * dx) + (dy * dy);
            float sumaRadios = it->radio + jugador2.radio;

            if (distCuadrada < (sumaRadios * sumaRadios)) {
                jugador2.vida -= jugador1.ataque;
                impacto = true;
                std::cout << "¡Impacto en el Rey! Vida restante: " << jugador2.vida << std::endl;
            }
        }
        // B) SI EL PROYECTIL VA A LA IZQUIERDA (Viene del Rey -> Ataca al Chef)
        else if (it->velocidad.x < 0) {
            float dx = it->posicion.x - jugador1.posicion.x;
            float dy = it->posicion.y - jugador1.posicion.y;
            float distCuadrada = (dx * dx) + (dy * dy);
            float sumaRadios = it->radio + jugador1.radio;

            if (distCuadrada < (sumaRadios * sumaRadios)) {
                jugador1.vida -= jugador2.ataque;
                impacto = true;
                std::cout << "¡Impacto en el Chef! Vida restante: " << jugador1.vida << std::endl;
            }
        }

        // C) GESTIÓN DE BORRADO (Por impacto o por salir de pantalla)
        if (impacto || it->posicion.x > 800.0f || it->posicion.x < 0.0f ||
            it->posicion.y > 600.0f || it->posicion.y < 0.0f) {
            it = disparos.erase(it);
        }
        else {
            it++;
        }
    }

    // Condicion de fin de combate
    // Usamos <= 0 por seguridad científica: un impacto podría dejar la vida en -10
    if (jugador2.vida <= 0) {
        std::cout << "¡VICTORIA PARA EL BANDO SALUDABLE!" << std::endl;

        // Volvemos al Tablero
        estadoActual = TABLERO;

        // Opcional: Resetear la vida para el siguiente combate
        jugador2.vida = 150;
        jugador1.vida = 150;
    }
    else if (jugador1.vida <= 0) {
        std::cout << "¡EL REY DE LA HAMBURGUESA HA GANADO!" << std::endl;

        estadoActual = TABLERO;

        jugador1.vida = 150;
        jugador2.vida = 150;
    }

}