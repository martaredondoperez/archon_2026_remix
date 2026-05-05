#include "Mundo.h"
#include "freeglut.h"
#include <iostream>
float Mundo::mouseX = 0;
float Mundo::mouseY = 0;

void Mundo::inicializa() {

    // 1. Configurar estados iniciales
    estadoActual = MENU_PRINCIPAL;
    infoActual = NINGUNA;
    numJugadores = 0;      // Empezamos sin selección
    bandoSeleccionado = 0; // Empezamos sin bando
    pausa = false;

    // 2. CONFIGURACIÓN DE TRANSPARENCIA (Punto 3 que preguntabas)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Mundo::dibuja() {
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
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
        if (infoActual != NINGUNA) {
            interfaz.mostrarInfoBando(infoActual);
        }
        break;
    case INSTRUCCIONES:
        interfaz.dibujaInstrucciones();
        break;
    case TABLERO:
        tablero.dibuja(pausa); // La Persona 2 trabaja aquí
        interfaz.dibujaHUDJuego();
        if (pausa) {
            interfaz.dibujaPausa();
        }
        // Si hay alguna info activa (AYUDA o AJUSTES)
        if (infoActual != NINGUNA) {
            interfaz.mostrarInfoTablero(infoActual);
        }
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


void Mundo::mouse(int button, int state, int x, int y) {
    // 1. Detectar el clic izquierdo
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 2. Traducir los píxeles de la ventana a nuestro mundo de 800x600
        // Esto es lo que permite que el ratón funcione aunque maximices
        float clickX = x * (800.0f / (float)glutGet(GLUT_WINDOW_WIDTH));
        float clickY = 600.0f - (y * (600.0f / (float)glutGet(GLUT_WINDOW_HEIGHT)));
        // 3. Lógica según la pantalla en la que estemos
        if (infoActual != NINGUNA) {
            // Comprobamos si el clic cae dentro del cuadradito de la "X"
            // Ajusta estas coordenadas (630, 470) a donde dibujes tu botón de cerrar
            if (interfaz.botonPulsado(clickX, clickY, 610, 410, 30, 30)) {
                infoActual = NINGUNA;
            }
            // Si pincha fuera de la X, no hacemos nada (el pop-up sigue abierto)
            glutPostRedisplay();
            return;
        }
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
            // Si pulsa "SALIR"
            else if (interfaz.botonPulsado(clickX, clickY, 670, 30, 110, 45)) {
                exit(0); // Cierra la aplicación
            }
            break;

        case INSTRUCCIONES:
            // Botón "VOLVER" en las instrucciones
            if (interfaz.botonCircularPulsado(clickX, clickY, 150, 450, 25)) {
                estadoActual = MENU_PRINCIPAL;
            }
            break;

        case SELECCION_BANDO:

            // 1. Botón INFO HEALTHY
            if (interfaz.botonCircularPulsado(clickX, clickY, 100 + 160, 500 + 30, 20.0f)) {
                infoActual = INFO_HEALTHY;
            }
            // 2. Botón INFO JUNK 
            else if (interfaz.botonCircularPulsado(clickX, clickY, 500 + 160, 500 + 30, 20.0f)) {
                infoActual = INFO_JUNK;
            }
            // 3. Botón JUGAR CON HEALTHY 
            else if (interfaz.botonPulsado(clickX, clickY, 100, 500, 180, 60)) {
                bandoSeleccionado = HEALTHY;
                estadoActual = TABLERO;
            }
            // 4. Botón JUGAR CON JUNK 
            else if (interfaz.botonPulsado(clickX, clickY, 500, 500, 180, 60)) {
                bandoSeleccionado = JUNK;
                estadoActual = TABLERO;
            }
            // BOTÓN VOLVER AL MENÚ PRINCIPAL
            if (interfaz.botonCircularPulsado(clickX, clickY, 40, 560, 25)) {
                estadoActual = MENU_PRINCIPAL;
            }
            break;
        
        case TABLERO:
            // Botón PAUSA 
            if (interfaz.botonCircularPulsado(clickX, clickY, 760, 560, 20)) {
                pausa = !pausa; 
            }

            // Botón AJUSTES 
            else if (interfaz.botonCircularPulsado(clickX, clickY, 710, 560, 20)) {
                infoActual = INFO_AJUSTES;
            }

            // Botón INFO 
            else if (interfaz.botonCircularPulsado(clickX, clickY, 660, 560, 20)) {
                infoActual = INFO_GENERAL;
            }
            else {
                // Le pasamos la 'pausa' para que el tablero sepa si debe ignorar el clic
                tablero.gestionRaton(x, y, pausa);
            }
            break;
        }

    }

    // IMPORTANTE: Redibujar para que el cambio de pantalla se vea al instante
    glutPostRedisplay();
}

void Mundo::mousePasivo(int x, int y) {
    // Traducimos igual que en la función mouse()
    mouseX = x * (800.0f / (float)glutGet(GLUT_WINDOW_WIDTH));
    mouseY = 600.0f - (y * (600.0f / (float)glutGet(GLUT_WINDOW_HEIGHT)));

    glutPostRedisplay(); // Forzamos redibujado para que se vea el cambio de color al instante
}