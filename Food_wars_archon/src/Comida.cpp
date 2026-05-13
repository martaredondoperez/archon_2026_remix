#include "Comida.h"
#include "freeglut.h"
#include <iostream>



// La lógica de dibujo es común para casi todas (todas usan sprites y efectos)
void Comida::dibuja(float xMin, float yMin, float lado) {
    // 1. Dibujo del Sprite
    if (sprite != nullptr) {
        glColor3ub(255, 255, 255);
        glEnable(GL_TEXTURE_2D);
        sprite->setPos(xMin + lado / 2.0f, yMin + lado / 2.0f);
        sprite->draw();
        glDisable(GL_TEXTURE_2D);
    }

    // 2. Efecto de Hechizo Encarcelada
    if (encarcelada || estaEncarcelada) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.7f, 1.0f, 0.5f); // Fondo azul mágico
        glBegin(GL_QUADS);
        glVertex2f(xMin, yMin);
        glVertex2f(xMin + lado, yMin);
        glVertex2f(xMin + lado, yMin + lado);
        glVertex2f(xMin, yMin + lado);
        glEnd();

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // X blanca
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glVertex2f(xMin, yMin); glVertex2f(xMin + lado, yMin + lado);
        glVertex2f(xMin + lado, yMin); glVertex2f(xMin, yMin + lado);
        glEnd();
        glPopAttrib();
    }

    // 3. Efecto de Selección para Hechizos
    if (seleccionadaParaHechizo) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1.0f, 0.8f, 0.0f, 0.4f); // Resalte dorado
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xMin, yMin); glVertex2f(xMin + lado, yMin);
        glVertex2f(xMin + lado, yMin + lado); glVertex2f(xMin, yMin + lado);
        glEnd();
        glPopAttrib();
    }
}

// Actualización de la secuencia del sprite
void Comida::actualiza(float t) {
    if (sprite) {
        sprite->loop();
    }
}

// Lógica de daño común
void Comida::recibirDano(int cantidad) {
    vidaActual -= cantidad;
    if (vidaActual < 0) vidaActual = 0;
}