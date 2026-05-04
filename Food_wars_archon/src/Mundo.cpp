#include "Mundo.h"
#include "freeglut.h"

void Mundo::inicializa() {

    // 1. Configurar estados iniciales
    estadoActual = MENU_PRINCIPAL;
    infoActual = NINGUNA;

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
    if (boton == 0 && estado == 0) {    //if para que se actica solo al clicar y no al clicar y desclicar

        tablero.gestionRaton(x, y);
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