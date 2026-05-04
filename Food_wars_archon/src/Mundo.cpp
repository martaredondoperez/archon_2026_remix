#include "Mundo.h"
#include "freeglut.h"
#include <iostream>
void Mundo::inicializa() {

    // 1. Configurar estados iniciales
    estadoActual = MENU_PRINCIPAL;
    infoActual = NINGUNA;
    numJugadores = 0;      // Empezamos sin selección
    bandoSeleccionado = 0; // Empezamos sin bando

    // 2. CONFIGURACIÓN DE TRANSPARENCIA (Punto 3 que preguntabas)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Mundo::dibuja() {

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
        interfaz.dibujaHUDJuego();
        break;
    case ARENA:
       // arena.dibuja();   // La Persona 3 trabaja aquí
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
    // De momento vacío para que no de error
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
}

void Mundo::mouse(int button, int state, int x, int y) {
    // 1. Detectar el clic izquierdo
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 2. Traducir los píxeles de la ventana a nuestro mundo de 800x600
        // Esto es lo que permite que el ratón funcione aunque maximices
        float clickX = x * (800.0f / (float)glutGet(GLUT_WINDOW_WIDTH));
        float clickY = 600.0f - (y * (600.0f / (float)glutGet(GLUT_WINDOW_HEIGHT)));
        // 3. Lógica según la pantalla en la que estemos
        switch (estadoActual) {

        case MENU_PRINCIPAL:
            // Si pulsa "1 JUGADOR" 
            if (interfaz.botonPulsado(clickX, clickY, 300, 300, 200, 60)) {
                numJugadores = 1;
                estadoActual = SELECCION_BANDO;
            }
            // Si pulsa "2 JUGADORES" 
            else if (interfaz.botonPulsado(clickX, clickY, 300, 200, 200, 60)) {
                numJugadores = 2;
                estadoActual = SELECCION_BANDO;
            }
            // Si pulsa "INSTRUCCIONES"
            else if (interfaz.botonPulsado(clickX, clickY, 300, 100, 200, 50)) {
                estadoActual = INSTRUCCIONES;
            }
            break;

        case INSTRUCCIONES:
            // Botón "VOLVER" en las instrucciones
            if (interfaz.botonPulsado(clickX, clickY, 50, 50, 120, 40)) {
                estadoActual = MENU_PRINCIPAL;
            }
            break;

        case SELECCION_BANDO:
            // Aquí podrías detectar si elige Healthy o Junk
            if (interfaz.botonPulsado(clickX, clickY, 100, 200, 250, 300)) {
                bandoSeleccionado = HEALTHY;
                estadoActual = TABLERO; // ¡A jugar!
            }
            break;
        }
    }

    // IMPORTANTE: Redibujar para que el cambio de pantalla se vea al instante
    glutPostRedisplay();
}