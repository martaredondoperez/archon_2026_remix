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

    // 3. INICIALIZAR MÚSICA DE FONDO
    ETSIDI::playMusica("sonidos/musicafondo.mp3", false);
}

void Mundo::actualizaFisicas() {
    // Actualizar físicas del juego (llamado por el timer)
    // Aquí se pueden actualizar animaciones, posiciones, etc.
    if (estadoActual == ARENA) {
        arena.actualiza();
    }
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
    case TABLERO: {
        tablero.dibuja(pausa); // La Persona 2 trabaja aquí
        // Calculamos qué toca el ratón y se lo pasamos al HUD
        InfoFicha infoRat = tablero.obtenerInfoRaton(Mundo::mouseX, Mundo::mouseY);
        interfaz.dibujaHUDJuego(infoRat);

        victoria = tablero.comprobarVictoria();
        if (victoria != 0) { // Si devuelve 1 o 2...
            ganadorJuego = victoria; // Guardamos quién ganó
            registrarVictoria(ganadorJuego, tablero.getTurnos());
            estadoActual = GAMEOVER; // ¡Cambiamos de pantalla!
        }
        if (tablero.esModoUnJugador() == true && tablero.getTurnoActual() == tablero.getBandoIA()) {
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
        interfaz.dibujaBotones(TABLERO, infoActual);
        break;
    }
    case ARENA: {
        arena.actualiza();
        arena.dibuja();
      

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

        std::string textoBando = (tablero.getTurnoActual() == 1) ? "SALUDABLE" : "BASURA";
        interfaz.dibujaMenuRanking(&gestorRanking, tablero.nombreJugador1, tablero.getTurnosTotales(), textoBando);
        interfaz.dibujaBotones(estadoActual, infoActual);
        gestorPantalla.dibujarFranjas();
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

    if (estadoActual == ARENA) {
        arena.teclado(tecla);
    }

    if (estadoActual == PANTALLA_NOMBRE) {
        const int LONGITUD_MAX_NOMBRE = 15;

        if (tecla == 13) { // Tecla ENTER

            // Validar que el nombre no esté vacío
            if (!tablero.bufferEscritura.empty()) {
                // Trim automático de espacios al inicio/final
                tablero.bufferEscritura.erase(0, tablero.bufferEscritura.find_first_not_of(" "));
                tablero.bufferEscritura.erase(tablero.bufferEscritura.find_last_not_of(" ") + 1);

                // Guardar solo si después del trim sigue teniendo contenido
                if (!tablero.bufferEscritura.empty()) {
                    if (tablero.nombresRecogidos == 0) {
                        tablero.nombreJugador1 = tablero.bufferEscritura;
                    }
                    else {
                        tablero.nombreJugador2 = tablero.bufferEscritura;
                    }

                    tablero.nombresRecogidos++;
                    tablero.bufferEscritura.clear(); // Limpiamos el buffer para el siguiente

                    // Si tenemos todos los nombres, pasar a siguiente pantalla
                    if (tablero.nombresRecogidos >= tablero.maxNombresNecesarios) {
                        estadoActual = SELECCION_BANDO;
                    }
                }
            }
        }
        else if (tecla == 8) { // Tecla BACKSPACE (Borrar)
            tablero.borrarUltimaLetraBuffer();
        }

        else if (tablero.bufferEscritura.length() < LONGITUD_MAX_NOMBRE) { // Límite de caracteres
            // Solo letras, números y espacios usando std::isalnum
            if (std::isalnum(static_cast<unsigned char>(tecla)) || tecla == ' ') {
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
    if (estadoActual == ARENA) {
        arena.teclasEspeciales(tecla);
    }
}

void Mundo::tecladoUp(unsigned char tecla, int x, int y) {
    // Si estás en la arena, le pasamos el evento a la arena para que deje de mover al J1
    if (estadoActual == ARENA) {
        arena.tecladoUp(tecla);
    }
}

void Mundo::teclasEspecialesUp(int tecla, int x, int y) {
    // Si estás en la arena, le pasamos el evento a la arena para que deje de mover al J2
    if (estadoActual == ARENA) {
        arena.teclasEspecialesUp(tecla);
    }
}

void Mundo::mouse(int button, int state, int x, int y) {
    // 1. Detectar el clic izquierdo
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 2. Lógica según la pantalla en la que estemos
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            // La interfaz buscará en el mapa del estadoActual.
            // Si hay un PopUp y pulsas en la X, se ejecutará su lambda y cerrará la info.
            float clickX, clickY;
            gestorPantalla.convertirCoordenadasMouse((float)x, (float)y, clickX, clickY);
            interfaz.gestionarClick(clickX, clickY, estadoActual);

/*
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
            // Gestionar clics dentro del popup de ajustes
            if (infoActual == INFO_AJUSTES && interfaz.popUpAjustes != nullptr) {
                interfaz.popUpAjustes->gestionarClick(clickX, clickY);
                if (!interfaz.popUpAjustes->esVisible()) {
                    infoActual = NINGUNA;
                }
            }
            // Comprobamos si el clic cae dentro del cuadradito de la "X"
            // Ajusta estas coordenadas (630, 470) a donde dibujes tu botón de cerrar
            else if (interfaz.botonPulsado(clickX, clickY, 610, 410, 30, 30)) {
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
                tablero.setMaxNombres(1);          // Usamos el setter
                tablero.resetNombresRecogidos();   // Reseteamos a 0
                tablero.limpiarBuffer();           // Vaciamos el texto
                estadoActual = PANTALLA_NOMBRE;
            }
            // Si pulsa "2 JUGADORES"
            else if (interfaz.botonPulsado(clickX, clickY, 300, 200, 200, 60)) {
                numJugadores = 2;
                dificultadIA = 0;
                tablero.setMaxNombres(2);          // Usamos el setter para 2
                tablero.resetNombresRecogidos();   // Reseteamos a 0
                tablero.limpiarBuffer();           // Vaciamos el texto
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

                // El Jugador 1 eligió Healthy (Ojo a cómo pasamos el 50 y el _TRUNCATE)
                strncpy_s(tablero.getNombreSana(), 50, tablero.getNombreJ1().c_str(), _TRUNCATE);

                if (numJugadores == 2) {
                    strncpy_s(tablero.getNombreBasura(), 50, tablero.getNombreJ2().c_str(), _TRUNCATE);
                }
                else {
                    // Al usar strcpy_s ahora hay que pasarle el tamaño máximo (50)
                    strcpy_s(tablero.getNombreBasura(), 50, "MIGUEL HERNANDO (IA)");
                    tablero.setModoUnJugador(true);
                    tablero.setBandoIA(BASURA); // La IA será la Basura
                }

                // Pasamos la dificultad elegida al tablero antes de entrar
                tablero.setDificultad(this->dificultadIA);
                tablero.inicializa();
                estadoActual = TABLERO;
            }

            // 4. Botón JUGAR CON JUNK
            else if (interfaz.botonPulsado(clickX, clickY, 500, 500, 180, 60)) {
                bandoSeleccionado = JUNK;
                tablero.setTurnoInicial(BASURA);

                // El Jugador 1 eligió Junk
                strncpy_s(tablero.getNombreBasura(), 50, tablero.getNombreJ1().c_str(), _TRUNCATE);

                if (numJugadores == 2) {
                    strncpy_s(tablero.getNombreSana(), 50, tablero.getNombreJ2().c_str(), _TRUNCATE);
                }
                else {
                    strcpy_s(tablero.getNombreSana(), 50, "MIGUEL HERNANDO (IA)");
                    tablero.setModoUnJugador(true);
                    tablero.setBandoIA(SALUDABLE); // La IA será la Sana
                }

                // Pasamos la dificultad elegida al tablero antes de entrar
                tablero.setDificultad(this->dificultadIA);
                tablero.inicializa();
                estadoActual = TABLERO;
            }

            // BOTÓN VOLVER AL MENÚ PRINCIPAL
            if (interfaz.botonCircularPulsado(clickX, clickY, 40, 560, 25)) {
                tablero.resetNombresRecogidos(); // Reset contador
                tablero.limpiarBuffer();         // Reset texto
                tablero.setNombreJ1("");         // Limpiar nombres guardados
                tablero.setNombreJ2("");         // Limpiar nombres guardados

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
*/
            // Solo si no hay popup visible, permitimos clickar en el tablero
            // Pasamos x, y SIN convertir (gestionRaton lo hace internamente)
            if (estadoActual == TABLERO && !pausa && (interfaz.popUpActivo == nullptr || !interfaz.popUpActivo->esVisible())) {
                tablero.gestionRaton(button, (int)clickX, (int)clickY, pausa);
            }

            glutPostRedisplay();
        }
    }
}

void Mundo::mousePasivo(int x, int y) {
    // Usar el gestor para convertir coordenadas
    gestorPantalla.convertirCoordenadasMouse((float)x, (float)y, mouseX, mouseY);

    // Actualizar estado del mouse en el popup de ajustes si está visible
    if (infoActual == INFO_AJUSTES && interfaz.popUpAjustes != nullptr) {
        interfaz.popUpAjustes->actualizarMouse(mouseX, mouseY);
    }

    // Redibujar
    glutPostRedisplay();
}


void Mundo::registrarVictoria(int ganador, int turnosTotales) {
    // 1. Identificamos quién ha ganado y preparamos los datos
    std::string nombre;
    std::string bando;

    // Mapear ganador (1=SALUDABLE, 2=BASURA) a nombre y bando
    if (ganador == 1) { // Gana SALUDABLE
        bando = "Healthy";
        // Verificar quién jugaba con SALUDABLE
        if (tablero.bandoJugador1 == 1) {
            // Jugador 1 eligió Saludable, así que Jugador 1 gana
            nombre = tablero.nombreJugador1;
        }
        else {
            // En modo 2 jugadores, Jugador 2 jugaba con Saludable
            nombre = tablero.nombreJugador2;
        }
    }
    else { // Gana BASURA
        bando = "Junk";
        // Verificar quién jugaba con BASURA
        if (tablero.bandoJugador1 == 2) {
            // Jugador 1 eligió Basura, así que Jugador 1 gana
            nombre = tablero.nombreJugador1;
        }
        else {
            // Verificar si es modo 1 jugador (IA) o 2 jugadores (Jugador 2)
            if (tablero.modoUnJugador) {
                // Modo 1 jugador: IA ganó
                nombre = "IA";
            }
            else {
                // Modo 2 jugadores: Jugador 2 jugaba con Basura
                nombre = tablero.nombreJugador2;
            }
        }
    }

    // 2. Guardar usando el GestorRanking
    gestorRanking.agregarEntrada(nombre, turnosTotales, bando);
}