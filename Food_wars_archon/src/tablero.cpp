#include "Tablero.h"
#include "Definiciones.h"
#include <iostream>
#include "freeglut.h"
#include "Interfaz.h"
#include <vector>
#include <cstdlib>
#include "cmath"

// Fichas Saludables
#include "ChefMichelin.h"
#include "AguacateEscudo.h"
#include "RodajaLimon.h"
#include "ChorroVitaminas.h"
#include "JengibreTeleport.h"

// Fichas Basura
#include "ReyHamburguesa.h"
#include "HamburguesaTriple.h"
#include "PizzaPepperoni.h"
#include "DispatoKetchup.h"
#include "HeladoShapeshifter.h"

// Neutral / Otros
#include "ElementalFuego.h"

Tablero::Tablero() :
    fondo_tablero("imagenes/fondo_menu_principal.png")
{
    turnosTotales = 0;
    fondo_tablero.setPos(0, 0);
    fondo_tablero.setSize(800, 600);
    fondo_tablero.setCenter(0, 0);
    bajasSaludables.clear();
    bajasBasura.clear();

    // Inicializar array de casillas a nullptr sin llenar de fichas todavía
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j] = NULL;
        }
    }
}


void Tablero::inicializa() {
    // --- NUEVO: LIMPIEZA DE MEMORIA ANTES DE REINICIAR ---
    // 1. Borramos fichas que pudieran quedar en el tablero de una partida anterior
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (casillas[i][j] != nullptr) {
                delete casillas[i][j];
                casillas[i][j] = nullptr;
            }
        }
    }
    muertoSeleccionado = nullptr;

    // 2. Borramos las fichas de las listas de bajas (morgue)
    for (auto p : bajasSaludables) delete p;
    for (auto p : bajasBasura) delete p;

    bajasSaludables.clear();
    bajasBasura.clear();

    // 3. Limpiamos el tablero (variables y matriz)
    haySeleccion = false;
    turnosTotales = 0;

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            casillas[i][j] = NULL;
        }
    }

    // 4. Línea frontal Saludable (Fila 1) - Todos Distancia
    for (int j = 0; j < DIMENSION; j++) {
        casillas[1][j] = new ChorroVitaminas(1, j);
    }

    // 5. Línea trasera Saludable (Fila 0)
    casillas[0][0] = new RodajaLimon(0, 0);
    casillas[0][1] = new AguacateEscudo(0, 1);
    casillas[0][2] = new JengibreTeleport(0, 2);
    casillas[0][3] = new AguacateEscudo(0, 3);
    casillas[0][4] = new ChefMichelin(0, 4);       // Chef (Líder)
    casillas[0][5] = new AguacateEscudo(0, 5);
    casillas[0][6] = new JengibreTeleport(0, 6);
    casillas[0][7] = new AguacateEscudo(0, 7);
    casillas[0][8] = new RodajaLimon(0, 8);

    // 6. Línea frontal Fast Food (Fila 7) - Todos Distancia
    for (int j = 0; j < DIMENSION; j++) {
        casillas[7][j] = new DisparoKetchup(7, j);
    }

    // 7. Línea trasera Fast Food (Fila 8)
    casillas[8][0] = new PizzaPepperoni(8, 0);
    casillas[8][1] = new HamburguesaTriple(8, 1);
    casillas[8][2] = new HeladoShapeshifter(8, 2);
    casillas[8][3] = new HamburguesaTriple(8, 3);
    casillas[8][4] = new ReyHamburguesa(8, 4);       // Rey (Líder)
    casillas[8][5] = new HamburguesaTriple(8, 5);
    casillas[8][6] = new HeladoShapeshifter(8, 6);
    casillas[8][7] = new HamburguesaTriple(8, 7);
    casillas[8][8] = new PizzaPepperoni(8, 8);

    menuMagiaActivo = false;
    hechizoSeleccionado = 0;
    esperandoObjetivo = false;

    // Reseteamos los 7 hechizos de ambos bandos para que se puedan usar
    for (int i = 0; i < 7; i++) {
        hechizosSanaUsados[i] = false;
        hechizosBasuraUsados[i] = false;
    }
}

void Tablero::dibuja(bool pausaActiva) {
    // 1. FONDO
    glEnable(GL_TEXTURE_2D);
    fondo_tablero.draw();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // Desvincula la textura del fondo

    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_LIGHTING);
            float xMin = offsetX + (i * 50.0f);
            float yMin = offsetY + (j * 50.0f);
            float xMax = xMin + 50.0f;
            float yMax = yMin + 50.0f;


            if (esPuntoDePoder(i, j)) {
                //  Los 5 Puntos de Poder 
                glColor3ub(100, 255, 100);
            }
            else if (esCasillaOscilante(i, j)) {
                // EL CAMINO MÁGICO 
                int fase = (turnosTotales / 2) % 4;

                if (fase == 0) glColor3ub(150, 150, 150);      // Neutral (Gris)
                else if (fase == 1) glColor3ub(200, 240, 200); // Ventaja Sana (Clarito)
                else if (fase == 2) glColor3ub(150, 150, 150); // Neutral (Gris)
                else glColor3ub(180, 50, 50);                  // Ventaja Basura (Oscuro)
            }
            else {
                //  EL FONDO DE AJEDREZ (Fijo e inamovible)
                if ((i + j) % 2 == 0) {
                    glColor3ub(40, 40, 40);    // Casillas Oscuras (Gris muy oscuro/Negro)
                }
                else {
                    glColor3ub(240, 230, 200); // Casillas Claras (Beige)
                }
            }

            glBegin(GL_QUADS);
            glVertex2f(xMin, yMin);
            glVertex2f(xMax, yMin);
            glVertex2f(xMax, yMax);
            glVertex2f(xMin, yMax);
            glEnd();

            // Borde negro
            glColor3ub(0, 0, 0);
            glBegin(GL_LINE_LOOP);
            glVertex2f(xMin, yMin);
            glVertex2f(xMax, yMin);
            glVertex2f(xMax, yMax);
            glVertex2f(xMin, yMax);
            glEnd();
            //iluminacion seleeccion de raton
            if (haySeleccion && i == filaSel && j == colSel) {
                glColor3ub(255, 255, 0);
                glLineWidth(4.0f);

                glBegin(GL_LINE_LOOP);
                glVertex2f(xMin, yMin);
                glVertex2f(xMax, yMin);
                glVertex2f(xMax, yMax);
                glVertex2f(xMin, yMax);
                glEnd();

                glLineWidth(1.0f);
            }
            // dibujo ficha
            // Dibujamos la comida (dentro de Comida::dibuja debe haber un glEnable(GL_TEXTURE_2D))
            if (casillas[i][j] != NULL) {
                // Si es la pieza del teletransporte...
                if (esperandoObjetivo && hechizoSeleccionado == 0 && casillas[i][j] == piezaParaTeletransporte) {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    glEnable(GL_TEXTURE_2D);
                    // CLAVE: Esto hace que el color amarillo se mezcle con la textura
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

                    // Amarillo brillante con transparencia
                    glColor4f(1.0f, 1.0f, 0.0f, 0.6f);

                    casillas[i][j]->dibuja(xMin, yMin, ladoCasilla);

                    glDisable(GL_BLEND);
                    glColor3f(1.0f, 1.0f, 1.0f); // Reset color a blanco
                }
                else {
                    // DIBUJO NORMAL
                    glEnable(GL_TEXTURE_2D);
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // Para que el resto se vean normales
                    casillas[i][j]->dibuja(xMin, yMin, ladoCasilla);
                }

                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }

        // Si hay una ficha agarrada en la mano
        if (haySeleccion) {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glColor4f(0.0f, 1.0f, 0.0f, 0.25f);

            // Recorre el tablero
            // Recorre el tablero
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {

                    if (casillas[filaSel][colSel]->intentarMover(i, j) == true) {

                        // filtro apra voladores
                        Comida* miFicha = casillas[filaSel][colSel];
                        if (miFicha->volador == false) {
                            // Si no vuela, comprobamos si hay muros en el camino
                            if (caminoLibre(filaSel, colSel, i, j) == false) {
                                continue; 
                            }
                        }

                        float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
                        float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;
                        float xMin = offsetX + (i * ladoCasilla);
                        float yMin = offsetY + (j * ladoCasilla);
                        float xMax = xMin + ladoCasilla;
                        float yMax = yMin + ladoCasilla;

                        //  Casilla vacía VERDE
                        if (casillas[i][j] == NULL) {
                            glColor4f(0.0f, 1.0f, 0.0f, 0.4f); // Verde

                            glBegin(GL_QUADS);
                            glVertex2f(xMin, yMin); glVertex2f(xMax, yMin);
                            glVertex2f(xMax, yMax); glVertex2f(xMin, yMax);
                            glEnd();
                        }
                        //  Casilla con enemigo ROJO
                        else if (casillas[i][j]->getBando() != turnoActual) {
                            glColor4f(1.0f, 0.0f, 0.0f, 0.2f); // Rojo un poco más intenso

                            glBegin(GL_QUADS);
                            glVertex2f(xMin, yMin); glVertex2f(xMax, yMin);
                            glVertex2f(xMax, yMax); glVertex2f(xMin, yMax);
                            glEnd();
                        }
                    }
                }
            }
            // Desactivamos la transparencia para no afectar al resto del juego
            glDisable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);


        }



        glColor3f(1.0f, 1.0f, 0.0f);


        glRasterPos2f(10.0f, 570.0f);

        int estadoPartida = comprobarVictoria();
        std::string mensaje;

        if (estadoPartida == 0) {
            glColor3f(1.0f, 1.0f, 0.0f);
            glRasterPos2i(10, 570);
            if (turnoActual == SALUDABLE) mensaje = "TURNO: COMIDA SANA";
            else mensaje = "TURNO: COMIDA BASURA";
        }
        else if (estadoPartida == 1) {
            // GANA LA COMIDA SANA
            glColor3f(0.0f, 1.0f, 0.0f);
            glRasterPos2i(250, 300);
            mensaje = "¡VICTORIA! GANA LA COMIDA SANA";
        }
        else if (estadoPartida == 2) {
            // GANA LA COMIDA BASURA
            glColor3f(1.0f, 0.0f, 0.0f);
            glRasterPos2i(250, 300);
            mensaje = "¡VICTORIA! GANA LA COMIDA BASURA";
        }

        // Imprimimos el mensaje letra a letra
        for (int i = 0; i < mensaje.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, mensaje[i]);
        }



    }

    //menu magia
    if (menuMagiaActivo) {

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //fondo caja
        glColor4f(0.1f, 0.1f, 0.3f, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(100, 10); glVertex2f(700, 10);
        glVertex2f(700, 70); glVertex2f(100, 70);
        glEnd();

        // Borde blanco
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(100, 10); glVertex2f(700, 10);
        glVertex2f(700, 70); glVertex2f(100, 70);
        glEnd();
        glLineWidth(1.0f);

        //  7 hechizos 
        std::string nombresHechizos[7] = {
            "Teletransporte",
            "Curacion",
            "Cambiar Tiempo",
            "Intercambio",
            "Invocar Elemental",
            "Revivir",
            "Encarcelar"
        };

        std::string textoHechizo = nombresHechizos[hechizoSeleccionado];

        // Título del hechizo
        glColor3f(1.0f, 1.0f, 0.0f);
        glRasterPos2i(320, 40);
        for (int i = 0; i < textoHechizo.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, textoHechizo[i]);
        }

        // Instrucciones en una sola línea
        glColor3f(1.0f, 1.0f, 1.0f);
        std::string inst1 = " Usa flechas | [ENTER]: confirmar | [ESPACIO]: salir ";
        glRasterPos2i(230, 20);
        for (int i = 0; i < inst1.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, inst1[i]);
        }
    }
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    // --- NUEVO: VISUALIZACIÓN DE PIEZAS PARA REVIVIR ---
    if (esperandoObjetivo && hechizoSeleccionado == 5) {
        // 1. Decidir qué lista mostrar
        std::vector<Comida*>* lista = (turnoActual == SALUDABLE) ? &bajasSaludables : &bajasBasura;

        if (!lista->empty()) {
            // Dibujamos un pequeño fondo para la lista de bajas
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
            glBegin(GL_QUADS);
            glVertex2f(650, 100); glVertex2f(780, 100);
            glVertex2f(780, 500); glVertex2f(650, 500);
            glEnd();

            // Texto informativo
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2i(660, 480);
            std::string t = "MUERTOS:";
            for (char c : t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);

            // 2. Dibujar las fichas en vertical a la derecha
            glEnable(GL_TEXTURE_2D);
            for (int i = 0; i < lista->size(); i++) {
                // Calculamos una posición en el lateral derecho
                float xPos = 680.0f;
                float yPos = 420.0f - (i * 55.0f); // Se van apilando hacia abajo

                // Si hay demasiadas, dejamos de dibujar para que no se salgan
                if (yPos < 120) break;

                // Llamamos al dibuja de la comida
                (*lista)[i]->dibuja(xPos, yPos, 40.0f); // Tamaño un poco más pequeño (40)
            }
        }
    
        else {
            // Si no hay muertos, avisamos
            glColor3f(1.0f, 0.0f, 0.0f);
            glRasterPos2i(300, 300);
            std::string t = "NO HAY PIEZAS PARA REVIVIR";
            for (char c : t) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

}
void Tablero::gestionRaton(int boton, int x, int y, bool pausaActiva) {
    // Si estamos en modo 1 jugador, y le toca a la IA Ignora el ratón
    if (modoUnJugador == true && turnoActual == bandoIA) {
        return; 
    }
    //  EL MURO DE PAUSA
    if (pausaActiva) return;
    if (boton != GLUT_LEFT_BUTTON) return;

    //ACTUALIZAR CÁRCELES SEGÚN EL CICLO ---
    int faseCiclo = (turnosTotales / 2) % 4;
    if (faseCiclo == 0 && turnosTotales % 2 == 0) { // En la fase neutral (Gris), las cárceles se abren
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (casillas[i][j] != NULL) casillas[i][j]->encarcelada = false;
            }
        }
    }
    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;
    //int y_real_gl = 600 - y;
    int filaClic = (int)((x - offsetX) / ladoCasilla);
    int columnaClic = (int)((y - offsetY) / ladoCasilla);

    // --- 1. INTERCEPTOR DE HECHIZOS ---
    // Si el menú está cerrado pero estamos esperando un objetivo de hechizo...
    if (!menuMagiaActivo && esperandoObjetivo) {
        if (boton == GLUT_LEFT_BUTTON) {
            int y_gl =y;

            // --- SECCIÓN A: CLIC EN LA LISTA DE MUERTOS (Fuera del tablero) ---
            // Si el clic es a la derecha (x >= 650) y el hechizo es REVIVIR
            if (hechizoSeleccionado == 5 && x >= 650) {
                std::vector<Comida*>* lista = (turnoActual == SALUDABLE) ? &bajasSaludables : &bajasBasura;

                for (int i = 0; i < (int)lista->size(); i++) {
                    float yFichaCentro = 420.0f - (i * 55.0f);
                    if (y_gl >= (yFichaCentro - 25) && y_gl <= (yFichaCentro + 25)) {
                        muertoSeleccionado = (*lista)[i];
                        return; // Salimos: Ya tenemos muerto, ahora esperamos el siguiente clic en el tablero
                    }
                }
            }

            // --- SECCIÓN B: CLIC EN EL TABLERO (Para lanzar el hechizo) ---
            
            if (filaClic >= 0 && filaClic < 9 && columnaClic >= 0 && columnaClic < 9) {
                
                // Aquí decidimos qué hace el clic según el hechizo activo
                switch (hechizoSeleccionado) {

                case 0: // TELETRANSPORTE
                    if (pasoTeletransporte == 0) {
                        Comida* p = casillas[filaClic][columnaClic];
                        // Primer clic: seleccionar cualquier pieza de mi bando
                        if (p != NULL && p->bando == turnoActual) {
                            piezaParaTeletransporte = p;
                            piezaParaTeletransporte->seleccionadaParaHechizo = true;
                            pasoTeletransporte = 1;
                        }
                    }
                    else if (pasoTeletransporte == 1) {
                        // Segundo clic: mover a casilla vacía
                        if (casillas[filaClic][columnaClic] == NULL) {
                            // 1. Mover la pieza en la matriz
                            casillas[piezaParaTeletransporte->fila][piezaParaTeletransporte->columna] = NULL;
                            casillas[filaClic][columnaClic] = piezaParaTeletransporte;

                            // 2. Actualizar sus coordenadas
                            piezaParaTeletransporte->fila = filaClic;
                            piezaParaTeletransporte->columna = columnaClic;

                            // 3. Resetear estados y cambiar turno
                            piezaParaTeletransporte->seleccionadaParaHechizo = false;
                            esperandoObjetivo = false;
                            haySeleccion = false;
                            pasoTeletransporte = 0;
                            piezaParaTeletransporte = NULL;

                            turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                            turnosTotales++;
                        }
                    }
                    break;

                case 1://CURACION IMPORTANTE COMENTADO VIDA
                    if (casillas[filaClic][columnaClic] != NULL && casillas[filaClic][columnaClic]->bando == turnoActual) {
                       // casillas[filaClic][columnaClic]->vida = casillas[filaClic][columnaClic]->vidaMax; // Curar

                        esperandoObjetivo = false;
                        haySeleccion = false;
                        turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                        turnosTotales++;
                    }
                    break;

                case 3: // INTERCAMBIO
                {
                    // Verificamos que pinchamos en una casilla con alguien
                    if (casillas[filaClic][columnaClic] != NULL) {

                        if (primeraPiezaIntercambio == NULL) {
                            // --- PRIMER CLIC ---
                            primeraPiezaIntercambio = casillas[filaClic][columnaClic];
                            // No pasamos turno ni cerramos esperandoObjetivo aún
                            // Actibar brillo 
                            primeraPiezaIntercambio->seleccionadaParaHechizo = true;
                        }
                        else {
                            // --- SEGUNDO CLIC ---
                            Comida* piezaA = primeraPiezaIntercambio;
                            Comida* piezaB = casillas[filaClic][columnaClic];

                            // Si pinchas la misma pieza dos veces, cancelamos el primer clic
                            if (piezaA == piezaB) {
                                //QUITAR BRILLO SI CANCELA ---
                                piezaA->seleccionadaParaHechizo = false;
                                primeraPiezaIntercambio = NULL;
                                return;
                            }

                            // Guardamos coordenadas de A
                            int filaA = piezaA->fila;
                            int colA = piezaA->columna;

                            // Guardamos coordenadas de B
                            int filaB = piezaB->fila;
                            int colB = piezaB->columna;

                            // 1. Intercambio en la matriz
                            casillas[filaB][colB] = piezaA;
                            casillas[filaA][colA] = piezaB;

                            // 2. Actualizamos sus coordenadas internas
                            piezaA->fila = filaB;
                            piezaA->columna = colB;
                            piezaB->fila = filaA;
                            piezaB->columna = colA;
                           
                            //quitar brillo al terminar
                            piezaA->seleccionadaParaHechizo = false;
                            // --- FINALIZAR TODO ---
                            primeraPiezaIntercambio = NULL;
                            esperandoObjetivo = false;
                            haySeleccion = false;
                            hechizoSeleccionado = -1;

                            turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                            turnosTotales++;
                        }
                    }
                }
                break;
                
                case 4: //INVOCAR ELEMENTAL IMPORTANTE COMENTADO ARENA
                    // 1. Comprobamos que pinchamos a un ENEMIGO
                    if (casillas[filaClic][columnaClic] != NULL && casillas[filaClic][columnaClic]->bando != turnoActual) {

                        // 2. Creamos el objeto Elemental en memoria (el "Gladiador")
                        // Le pasamos el bando del turno actual (el que invoca)
                        Comida* elemental = new ElementalFuego(turnoActual, -1, -1);
                        // 3. ¡A LA ARENA!
                        // Aquí llamas a la función que inicia tu combate. 
                        // IMPORTANTE: Le pasas el 'elemental' que acabamos de crear y la ficha enemiga del tablero.

                        /*bool ganaElemental = iniciarCombateEnArena(elemental, casillas[filaClic][columnaClic]);

                        // 4. Resultado del combate
                        if (ganaElemental) {
                            // Si el elemental gana, la pieza enemiga desaparece del tablero
                            if (casillas[filaClic][columnaClic]->bando == SALUDABLE) 
                                bajasSaludables.push_back(casillas[filaClic][columnaClic]);
                            else 
                                bajasBasura.push_back(casillas[filaClic][columnaClic]);
                            casillas[filaClic][columnaClic] = NULL;
                        }*/
                        // Si pierde el elemental, no pasa nada, el enemigo se queda herido o igual

                        // 5. BORRAMOS EL ELEMENTAL
                        // Como no está en el tablero, si no hacemos delete aquí, 
                        // el objeto se quedaría flotando en la memoria para siempre.
                        delete elemental;

                        // 6. Finalizamos turno
                        esperandoObjetivo = false;
                        haySeleccion = false;
                        turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                        turnosTotales++;
                    }
                    break;
                
                case 5: // REVIVIR
                { int ratonY_GL = 600 - y;
                                        // --- SECCIÓN A: SELECCIONAR EL MUERTO ---
                    if (x >= 650 && x <= 800) {
                        std::vector<Comida*>* lista = (turnoActual == SALUDABLE) ? &bajasSaludables : &bajasBasura;

                        for (int i = 0; i < (int)lista->size(); i++) {
                            float yFichaCentro = 420.0f - (i * 55.0f);

                            // Si el clic cae dentro del área de la ficha en la lista
                            if (ratonY_GL >= (yFichaCentro - 25) && ratonY_GL <= (yFichaCentro + 25)) {
                                muertoSeleccionado = (*lista)[i];
                                // Quitamos el .nombre para que no de error
                                return;
                            }
                        }
                    }

                    // --- SECCIÓN B: PONERLO EN EL TABLERO ---
                    if (muertoSeleccionado != NULL) {
                        // Asegúrate de que filaClic y columnaClic estén bien calculadas antes de este case
                        if (filaClic >= 0 && filaClic < 9 && columnaClic >= 0 && columnaClic < 9) {
                            if (casillas[filaClic][columnaClic] == NULL) {

                                std::vector<Comida*>* lista = (turnoActual == SALUDABLE) ? &bajasSaludables : &bajasBasura;
                                for (auto it = lista->begin(); it != lista->end(); ++it) {
                                    if (*it == muertoSeleccionado) {
                                        lista->erase(it);
                                        break;
                                    }
                                }

                                casillas[filaClic][columnaClic] = muertoSeleccionado;
                                muertoSeleccionado->fila = filaClic;
                                muertoSeleccionado->columna = columnaClic;
                                muertoSeleccionado->vidaActual = muertoSeleccionado->vidaMax;

                                // Limpieza de estados
                                muertoSeleccionado = NULL;
                                esperandoObjetivo = false;
                                hechizoSeleccionado = -1;
                                menuMagiaActivo = false;

                                // Cambio de turno
                                turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                                turnosTotales++;
                            }
                        }
                    }
                    }

                break;
               
                case 6: // ENCARCELAR
                    // 1. Verificamos que pinchamos en una pieza ENEMIGA
                    if (casillas[filaClic][columnaClic] != NULL && casillas[filaClic][columnaClic]->bando != turnoActual) {

                        // 2. Activamos el estado de cárcel en la pieza enemiga
                        casillas[filaClic][columnaClic]->encarcelada = true;

                        // 3. Finalizamos el estado de hechizo y el turno
                        esperandoObjetivo = false;
                        haySeleccion = false;

                        // Cambio de turno
                        turnoActual = (turnoActual == SALUDABLE) ? BASURA : SALUDABLE;
                        turnosTotales++;
                    }
                    break;


                }
            }
        }

        return; // IMPORTANTE: Salimos para que no ejecute el movimiento normal
    }

    //LOGICA MENU Y MOVIMIENTO NORMAL
    if (menuMagiaActivo) {
        return; // Corta la función aquí y no hace nada más
    }

    //  Comprobar si el clic ha sido DENTRO del tablero 
    if (columnaClic >= 0 && columnaClic < 9 && filaClic >= 0 && filaClic < 9) {

        //  MÁQUINA DE ESTADOS DE SELECCIÓN 
        if (boton == 0) {
            if (!haySeleccion) {
                // ESTADO 1: No tenemos nada agarrado. Intentamos agarrar una ficha.
                if (casillas[filaClic][columnaClic] != NULL) {
                    if (casillas[filaClic][columnaClic]->bando == turnoActual) {
                        haySeleccion = true;
                        filaSel = filaClic;
                        colSel = columnaClic;
                    }
                }
            }
            else {
                // ESTADO 2: Ya teníamos una ficha agarrada. Intentamos moverla al nuevo clic.
                // BLOQUEO POR ENCARCELAMIENTO ---
                if (casillas[filaSel][colSel]->encarcelada == true) {
                    // Si está encarcelada, no permitimos que haga nada y la soltamos
                    haySeleccion = false;
                    return; // Salimos de la función inmediatamente
                }
                // Si hacemos clic en la misma casilla, se cancela
                if (filaClic == filaSel && columnaClic == colSel) {
                    haySeleccion = false;
                    // ¡AQUÍ NO SE CAMBIA EL TURNO! Solo la soltamos.
                }

                // CASO B: Mover a una casilla vacía
                else if (casillas[filaClic][columnaClic] == NULL) {

                    // Comprobamos si el movimiento es válido según las reglas de la ficha
                    if (casillas[filaSel][colSel]->intentarMover(filaClic, columnaClic) == true) {
                        Comida* miFicha = casillas[filaSel][colSel];

                        if (miFicha->volador == false && caminoLibre(filaSel, colSel, filaClic, columnaClic) == false) {
                            // Si no vuela y el camino está bloqueado por otra ficha, cancelamos la acción
                            haySeleccion = false;
                            return; 
                        }
                        casillas[filaClic][columnaClic] = casillas[filaSel][colSel];
                        casillas[filaSel][colSel] = NULL;
                        casillas[filaClic][columnaClic]->fila = filaClic;
                        casillas[filaClic][columnaClic]->columna = columnaClic;


                        haySeleccion = false;

                        // SOLO PASA TURNO SI HUBO MOV
                        if (turnoActual == SALUDABLE) {
                            turnoActual = BASURA;
                        }
                        else {
                            turnoActual = SALUDABLE;
                        }
                        turnosTotales++;

                    }
                }
                else {
                    // La ficha que hay ahí es del enemigo?
                    if (casillas[filaClic][columnaClic]->bando != turnoActual) {

                        // Mi ficha tiene rango suficiente para llegar hasta ahí y atacar?
                        if (casillas[filaSel][colSel]->intentarMover(filaClic, columnaClic) == true) {

                            Comida* miFicha = casillas[filaSel][colSel];
                            if (miFicha->volador == false && caminoLibre(filaSel, colSel, filaClic, columnaClic) == false) {
                                // Si no vuela y hay un obstáculo en medio, no puede llegar a atacar
                                haySeleccion = false;
                                return;
                            }

                            combatePendiente = true;

                            // Guardamos quién es el que ataca y quién se defiende
                            atacantePendiente = casillas[filaSel][colSel];
                            defensorPendiente = casillas[filaClic][columnaClic];

                            // Quitamos la selección del tablero para que no se quede iluminado
                            haySeleccion = false;

                            // ¡OJO! No cambiamos el turno ni movemos la ficha todavía. 
                            // Dejamos que el Mundo.cpp se dé cuenta de que combatePendiente es true, 
                            // y nos lleve a la pantalla de la Arena.

                        }
                    }
                    else {
                        // Haces clic en una casilla donde hay OTRA ficha de tu propio equipo.
                        // Simplemente soltamos la selección para que puedas agarrar otra cosa.
                        haySeleccion = false;
                    }
                }
            }
        }
    }
    else if (boton == 2) {
        haySeleccion = false;
        // Al poner esto a false, la ficha se cae de la mano del jugador
    }
}

bool Tablero::esPuntoDePoder(int f, int c) const {
    // El centro absoluto
    if (f == 4 && c == 4) return true;
    // Los cuatro centros de los bordes
    if (f == 0 && c == 4) return true; // Borde superior
    if (f == 8 && c == 4) return true; // Borde inferior
    if (f == 4 && c == 0) return true; // Borde izquierdo
    if (f == 4 && c == 8) return true; // Borde derecho

    return false; // Si no es ninguno de esos, es una casilla normal
}

bool Tablero::esCasillaOscilante(int f, int c) const {
    // La cruz central (Fila 4 entera o Columna 4 entera)
    if (f == 4 || c == 4) return true;

    // El rombo mágico 
    if (std::abs(f - 4) + std::abs(c - 4) == 4) return true;

    return false;
}

int Tablero::comprobarVictoria() {
    int fichasSaludables = 0;
    int fichasBasura = 0;

    int poderSaludables = 0;
    int poderBasura = 0;

    int encarceladasSaludables = 0;
    int encarceladasBasura = 0;

    // Recorre el tablero contando quién está vivo y dónde
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Si hay una ficha en esta casilla...
            if (casillas[i][j] != NULL) {

                if (casillas[i][j]->bando == SALUDABLE) {
                    fichasSaludables++; // Sumamos 1 vivo
                    if (esPuntoDePoder(i, j)) poderSaludables++; // Está en la cruz?
                    if (casillas[i][j]->encarcelada) encarceladasSaludables++;
                }
                else if (casillas[i][j]->bando == BASURA) {
                    fichasBasura++;
                    if (esPuntoDePoder(i, j)) poderBasura++;
                    if (casillas[i][j]->encarcelada) encarceladasBasura++;
                }

            }
        }
    }

    // ganar por comer
    if (fichasSaludables == 0) return 2; // Gana la Basura
    if (fichasBasura == 0) return 1;     // Gana la Sana

    // ganar por dominar 
    if (poderSaludables == 5) return 1; // Gana la Sana
    if (poderBasura == 5) return 2;     // Gana la Basura

    // ULTIMA PIEZA ENCARCELADA ---

    if (fichasSaludables == 1 && encarceladasSaludables == 1) return 2; // Gana la Basura
    if (fichasBasura == 1 && encarceladasBasura == 1) return 1;         // Gana la Sana

    // Si nadie cumple nada de lo anterior, la partida sigue
    return 0;
}

void Tablero::gestionTeclado(unsigned char tecla, int x, int y) {

    if (tecla == ' ') {
        // Solo dejamos abrir el menú si hay una ficha seleccionada
        if (haySeleccion && casillas[filaSel][colSel]->tipo == LIDER) {
            menuMagiaActivo = !menuMagiaActivo;

            // Si lo acabamos de abrir, empezamos siempre por el primer hechizo 
            if (menuMagiaActivo) {
                hechizoSeleccionado = 0;
            }
        }
    }

    // 2. TECLA ENTER 
    if (tecla == 13) {
        if (menuMagiaActivo) {

            // De quién es el turno? Cogemos su libreta de hechizos
            bool* libretaHechizos;
            if (turnoActual == SALUDABLE) libretaHechizos = hechizosSanaUsados;
            else libretaHechizos = hechizosBasuraUsados;

            // Comprobamos si este hechizo ya se gastó en esta partida
            if (libretaHechizos[hechizoSeleccionado] == true) {
                // Ya está usad, Aquí se peude reproducir un sonido de error.
                return;
            }

            // LANZAMOS EL HECHIZO
            switch (hechizoSeleccionado) {
            
            case 0: // HECHIZO 1: TELETRANSPORTE (Requiere ratón)
                menuMagiaActivo = false;
                esperandoObjetivo = true;
                pasoTeletransporte = 0; // Empezamos por elegir pieza
                piezaParaTeletransporte = NULL;

                if (turnoActual == SALUDABLE) hechizosSanaUsados[hechizoSeleccionado] = true;
                else hechizosBasuraUsados[hechizoSeleccionado] = true;
                break;;
            
            case 1: // HECHIZO 2: CURAR (Requiere ratón)
                // 1. Cerramos menú
                    menuMagiaActivo = false;
                // 2. Activamos el modo "espera de clic"
                esperandoObjetivo = true;
                // 3. Marcamos como usado en la libreta del bando actual
                if (turnoActual == SALUDABLE) hechizosSanaUsados[hechizoSeleccionado] = true;
                else hechizosBasuraUsados[hechizoSeleccionado] = true;
                break;


            case 2: // HECHIZO 3: CAMBIAR TIEMPO 

                turnosTotales += 2;

                // Lo marcamos como gastado, cerramos el menú y pasamos turno
                libretaHechizos[hechizoSeleccionado] = true;
                menuMagiaActivo = false;
                haySeleccion = false; // Soltamos al líder
                if (turnoActual == SALUDABLE) turnoActual = BASURA;
                else turnoActual = SALUDABLE;
                turnosTotales++; // El turno avanza de forma normal
                break;

            
            case 3: // HECHIZO 4: INTERCAMBIO (Requiere ratón)
                menuMagiaActivo = false;
                esperandoObjetivo = true;
                primeraPiezaIntercambio = NULL; // Reset por seguridad
                if (turnoActual == SALUDABLE) hechizosSanaUsados[3] = true;
                else hechizosBasuraUsados[3] = true;
                break;
            
            case 4: // HECHIZO 5: INVOCAR ELEMENTAL (Requiere ratón)
                menuMagiaActivo = false;
                esperandoObjetivo = true;

                // Marcamos como usado
                if (turnoActual == SALUDABLE) hechizosSanaUsados[4] = true;
                else hechizosBasuraUsados[4] = true;
                break;
            
            case 5: // HECHIZO 6: REVIVIR (Requiere ratón)
                if ((turnoActual == SALUDABLE && !bajasSaludables.empty()) ||
                    (turnoActual == BASURA && !bajasBasura.empty())) {

                    menuMagiaActivo = false;
                    esperandoObjetivo = true;

                    if (turnoActual == SALUDABLE) hechizosSanaUsados[5] = true;
                    else hechizosBasuraUsados[5] = true;
                }
                break;
            
            case 6: // HECHIZO 7: ENCARCELAR (Requiere ratón)
                menuMagiaActivo = false;
                esperandoObjetivo = true;
                if (turnoActual == SALUDABLE) {
                    hechizosSanaUsados[6] = true;
                }
                else {
                    hechizosBasuraUsados[6] = true;
                }
                break;
            }
        }
    }
}


void Tablero::gestionTeclasEspeciales(int tecla, int x, int y) {

    // Las flechas SOLO funcionan si el menú de magia está abierto
    if (menuMagiaActivo) {

        // FLECHA DERECHA (Avanzar)
        if (tecla == GLUT_KEY_RIGHT) {
            hechizoSeleccionado++;
            // Si nos pasamos del último hechizo (el 6), volvemos al primero (el 0)
            if (hechizoSeleccionado > 6) {
                hechizoSeleccionado = 0;
            }
        }

        // FLECHA IZQUIERDA 
        else if (tecla == GLUT_KEY_LEFT) {
            hechizoSeleccionado--;
            // Si retrocedemos antes del 0, nos vamos al último (el 6)
            if (hechizoSeleccionado < 0) {
                hechizoSeleccionado = 6;
            }
        }
    }
}



Tablero::~Tablero() {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            if (casillas[i][j] != nullptr) delete casillas[i][j];
        }
    }


    // 2. Borrar piezas que se quedaron en la lista de bajas (Paso 2)
    for (int i = 0; i < bajasSaludables.size(); i++) {
        delete bajasSaludables[i];
    }
    bajasSaludables.clear();

    for (int i = 0; i < bajasBasura.size(); i++) {
        delete bajasBasura[i];
    }
    bajasBasura.clear();
}

void Tablero::resolverCombate(int ganador) {
    // Sacamos las coordenadas de dónde venía el atacante y dónde estaba el defensor
    int fOrigen = atacantePendiente->fila;
    int cOrigen = atacantePendiente->columna;
    int fDestino = defensorPendiente->fila;
    int cDestino = defensorPendiente->columna;

    if (ganador == 1) {
        //  GANA EL ATACANTE 
        //  Mandamos al DEFENSOR a las bajas
        if (defensorPendiente->getBando() == SALUDABLE) {
            bajasSaludables.push_back(defensorPendiente);
        }
        else {
            bajasBasura.push_back(defensorPendiente);
        }

        //  Movemos al atacante a la casilla que acaba de ganar
        casillas[fDestino][cDestino] = atacantePendiente;
        casillas[fOrigen][cOrigen] = NULL;

        // Actualizamos sus coordenadas internas
        atacantePendiente->fila = fDestino;
        atacantePendiente->columna = cDestino;
    }
    else if (ganador == 2) {
        //  GANA EL DEFENSOR 
        if (atacantePendiente->getBando() == SALUDABLE) {
            bajasSaludables.push_back(atacantePendiente);
        }
        else {
            bajasBasura.push_back(atacantePendiente);
        }

        casillas[fOrigen][cOrigen] = NULL;

        // El defensor se queda donde estaba
    }

    // Limpiamos los punteros de la pelea para el futuro
    atacantePendiente = NULL;
    defensorPendiente = NULL;

    // cambio de turno
    if (turnoActual == SALUDABLE) turnoActual = BASURA;
    else turnoActual = SALUDABLE;

    turnosTotales++;
}

InfoFicha Tablero::obtenerInfoRaton(float mouseX, float mouseY) {
    InfoFicha info;
    info.activa = false;

    // 1. Márgenes de tu función dibuja()
    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;

    // 2. Cálculo de posición relativa dentro del tablero
    // Usar el mismo sistema que gestionRaton (SIN invertir Y)
    float posX = mouseX - offsetX;
    float posY = mouseY - offsetY;

    // 3. Verificación: ¿Está el ratón dentro de los límites del tablero?
    float tableroAncho = 9.0f * ladoCasilla;
    float tableroAlto = 9.0f * ladoCasilla;

    if (posX < 0.0f || posX > tableroAncho || posY < 0.0f || posY > tableroAlto) {
        return info; // Fuera del tablero, no hay información
    }

    // 4. Cálculo de índices usando 50.0f (el tamaño que usas al dibujar)
    // Consistente con gestionRaton
    int i = (int)(posX / 50.0f);
    int j = (int)(posY / 50.0f);

    // 5. Verificación final de límites
    if (i >= 0 && i < 9 && j >= 0 && j < 9) {
        if (casillas[i][j] != nullptr) {
            info.nombre = casillas[i][j]->nombre;          
            info.vidaActual = casillas[i][j]->vidaActual;  
            info.vidaMax = casillas[i][j]->vidaMax;        
            info.activa = true;
            info.ataque = casillas[i][j]->ataque;          
        }
    }
    return info;
}
void Tablero::jugarTurnoIA() {
    struct Movimiento {
        Comida* ficha;
        int fDest, cDest;
        int puntuacion; // Aquí guardaremos la priotidad de  movimiento
    };

    std::vector<Movimiento> movimientosPosibles;

    // 1. ESCANEAR TABLERO Y PUNTUAR TODOS LOS MOVIMIENTOS
   
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Si la ficha es mía y no está en la cárcel
            if (casillas[i][j] != NULL && casillas[i][j]->getBando() == bandoIA && !casillas[i][j]->estaEncarcelada) {

                Comida* miFicha = casillas[i][j];
                bool pocaVida = (miFicha->vidaActual <= miFicha->vidaMax / 3); // twengo un tercio de vida?

                // Probamos a dónde puede ir
                for (int f = 0; f < 9; f++) {
                    for (int c = 0; c < 9; c++) {

                        if (miFicha->intentarMover(f, c)) {
                            Movimiento m;
                            m.ficha = miFicha;
                            m.fDest = f;
                            m.cDest = c;
                            m.puntuacion = 0; // Empezamos con 0 puntos

                            // APLICAMOS LAS 5 REGLAS DE INTELIGENCIA

                            // CASO A: LA CASILLA ESTÁ VACÍA (Movimiento)
                            if (casillas[f][c] == NULL) {
                                m.puntuacion += 10; // Moverse siempre suma algo

                                //  REGLA 1: DOMINIO DE LOS PUNTOS DE PODER 
                                if (esPuntoDePoder(f, c)) {
                                    m.puntuacion += 50; // Es importantísimo cogerlos

                                    //  REGLA 2: SUPERVIVENCIA (Curación) 
                                    // Si estoy medio muerto y esto es un punto de poder, PRIORIDAD CURARSE
                                    if (pocaVida) m.puntuacion += 80;
                                }

                                //  REGLA 3: CONTROL DEL CENTRO 
                                // dominar el centro del tablero (casilla 4,4) da ventaja táctica.
                                int distanciaAlCentro = abs(f - 4) + abs(c - 4);
                                m.puntuacion += (10 - distanciaAlCentro); // Cuanto más cerca del centro, más puntos suma

                            }
                            // CASO B: HAY UN ENEMIGO (Combate)
                            else if (casillas[f][c]->getBando() != bandoIA) {
                                Comida* enemigo = casillas[f][c];

                                //  REGLA 4: COMBATE CON CABEZA
                                if (miFicha->vidaActual > enemigo->vidaActual + 20) {
                                    m.puntuacion += 70; // Soy mucho más fuerte -> ¡ATACAR!
                                }
                                else if (miFicha->vidaActual >= enemigo->vidaActual) {
                                    m.puntuacion += 40; // Combate igualado -> Buena opción
                                }
                                else {
                                    m.puntuacion -= 30; // El enemigo es más fuerte -> HUIR (Resta puntos)
                                }

                                //  REGLA 5: IR A POR DEBILES 
                                // Si el enemigo está a punto de morir (menos de 20 de vida), 
                                // ignoramos los riesgos y vamos a rematarlo para quitarle fichas al jugador.
                                if (enemigo->vidaActual <= 20) {
                                    m.puntuacion += 100; // Prioridad ABSOLUTA
                                }
                            }

                            // Si el movimiento no es un suicidio total, lo añadimos a la bolsa
                            if (m.puntuacion > 0) {
                                movimientosPosibles.push_back(m);
                            }
                        }
                    }
                }
            }
        }
    }

    // 2. ELEGIR EL MOVIMIENTO CON LA NOTA MÁS ALTA
    if (!movimientosPosibles.empty()) {

        // Buscamos cuál es la nota más alta que hemos conseguido
        int notaMaxima = 0;
        for (auto& m : movimientosPosibles) {
            if (m.puntuacion > notaMaxima) notaMaxima = m.puntuacion;
        }

        // Guardamos todos los movimientos que hayan sacado esa nota máxima (puede haber empates)
        std::vector<Movimiento> losMejores;
        for (auto& m : movimientosPosibles) {
            if (m.puntuacion == notaMaxima) losMejores.push_back(m);
        }

        // De los mejores, elegimos uno a cara o cruz para que la IA no haga SIEMPRE lo mismo
        Movimiento elegido = losMejores[rand() % losMejores.size()];

        // 3. EJECUTAR EL MOVIMIENTO ELEGIDO
        if (casillas[elegido.fDest][elegido.cDest] == NULL) {

            // Movimiento pacífico a hueco vacío
            int fOrg = elegido.ficha->fila;
            int cOrg = elegido.ficha->columna;

            casillas[elegido.fDest][elegido.cDest] = elegido.ficha;
            casillas[fOrg][cOrg] = NULL;
            elegido.ficha->fila = elegido.fDest;
            elegido.ficha->columna = elegido.cDest;

            // Cambiamos de turno
            if (turnoActual == SALUDABLE) turnoActual = BASURA;
            else turnoActual = SALUDABLE;
            turnosTotales++;

        }
        else {
            // ¡ATAQUE! 
            combatePendiente = true;
            atacantePendiente = elegido.ficha;
            defensorPendiente = casillas[elegido.fDest][elegido.cDest];

            //aqui nose pasa de turno porque entraria en modo combate

        }

    }
    else {
        // Seguro anti-cuelgues: Si la IA está bloqueada por todas partes, pasa el turno.
        if (turnoActual == SALUDABLE) turnoActual = BASURA;
        else turnoActual = SALUDABLE;
        turnosTotales++;
    }
}

bool Tablero::caminoLibre(int fOrg, int cOrg, int fDest, int cDest) {
    int pasoFila = (fDest > fOrg) ? 1 : (fDest < fOrg ? -1 : 0);
    int pasoCol = (cDest > cOrg) ? 1 : (cDest < cOrg ? -1 : 0);

    int fActual = fOrg + pasoFila;
    int cActual = cOrg + pasoCol;

    while (fActual != fDest || cActual != cDest) {
        if (casillas[fActual][cActual] != nullptr) return false;
        fActual += pasoFila;
        cActual += pasoCol;
    }
    return true;
}