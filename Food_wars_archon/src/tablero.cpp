#include "Tablero.h"

Tablero::Tablero() {
    ladoCasilla = 50.0f; 
    inicializa();
}

void Tablero::inicializa() {
    // 1. Limpiamos el tablero
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j] = 0;
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
        casillas[1][j] = 14;
    }

    // 4. Línea trasera Saludable (Columna 0) - Combinación de piezas fuertes
    casillas[0][0] = 12; // Voladora (Limón)
    casillas[0][1] = 13; // Pesada (Aguacate)
    casillas[0][2] = 11; // Especial (Jengibre)
    casillas[0][3] = 13; // Pesada (Aguacate)
    casillas[0][4] = 10; // HECHICERO (Chef Michelin) - 
    casillas[0][5] = 13; // Pesada (Aguacate)
    casillas[0][6] = 11; // Especial (Jengibre)
    casillas[0][7] = 13; // Pesada (Aguacate)
    casillas[0][8] = 12; // Voladora (Limón)

    // 5. Línea frontal Fast Food (Columna 7) - Todos Distancia (Disparo Ketchup)
    for (int j = 0; j < 9; j++) {
        casillas[7][j] = 24;
    }

    // 6. Línea trasera Fast Food (Columna 8) - Combinación simétrica
    casillas[8][0] = 22; // Voladora (Pizza)
    casillas[8][1] = 23; // Pesada (Hamburguesa)
    casillas[8][2] = 21; // Especial (Helado)
    casillas[8][3] = 23; // Pesada (Hamburguesa)
    casillas[8][4] = 20; // HECHICERO (Rey Hamburguesa) 
    casillas[8][5] = 23; // Pesada (Hamburguesa)
    casillas[8][6] = 21; // Especial (Helado)
    casillas[8][7] = 23; // Pesada (Hamburguesa)
    casillas[8][8] = 22; // Voladora (Pizza)

}

void Tablero::dibuja() {
    // Calculamos el centro de la pantalla (800x600) para un tablero de 450x450
    float offsetX = (800.0f - (9.0f * ladoCasilla)) / 2.0f; // 175 px de margen izquierdo
    float offsetY = (600.0f - (9.0f * ladoCasilla)) / 2.0f; // 75 px de margen inferior

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // 1. Elegir el color del fondo de la casilla
            if ((i + j) % 2 == 0) {
                glColor3ub(240, 230, 200); // Claro (tipo mantel)
            }
            else {
                glColor3ub(180, 50, 50);   // Oscuro (tipo cuadros rojos de restaurante italiano)
            }

            // Sobreescribir si es un Punto de Nutrición 
            if (puntosNutricion[i][j]) {
                glColor3ub(100, 255, 100); // Verde brillante para destacar la curación
            }

            // Calcular las esquinas sumando el offset
            float xMin = offsetX + (i * ladoCasilla);
            float xMax = offsetX + ((i + 1) * ladoCasilla);
            float yMin = offsetY + (j * ladoCasilla);
            float yMax = offsetY + ((j + 1) * ladoCasilla);

            // Dibujar la casilla rellenada
            glBegin(GL_QUADS);
            glVertex2f(xMin, yMin);
            glVertex2f(xMax, yMin);
            glVertex2f(xMax, yMax);
            glVertex2f(xMin, yMax);
            glEnd();

            // 4. Dibujar un borde negro sutil
            glColor3ub(30, 30, 30);
            glBegin(GL_LINE_LOOP);
            glVertex2f(xMin, yMin);
            glVertex2f(xMax, yMin);
            glVertex2f(xMax, yMax);
            glVertex2f(xMin, yMax);
            glEnd();
            //  VISUAL TEMPORAL PARA LAS FICHAS
            int valorCasilla = casillas[i][j];
            if (valorCasilla != 0) {
                // Calcular el centro exacto de la casilla
                float centroX = xMin + (ladoCasilla / 2.0f);
                float centroY = yMin + (ladoCasilla / 2.0f);

                // Dar color según el bando (Verde = Saludable, Rojo = Fast Food)
                if (valorCasilla >= 10 && valorCasilla < 20) {
                    glColor3ub(0, 150, 0); // Verde oscuro
                }
                else if (valorCasilla >= 20) {
                    glColor3ub(150, 0, 0); // Rojo oscuro
                }

                // Si es el líder, lo pintamos de un color especial (Oro/Amarillo)
                if (valorCasilla == 10 || valorCasilla == 20) {
                    glColor3ub(255, 215, 0);
                }

                // Dibujar un cuadrado pequeño que simule la ficha (temporal)
                float radioFicha = ladoCasilla * 0.3f;
                glBegin(GL_QUADS);
                glVertex2f(centroX - radioFicha, centroY - radioFicha);
                glVertex2f(centroX + radioFicha, centroY - radioFicha);
                glVertex2f(centroX + radioFicha, centroY + radioFicha);
                glVertex2f(centroX - radioFicha, centroY + radioFicha);
                glEnd();
            }
        }
    }
}