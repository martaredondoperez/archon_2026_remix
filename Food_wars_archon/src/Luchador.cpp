#include "Luchador.h"

Luchador::Luchador() {
    vida = 100;       // Vida inicial por defecto
    ataque = 0;       // Ataque inicial por defecto 
    radio = 10.0f;       // Radio para futuras colisiones (acorde al dibujo)

    // lo pongo en el centro exacto de la pantalla (800x600)
    posicion.x = 400.0f;
    posicion.y = 300.0f;

    // empieza quieto
    velocidad.x = 0.0f;
    velocidad.y = 0.0f;
}

void Luchador::dibuja() {
    glPushMatrix();

    // me muevo a la posicion actual del bicho
    glTranslatef(posicion.x, posicion.y, 0);
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo (mi luchador provisional de Healthy)

    // dibujo el cuadradito, ahora más grande para la escala 800x600
    glBegin(GL_POLYGON);
    glVertex2f(-10.0f, -10.0f);
    glVertex2f(10.0f, -10.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(-10.0f, 10.0f);
    glEnd();

    glPopMatrix();
}

// le paso el dt y los topes de la arena para que no se escape
void Luchador::mueve(float t, float limiteX, float limiteY) {
    // 1. actualizo la posicion basandome en la velocidad (cinematica basica)
    posicion.x += velocidad.x * t;
    posicion.y += velocidad.y * t;

    // 2. FISICA SIMPLE: Choques contra las paredes horizontales
    if (posicion.x > limiteX - radio) {
        posicion.x = limiteX - radio; // si me paso por la derecha, me clavo en el borde
    }
    else if (posicion.x < radio) {
        posicion.x = radio; // la pared izquierda ahora es el 0
    }

    // 3. Choques contra las paredes verticales
    if (posicion.y > limiteY - radio) {
        posicion.y = limiteY - radio ; // techo
    }
    else if (posicion.y < radio) {
        posicion.y = radio; // suelo
    }
}