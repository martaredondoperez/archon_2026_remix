#include "Mundo.h"
#include "freeglut.h"
#include <iostream>
#include <cstring>
float Mundo::mouseX = 0.0f;
float Mundo::mouseY = 0.0f;

void Mundo::inicializa() {

    // 1. Configurar estados iniciales
    interfaz.inicializa(this);

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
    glLoadIdentity();

    // Configurar la cámara con el gestor
    gestorPantalla.configurarCamara();

    interfaz.actualizaEstadoBotones(mouseX, mouseY, estadoActual);
    interfaz.actualizarMouse(mouseX, mouseY); // Actualizar PopUp
    int victoria = 0;
    switch (estadoActual) {
    case MENU_PRINCIPAL:
        interfaz.dibujaMenu();
        break;
    case PANTALLA_NOMBRE:
        // Le pasamos el número de nombre que estamos pidiendo (1 o 2)
        interfaz.dibujaPantallaNombre(tablero.nombresRecogidos + 1, tablero.bufferEscritura);
        interfaz.dibujaBotones(PANTALLA_NOMBRE, NINGUNA);
        break;
    case SELECCION_BANDO:
        interfaz.dibujaSeleccion();
        break;
    case INSTRUCCIONES:
        interfaz.dibujaInstrucciones();
        break;
    case TABLERO:
    {
        tablero.dibuja(pausa);

        // 1. CREAMOS UNA FICHA VACÍA POR DEFECTO
        InfoFicha fichaBajoRaton;
        fichaBajoRaton.activa = false;

        // 2. SOLO BUSCAMOS SI NO HAY PAUSA Y NO HAY MENÚS ABIERTOS
        if (!pausa && infoActual == NINGUNA) {
            fichaBajoRaton = tablero.obtenerInfoRaton(mouseX, mouseY);
        }

        // 3. PASAMOS LA INFO (Si activa es false, la interfaz no dibujará nada)
        interfaz.dibujaHUDJuego(fichaBajoRaton);

        victoria = tablero.comprobarVictoria();
        if (victoria != 0) {
            ganadorJuego = victoria;
            registrarVictoria(ganadorJuego, tablero.getTurnos());
            estadoActual = GAMEOVER;
        }
        if (tablero.modoUnJugador == true && tablero.getTurnoActual() == tablero.bandoIA) {


            tablero.jugarTurnoIA();
        }
        //arena
        if (tablero.combatePendiente == true) {
            arena.iniciarCombate(tablero.atacantePendiente, tablero.defensorPendiente);
            tablero.combatePendiente = false;
            estadoActual = ARENA;
        }

        if (pausa) {
            interfaz.dibujaPausa();
        }

        if (infoActual != NINGUNA) {
            interfaz.mostrarInfoTablero(infoActual);
        }
        interfaz.dibujaBotones(TABLERO, infoActual);
    }
    break;
    case ARENA: {
        arena.dibuja();
        arena.actualiza();

        // Vigilamos si la pelea ha terminado
        int ganadorDeLaPelea = arena.getGanadorCombate();

        if (ganadorDeLaPelea != 0) { // Si es 1 (Atacante) o 2 (Defensor)

            // Le decimos al tablero que aplique las consecuencias (muertes/movimientos)
            tablero.resolverCombate(ganadorDeLaPelea);

            // Volvemos al tablero pacífico
            estadoActual = TABLERO;
        }
        break;        
    }
    case PAUSA:
        interfaz.dibujaPausa();
        break;
    case GAMEOVER:

        interfaz.dibujaFinal(ganadorJuego);
        break;
    case RANKING:
        interfaz.dibujaMenuRanking(&gestorRanking, tablero.nombreJugador1);
        break;
    }
    interfaz.dibujaBotones(estadoActual, infoActual);

    // Dibujar franjas negras si la pantalla se ha redimensionado
    gestorPantalla.dibujarFranjas();

    glutSwapBuffers();
}



void Mundo::teclado(unsigned char tecla, int x, int y) {

    if (tecla == 27) { //  ESC para salir del juego
        exit(0);
    }

    if (estadoActual == TABLERO) {
        tablero.gestionTeclado(tecla, x, y);
    }
    
    if (estadoActual == PANTALLA_NOMBRE) {
        if (tecla == 13) { // Tecla ENTER
            std::cout << "Nombres recogidos: " << tablero.nombresRecogidos + 1 << " de " << tablero.maxNombresNecesarios << std::endl;
            if (tablero.bufferEscritura.length() > 0) {
                // Guardamos el nombre actual
                if (tablero.nombresRecogidos == 0) {
                    tablero.nombreJugador1 = tablero.bufferEscritura;
                }
                else {
                    tablero.nombreJugador2 = tablero.bufferEscritura;
                }

                tablero.nombresRecogidos++;
                tablero.bufferEscritura = ""; // Limpiamos el buffer para el siguiente

                // --- LÓGICA DE SALTO DE PANTALLA ---
                if (tablero.nombresRecogidos >= tablero.maxNombresNecesarios) {
                    if (tablero.maxNombresNecesarios == 1) {
                        // Si es 1 jugador, primero elegimos dificultad para la IA
                        estadoActual = SELECCION_BANDO;
                    }
                    else {
                        // Si son 2 jugadores, vamos directo a elegir quién es quién
                        estadoActual = SELECCION_BANDO;
                    }
                }
            }
        }
        else if (tecla == 8) { // Tecla BACKSPACE (Borrar)
            if (!tablero.bufferEscritura.empty()) {
                tablero.bufferEscritura.pop_back();
            }
        }
        else if (tablero.bufferEscritura.length() < 15) { // Límite de caracteres
            // Solo letras, números y espacios
            if (isalnum(tecla) || tecla == ' ') {
                tablero.bufferEscritura += tecla;
            }
        }
        glutPostRedisplay();
        return;
    }
}

void Mundo::teclasEspeciales(int tecla, int x, int y) {

    // Controles especiales del Tablero magia
    if (estadoActual == TABLERO) {
        tablero.gestionTeclasEspeciales(tecla, x, y);
    }
}


void Mundo::mouse(int button, int state, int x, int y) {
    // 1. Detectar el clic izquierdo
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 2. Traducir los píxeles de la ventana a nuestro mundo de 800x600
        float clickX, clickY;
        gestorPantalla.convertirCoordenadasMouse((float)x, (float)y, clickX, clickY);

        // 3. Lógica según la pantalla en la que estemos
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            // La interfaz buscará en el mapa del estadoActual.
            // Si hay un PopUp y pulsas en la X, se ejecutará su lambda y cerrará la info.
            interfaz.gestionarClick(clickX, clickY, estadoActual);

            // Solo si no hay información abierta, permitimos clickar en el tablero
            if (estadoActual == TABLERO && infoActual == NINGUNA && !pausa) {
                tablero.gestionRaton(button, (int)clickX, (int)clickY, pausa);
            }

            glutPostRedisplay();
        }
    }
}

void Mundo::mousePasivo(int x, int y) {
    // Usar el gestor para convertir coordenadas
    gestorPantalla.convertirCoordenadasMouse((float)x, (float)y, mouseX, mouseY);

    // Redibujar
    glutPostRedisplay();
}


void Mundo::registrarVictoria(int ganador, int turnosTotales) {
    // 1. Identificamos quién ha ganado y preparamos los datos
    std::string nombre;
    std::string bando;

    if (ganador == 1) { // Gana Saludable
        nombre = tablero.nombreSana;
        bando = "Healthy";
    }
    else { // Gana Basura
        nombre = tablero.nombreBasura;
        bando = "Junk";
    }

    // 2. Guardar usando el GestorRanking
    gestorRanking.agregarEntrada(nombre, turnosTotales, bando);
}