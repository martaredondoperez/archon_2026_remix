#include "Proyectil.h"
#include "freeglut.h"

Proyectil::Proyectil() {
    radio = 5.0f;        // Tamaño del disparo (más pequeño que el luchador)
    posicion.x = 0.0f;   // Se inicializan a 0
    posicion.y = 0.0f;
    velocidad.x = 0.0f;
    velocidad.y = 0.0f;
}

void Proyectil::dibuja() {
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, 0); // Mueve el dibujo a su posición real

    glColor3f(1.0f, 0.0f, 0.0f); // Color rojo (como un tomate o kétchup)

    // Dibujamos un pequeño cuadrado para el proyectil
    glBegin(GL_POLYGON);
    glVertex2f(-radio, -radio);
    glVertex2f(radio, -radio);
    glVertex2f(radio, radio);
    glVertex2f(-radio, radio);
    glEnd();

    glPopMatrix();
}

void Proyectil::mueve(float t) {
    // La misma lógica que el luchador, pero sin límites de pantalla
    // porque el proyectil debe poder salir de ella.
    posicion.x += velocidad.x * t;
    posicion.y += velocidad.y * t;
}