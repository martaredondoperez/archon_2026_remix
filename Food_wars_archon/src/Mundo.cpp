#include "Mundo.h"
#include "freeglut.h"


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
       //arena.dibuja();   // La Persona 3 trabaja aquí
       jugador1.dibuja();

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
        case 'e': case 'E': {
            Proyectil nuevoDisparo;
            nuevoDisparo.posicion = jugador1.posicion; // Nace en el centro del jugador
            nuevoDisparo.velocidad.x = 300.0f;         // Sale disparado hacia la derecha
            nuevoDisparo.velocidad.y = 0.0f;

            disparos.push_back(nuevoDisparo); // Metemos el disparo en la memoria dinámica
            break;
            }
        }
    }
}

void Mundo::teclaUp(unsigned char tecla, int x, int y) {
    if (estadoActual == ARENA) {
        switch (tecla) {
        case 'a': case 'A': case 'd': case 'D':
            jugador1.velocidad.x = 0; // Freno el eje X al soltar A o D
            break;
        case 'w': case 'W': case 's': case 'S':
            jugador1.velocidad.y = 0; // Freno el eje Y al soltar W o S
            break;
        }
    }
}


// Esta funcion es el bucle de fisicas
void Mundo::update() {
    float t = 0.025f; // tiempo para las ecuaciones (dt)

    // Le paso los limites de la pantalla basandome en el Ortho2D (800x600)
    jugador1.mueve(t, 800.0f, 600.0f);

    // Borrado de proyectiles
    for (auto it = disparos.begin(); it != disparos.end(); ) {
        it->mueve(t); // Primero movemos el proyectil

        // Comprobamos si se ha salido de los límites (0 a 800 y 0 a 600)
        if (it->posicion.x > 800.0f || it->posicion.x < 0.0f ||
            it->posicion.y > 600.0f || it->posicion.y < 0.0f) {

            // Si está fuera, lo borramos de la memoria y el iterador pasa al siguiente
            it = disparos.erase(it);
        }
        else {
            // Si sigue dentro, simplemente pasamos al siguiente proyectil
            it++;
        }
    }

}