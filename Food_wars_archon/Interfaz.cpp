#include "Interfaz.h"
#include "freeglut.h"
void Interfaz::dibujaMenu() {
    dibujaBoton(300, 350, 200, 60, "1 JUGADOR");
    dibujaBoton(300, 250, 200, 60, "2 JUGADORES");
}

void Interfaz::dibujaSeleccion() {
    dibujaBoton(200, 300, 150, 60, "HEALTHY");
    dibujaBoton(450, 300, 150, 60, "JUNK");
}

void Interfaz::dibujaPausa() {}
void Interfaz::dibujaFinal() {}

void Interfaz::dibujaBoton(float x, float y, float ancho, float alto, const char* texto) {
    glDisable(GL_LIGHTING); // Por si acaso la iluminación está activada
    glColor3f(1.0f, 1.0f, 1.0f); // Blanco puro

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto);
    glVertex2f(x, y + alto);
    glEnd();
}