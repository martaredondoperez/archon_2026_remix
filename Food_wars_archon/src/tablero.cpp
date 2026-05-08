#include "Tablero.h"
#include "Definiciones.h"
#include <iostream>
#include "freeglut.h"
#include "Interfaz.h"

Tablero::Tablero():
    fondo_tablero("imagenes/fondo_menu_principal.png")
{
    ladoCasilla = 50.0f; 
    fondo_tablero.setPos(0, 0);
    fondo_tablero.setSize(800, 600);
    fondo_tablero.setCenter(0, 0);
    inicializa();
}
void Tablero::setTurnoInicial(Bando bandoElegido) {
    turnoActual = bandoElegido;
}

void Tablero::inicializa() {
    // 1. Limpiamos el tablero
    haySeleccion = false;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j] = NULL;
            puntosNutricion[i][j] = false;
        }
    }

    // 2. Definimos los 5 Puntos de Nutrición (centro y bordes)
    puntosNutricion[4][4] = true; // Centro exacto
    puntosNutricion[0][4] = true; // Borde inferior
    puntosNutricion[8][4] = true; // Borde superior
    puntosNutricion[4][0] = true; // Borde izquierdo
    puntosNutricion[4][8] = true; // Borde derecho
    // 3. Línea frontal Saludable (Columna 1) - Todos Distancia (Chorro de Vitaminas)
    for (int j = 0; j < 9; j++) {
        casillas[1][j] = new Comida(SALUDABLE, DISTANCIA, 1, j);
    }

    // 4. Línea trasera Saludable (Columna 0)
    casillas[0][0] = new Comida(SALUDABLE, VOLADORA, 0, 0);
    casillas[0][1] = new Comida(SALUDABLE, PESADA, 0, 1);
    casillas[0][2] = new Comida(SALUDABLE, ESPECIAL, 0, 2);
    casillas[0][3] = new Comida(SALUDABLE, PESADA, 0, 3);
    casillas[0][4] = new Comida(SALUDABLE, LIDER, 0, 4);     // Chef
    casillas[0][5] = new Comida(SALUDABLE, PESADA, 0, 5);
    casillas[0][6] = new Comida(SALUDABLE, ESPECIAL, 0, 6);
    casillas[0][7] = new Comida(SALUDABLE, PESADA, 0, 7);
    casillas[0][8] = new Comida(SALUDABLE, VOLADORA, 0, 8);

    // 5. Línea frontal Fast Food (Columna 7)
    for (int j = 0; j < 9; j++) {
        casillas[7][j] = new Comida(BASURA, DISTANCIA, 7, j);
    }

    // 6. Línea trasera Fast Food (Columna 8)
    casillas[8][0] = new Comida(BASURA, VOLADORA, 8, 0);
    casillas[8][1] = new Comida(BASURA, PESADA, 8, 1);
    casillas[8][2] = new Comida(BASURA, ESPECIAL, 8, 2);
    casillas[8][3] = new Comida(BASURA, PESADA, 8, 3);
    casillas[8][4] = new Comida(BASURA, LIDER, 8, 4);     // Rey
    casillas[8][5] = new Comida(BASURA, PESADA, 8, 5);
    casillas[8][6] = new Comida(BASURA, ESPECIAL, 8, 6);
    casillas[8][7] = new Comida(BASURA, PESADA, 8, 7);
    casillas[8][8] = new Comida(BASURA, VOLADORA, 8, 8);

    //Al empezar nadie ha ganado
    ganadorFinal = 0;
}

void Tablero::dibuja(bool pausaActiva) {
    // 1. FONDO
    glEnable(GL_TEXTURE_2D);
    fondo_tablero.draw();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // Desvincula la textura del fondo

    // 2. CÁMARA (Esto ya lo tienes y funciona, no lo toques)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

            // Colores normales (sin el gris de pausa aquí)
            if (puntosNutricion[i][j]) glColor3ub(100, 255, 100);
            else if ((i + j) % 2 == 0) glColor3ub(240, 230, 200);
            else glColor3ub(180, 50, 50);

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
                glEnable(GL_TEXTURE_2D); // Activamos para la ficha

                casillas[i][j]->dibuja(xMin, yMin, ladoCasilla);
                
                glBindTexture(GL_TEXTURE_2D, 0); 
                glDisable(GL_TEXTURE_2D); // Desactivamos tras dibujar la ficha
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
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {

                    // PUEDE IR?
                    if (casillas[i][j] == NULL && casillas[filaSel][colSel]->intentarMover(i, j)) {


                        float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
                        float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;

                        float xMin = offsetX + (i * ladoCasilla);
                        float yMin = offsetY + (j * ladoCasilla);
                        float xMax = xMin + ladoCasilla;
                        float yMax = yMin + ladoCasilla;

                        glBegin(GL_QUADS);
                        glVertex2f(xMin, yMin);
                        glVertex2f(xMax, yMin);
                        glVertex2f(xMax, yMax);
                        glVertex2f(xMin, yMax);
                        glEnd();
                    }
                }
            }
            // Desactivamos la transparencia para no afectar al resto del juego
            glDisable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
          

        }

     

        glColor3f(1.0f, 1.0f, 0.0f);

        
        glRasterPos2f(10.0f, 570.0f);

        std::string mensaje;
        if (turnoActual == SALUDABLE) { 
            mensaje = "TURNO: HEALTHY";
        }
        else {
            mensaje = "TURNO: JUNK";
        }

        for (int i = 0; i < mensaje.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, mensaje[i]);
        }

        

    }

}
void Tablero::gestionRaton(int boton, int x, int y, bool pausaActiva) {
    // 0. BLOQUEO: Si ya hay un ganadorFinal o pausa , no permitimos más movimientos
    if (pausaActiva || ganadorFinal != 0) {
        return;
    }

    // 1. Invertir el eje Y 
    int y_gl = 600 - y;

    //  Recuperar los márgenes 
    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;

    //  Convertir píxeles a índices de la matriz (0 a 8)
    int filaClic = (x - offsetX) / ladoCasilla;
    int columnaClic = (y_gl - offsetY) / ladoCasilla;

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

                // Si hacemos clic en la misma casilla, se cancela
                if (filaClic == filaSel && columnaClic == colSel) {
                    haySeleccion = false;
                    // ¡AQUÍ NO SE CAMBIA EL TURNO! Solo la soltamos.
                }

                // CASO B: Mover a una casilla vacía
                else if (casillas[filaClic][columnaClic] == NULL) {

                    // Comprobamos si el movimiento es válido según las reglas de la ficha
                    if (casillas[filaSel][colSel]->intentarMover(filaClic, columnaClic) == true) {


                        casillas[filaClic][columnaClic] = casillas[filaSel][colSel];
                        casillas[filaSel][colSel] = NULL;
                        casillas[filaClic][columnaClic]->fila = filaClic;
                        casillas[filaClic][columnaClic]->columna = columnaClic;

                        haySeleccion = false;

                        // CHEQUEO DE VICTORIA TRAS MOVIMIENTO 
                        int resultado = chequearVictoria(); 
                        if (resultado != 0) {   
                            ganadorFinal = resultado;
                            std::string nombre = (ganadorFinal == SALUDABLE) ? "HEALTHY" : "JUNK FOOD";
                            std::cout << "TENEMOS UN GANADOR: " << nombre << std::endl;
                        }

                        // SOLO PASA TURNO SI HUBO MOV
                        if (turnoActual == SALUDABLE) {
                            turnoActual = BASURA;
                        }
                        else {
                            turnoActual = SALUDABLE;
                        }
                    }
                }
                else {
                    // La ficha que hay ahí es del enemigo?
                    if (casillas[filaClic][columnaClic]->bando != turnoActual) {

                        // Mi ficha tiene rango suficiente para llegar hasta ahí y atacar?
                        if (casillas[filaSel][colSel]->intentarMover(filaClic, columnaClic) == true) {

                            // de mnomento comemos fcha

                            delete casillas[filaClic][columnaClic];

                            //mueve ficha nuestra
                            casillas[filaClic][columnaClic] = casillas[filaSel][colSel];
                            casillas[filaSel][colSel] = NULL;

                            casillas[filaClic][columnaClic]->fila = filaClic;
                            casillas[filaClic][columnaClic]->columna = columnaClic;

                            haySeleccion = false;

                            // CHEQUEO DE CCTORIA TRAS COMER FICHA
                            int ganador = chequearVictoria(); 
                            if (ganador != 0) {              
                                std::cout << "PARTIDA FINALIZADA. GANADOR: " << ganador << std::endl; 
                            }

                            if (turnoActual == SALUDABLE) {
                                turnoActual = BASURA;
                            }
                            else {
                                turnoActual = SALUDABLE;
                            }
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

int Tablero::chequearVictoria() {
    int piezasSaludables = 0;
    int piezasFastFood = 0;
    int puntosSaludables = 0;
    int puntosFastFood = 0;

    // Recorremos el tablero una sola vez para contar todo
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (casillas[i][j] != NULL) {
                // CONTAR PIEZAS (Condición 2 y 3)
                if (casillas[i][j]->getBando() == SALUDABLE) piezasSaludables++;
                else piezasFastFood++;

                // CONTAR PUNTOS DE PODER (Condición 1)
                // Si la casilla es un punto de nutrición y hay alguien encima
                if (puntosNutricion[i][j]) {
                    if (casillas[i][j]->getBando() == SALUDABLE) {
                        puntosSaludables++;
                        std::cout << "Saludable en punto (" << i << "," << j << ")" << std::endl;
                    }
                    else if (casillas[i][j]->getBando() == BASURA) {
                        puntosFastFood++;
                        std::cout << "Basura en punto (" << i << "," << j << ")" << std::endl;
                    }
                }
            }
        }
    }
    // Chivato para la consola
    std::cout << "TOTAL PUNTOS SALUDABLES: " << puntosSaludables << "/5" << std::endl;

    // CONDICIÓN 1: CONTROLAR LOS 5 PUNTOS DE PODER 
    if (puntosSaludables == 5) return SALUDABLE;
    if (puntosFastFood == 5) return BASURA;

    //CONDICIÓN 2: ELIMINAR TODAS LAS PIEZAS 
    if (piezasFastFood == 0) return SALUDABLE;
    if (piezasSaludables == 0) return BASURA;

    // CONDICIÓN 3: DEJAR AL RIVAL CON SOLO UNA PIEZA ENCARCELADA
    // De momento, como no tienes hechizos, esta condición no se cumplirá nunca.
    // Pero la dejamos lista para cuando implementes el hechizo "Imprison".

    bool piezaBasuraEstaEncarcelada = false; // Esto cambiará con el hechizo
    bool piezaSaludableEstaEncarcelada = false;

    if (piezasFastFood == 1 && piezaBasuraEstaEncarcelada) return SALUDABLE;
    if (piezasSaludables == 1 && piezaSaludableEstaEncarcelada) return BASURA;

    return 0; // Nadie ha ganado todavía
}