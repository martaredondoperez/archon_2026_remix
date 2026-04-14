#include "Mundo.h"
#include "freeglut.h"

void Mundo::inicializa() {
    // Aquí pondremos los colores de fondo más adelante
    gluOrtho2D(0, 800, 0, 600);
    estadoActual = MENU_PRINCIPAL;
    // Configura la proyección (Cámara)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Esto mapea tus coordenadas a la ventana

    glMatrixMode(GL_MODELVIEW); // Volvemos al modo de dibujo

    estadoActual = MENU_PRINCIPAL;
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
        break;
    case TABLERO:
        //tablero.dibuja(); // La Persona 2 trabaja aquí
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
    case 27: // Tecla ESC para salir
        exit(0);
        break;
    }
}