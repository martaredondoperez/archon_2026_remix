#include "Mundo.h"
#include "freeglut.h"
#include <iostream>
#include <cstring>
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

    glLoadIdentity();

    int victoria = 0;
    switch (estadoActual) {
    case MENU_PRINCIPAL:
        interfaz.dibujaMenu();
        break;
    case MENU_DIFICULTAD:
        interfaz.dibujaMenuDificultad();
        break;
    case PANTALLA_NOMBRE:
        // Le pasamos el número de nombre que estamos pidiendo (1 o 2)
        interfaz.dibujaPantallaNombre(tablero.nombresRecogidos + 1, tablero.bufferEscritura);
        break;
    case SELECCION_BANDO:
        interfaz.dibujaSeleccion();
        // LÓGICA DE INFO SUPERPUESTA
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

        victoria = tablero.comprobarVictoria();
        if (victoria != 0) { // Si devuelve 1 o 2...
            ganadorJuego = victoria; // Guardamos quién ganó
            registrarVictoria(ganadorJuego, tablero.getTurnos());
            estadoActual = GAMEOVER; // ¡Cambiamos de pantalla!
        }
        if (tablero.modoUnJugador == true && tablero.getTurnoActual() == tablero.bandoIA) {

            tablero.jugarTurnoIA();
        }
        //arena
        if (tablero.combatePendiente == true) {
            // Le pasamos los punteros a la clase de tu compañero
            arena.iniciarCombate(tablero.atacantePendiente, tablero.defensorPendiente);

            // Bajamos la bandera para que no se repita
            tablero.combatePendiente = false;

            // ¡Cambiamos de pantalla!
            estadoActual = ARENA;
        }
        if (pausa) {
            interfaz.dibujaPausa();
        }
        // Si hay alguna info activa (AYUDA o AJUSTES)
        if (infoActual != NINGUNA) {
            interfaz.mostrarInfoTablero(infoActual);
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
        interfaz.dibujaMenuRanking(tablero.nombreJugador1);
        break;
    }

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
                        estadoActual = MENU_DIFICULTAD;
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
        // Esto es lo que permite que el ratón funcione aunque maximices
        float anchoV = (float)glutGet(GLUT_WINDOW_WIDTH);
        float altoV = (float)glutGet(GLUT_WINDOW_HEIGHT);
        float aspect_ventana = anchoV / altoV;
        float aspect_juego = 800.0f / 600.0f;

        float clickX, clickY;

        if (aspect_ventana >= aspect_juego) {
            float ancho_logico = 600.0f * aspect_ventana;
            float extra = (ancho_logico - 800.0f) / 2.0f;
            clickX = (x / anchoV) * (800.0f + 2.0f * extra) - extra;
            clickY = (1.0f - (y / altoV)) * 600.0f;
        }
        else {
            float alto_logico = 800.0f / aspect_ventana;
            float extra = (alto_logico - 600.0f) / 2.0f;
            clickX = (x / anchoV) * 800.0f;
            clickY = (1.0f - (y / altoV)) * (600.0f + 2.0f * extra) - extra;
        }
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
                tablero.maxNombresNecesarios = 1; // Solo pediremos 1 nombre
                tablero.nombresRecogidos = 0;
                tablero.bufferEscritura = "";
                estadoActual = PANTALLA_NOMBRE;
            }
            // Si pulsa "2 JUGADORES" 
            else if (interfaz.botonPulsado(clickX, clickY, 300, 200, 200, 60)) {
                numJugadores = 2;
                dificultadIA = 0;
                tablero.maxNombresNecesarios = 2;
                tablero.nombresRecogidos = 0;
                tablero.bufferEscritura = "";
                estadoActual = PANTALLA_NOMBRE;
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

        case MENU_DIFICULTAD: 
            // Botón PRINCIPIANTE 
            if (interfaz.botonPulsado(clickX, clickY, 300, 350, 200, 60)) {
                dificultadIA = 1;
                estadoActual = SELECCION_BANDO;
            }
            // Botón GUERRERO 
            else if (interfaz.botonPulsado(clickX, clickY, 300, 250, 200, 60)) {
                dificultadIA = 2;
                estadoActual = SELECCION_BANDO;
            }
            // Botón PESADILLA
            else if (interfaz.botonPulsado(clickX, clickY, 300, 150, 200, 60)) {
                dificultadIA = 3;
                estadoActual = SELECCION_BANDO;
            }
            // Botón VOLVER (Circular)
            else if (interfaz.botonCircularPulsado(clickX, clickY, 60, 540, 25)) {
                estadoActual = PANTALLA_NOMBRE;
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
                tablero.setTurnoInicial(SALUDABLE);
                // El Jugador 1 eligió Healthy
                strncpy_s(tablero.nombreSana, tablero.nombreJugador1.c_str(), 49);

                if (numJugadores == 2) {
                    strncpy_s(tablero.nombreBasura, tablero.nombreJugador2.c_str(), 49);
                }
                else {
                    strcpy_s(tablero.nombreBasura, "MIGUEL HERNANDO (IA)");
                    tablero.modoUnJugador = true;
                    tablero.bandoIA = BASURA; // La IA será la Basura

                }

                // Pasamos la dificultad elegida al tablero antes de entrar
                tablero.setDificultad(this->dificultadIA); // <--- CLAVE
                tablero.inicializa();                      // <--- CLAVE (Opcional si quieres resetear piezas)
                estadoActual = TABLERO;
            }
            // 4. Botón JUGAR CON JUNK 
            else if (interfaz.botonPulsado(clickX, clickY, 500, 500, 180, 60)) {
                bandoSeleccionado = JUNK;
                tablero.setTurnoInicial(BASURA);
                // El Jugador 1 eligió Junk
                strncpy_s(tablero.nombreBasura, tablero.nombreJugador1.c_str(), 49);

                if (numJugadores == 2) {
                    strncpy_s(tablero.nombreSana, tablero.nombreJugador2.c_str(), 49);
                }
                else {
                    strcpy_s(tablero.nombreSana, "MIGUEL HERNANDO (IA)");
                    tablero.modoUnJugador = true;
                    tablero.bandoIA = SALUDABLE; // La IA

                }
                // Pasamos la dificultad elegida al tablero antes de entrar
                tablero.setDificultad(this->dificultadIA); // <--- CLAVE
                tablero.inicializa();                      // <--- CLAVE
                estadoActual = TABLERO;
            }
            // BOTÓN VOLVER AL MENÚ PRINCIPAL
            if (interfaz.botonCircularPulsado(clickX, clickY, 40, 560, 25)) {
                tablero.nombresRecogidos = 0;   // Reset contador
                tablero.bufferEscritura = "";    // Reset texto
                tablero.nombreJugador1 = "";     // Limpiar nombres guardados
                tablero.nombreJugador2 = "";
                if (numJugadores == 1) estadoActual = MENU_DIFICULTAD;
                else estadoActual = PANTALLA_NOMBRE;
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
                tablero.gestionRaton(button, (int)clickX, (int)clickY, pausa);
            }
            break;
        case GAMEOVER:
            // Botón VER RANKING
            if (interfaz.botonPulsado(clickX, clickY, 300, 320, 200, 50)) {
                estadoActual = RANKING; // Cuando lo tengamos listo
            }
            // Botón REINTENTAR (Reinicia el tablero y vuelve a jugar)
            else if (interfaz.botonPulsado(clickX, clickY, 300, 240, 200, 50)) {
                tablero.inicializa(); // Esto pone ganadorFinal a 0
                estadoActual = MENU_PRINCIPAL;
            }
            // Botón MENU PRINCIPAL
            else if (interfaz.botonPulsado(clickX, clickY, 300, 160, 200, 50)) {
                exit(0);
            }
            break;
        case RANKING:
            // Dibujamos el ranking pasando el nombre del jugador 1 por si queremos resaltarlo
            //interfaz.dibujaMenuRanking(tablero.nombreJugador1);
            // Botón "VOLVER" (usando las mismas coordenadas que el de instrucciones para ser coherentes)
            if (interfaz.botonCircularPulsado(clickX, clickY, 60, 540, 25)) {
                estadoActual = GAMEOVER;
            }
            break;
        case PANTALLA_NOMBRE:
            // Botón VOLVER (Circular)
            if (interfaz.botonCircularPulsado(clickX, clickY, 60, 540, 25)) {
                estadoActual = MENU_PRINCIPAL;
            }
        

        }

    }

    // IMPORTANTE: Redibujar para que el cambio de pantalla se vea al instante
    glutPostRedisplay();
}

void Mundo::mousePasivo(int x, int y) {
    // --- 1. TRADUCCIÓN CON FRANJAS (Igual que en Mundo::mouse) ---
    float anchoV = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoV = (float)glutGet(GLUT_WINDOW_HEIGHT);
    float aspect_ventana = anchoV / altoV;
    float aspect_juego = 800.0f / 600.0f;

    if (aspect_ventana >= aspect_juego) {
        // Ventana ancha (franjas laterales)
        float ancho_logico = 600.0f * aspect_ventana;
        float extra = (ancho_logico - 800.0f) / 2.0f;
        mouseX = (x / anchoV) * (800.0f + 2.0f * extra) - extra;
        mouseY = (1.0f - (y / altoV)) * 600.0f;
    }
    else {
        // Ventana alta (franjas arriba/abajo)
        float alto_logico = 800.0f / aspect_ventana;
        float extra = (alto_logico - 600.0f) / 2.0f;
        mouseX = (x / anchoV) * 800.0f;
        mouseY = (1.0f - (y / altoV)) * (600.0f + 2.0f * extra) - extra;
    }

    // --- 2. REDIBUJAR ---
    // Ahora mouseX y mouseY valen entre 0-800 y 0-600 correctamente
    glutPostRedisplay();
}

void Mundo::registrarVictoria(int ganador, int turnosTotales) {
    EntradaRanking nueva;

    // 1. Identificamos quién ha ganado y copiamos su nombre y bando
    if (ganador == 1) { // Gana Saludable
        strncpy_s(nueva.nombre, tablero.nombreSana, 49);
        strcpy_s(nueva.bando, "Healthy");
    }
    else { // Gana Basura
        strncpy_s(nueva.nombre, tablero.nombreBasura, 49);
        strcpy_s(nueva.bando, "Junk");
    }
    nueva.turnos = turnosTotales;

    // 2. Guardar en archivo (binario para que sea fácil de leer luego)
    FILE* f = nullptr;
    errno_t err = fopen_s(&f, "ranking.bin", "ab");
    if (err == 0 && f != nullptr) {
        fwrite(&nueva, sizeof(EntradaRanking), 1, f);
        fclose(f);
    }
}