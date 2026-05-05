#include "Tablero.h"
#include "Definiciones.h"
#include <iostream>
Tablero::Tablero():
    fondo_tablero("imagenes/fondo_tablero.png")
{
    ladoCasilla = 50.0f; 
    fondo_tablero.setPos(0, 0);
    fondo_tablero.setSize(800, 600);
    fondo_tablero.setCenter(0, 0);
    inicializa();
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
        casillas[7][j] = new Comida(FAST_FOOD, DISTANCIA, 7, j);
    }

    // 6. Línea trasera Fast Food (Columna 8)
    casillas[8][0] = new Comida(FAST_FOOD, VOLADORA, 8, 0);
    casillas[8][1] = new Comida(FAST_FOOD, PESADA, 8, 1);
    casillas[8][2] = new Comida(FAST_FOOD, ESPECIAL, 8, 2);
    casillas[8][3] = new Comida(FAST_FOOD, PESADA, 8, 3);
    casillas[8][4] = new Comida(FAST_FOOD, LIDER, 8, 4);     // Rey
    casillas[8][5] = new Comida(FAST_FOOD, PESADA, 8, 5);
    casillas[8][6] = new Comida(FAST_FOOD, ESPECIAL, 8, 6);
    casillas[8][7] = new Comida(FAST_FOOD, PESADA, 8, 7);
    casillas[8][8] = new Comida(FAST_FOOD, VOLADORA, 8, 8);

}

void Tablero::dibuja(bool pausaActiva) {
    glEnable(GL_TEXTURE_2D);
    fondo_tablero.draw();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f;
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
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
                casillas[i][j]->dibuja(xMin, yMin, ladoCasilla);
            }
        }
    }
}


void Tablero::gestionRaton(int x, int y, bool pausaActiva) {
    // 0. EL MURO DE PAUSA
    if (pausaActiva) {
        return; // Si el juego está pausado, ignoramos el clic y salimos
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

        if (!haySeleccion) {
            // ESTADO 1: No tenemos nada agarrado. Intentamos agarrar una ficha.
            if (casillas[filaClic][columnaClic] != NULL) { // Si hay una ficha aquí...
                haySeleccion = true;
                filaSel = filaClic;
                colSel = columnaClic;
            }
        }
        else {
            // ESTADO 2: Ya teníamos una ficha agarrada. Intentamos moverla al nuevo clic.

            // Si hacemos clic en la misma casilla, se cancela
            if (filaClic == filaSel && columnaClic == colSel) {
                haySeleccion = false;
            }
            // Si hacemos clic en una casilla vacía, movemos la ficha
            else if (casillas[filaClic][columnaClic] == NULL) {

                // ¡LA PREGUNTA CLAVE! Le pasamos las coordenadas destino a la ficha seleccionada
                if (casillas[filaSel][colSel]->intentarMover(filaClic, columnaClic) == true) {

                    //  EL MOVIMIENTO ES VÁLIDO 
                    // Movemos el puntero a la nueva casilla
                    casillas[filaClic][columnaClic] = casillas[filaSel][colSel];
                    // Vaciamos la casilla original
                    casillas[filaSel][colSel] = NULL;

                    // Actualizamos las coordenadas internas del objeto Comida
                    casillas[filaClic][columnaClic]->fila = filaClic;
                    casillas[filaClic][columnaClic]->columna = columnaClic;

                    // Soltamos la selección para el siguiente turno
                    haySeleccion = false;
                }
                else {
                    // El movimiento no es posbile
                    // No hacemos nada, la ficha sigue seleccionada esperando un clic válido.
                }
            }
            // (Más adelante añadiremos aquí el "else" para cuando haces clic en un enemigo = COMBATE)
        }
    }
}