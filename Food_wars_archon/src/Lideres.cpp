#include "Lideres.h"
#include "freeglut.h"

// --- CHEF MICHELIN ---
ChefMichelin::ChefMichelin() {
    vida = 150; // Vida de líder
    radio = 15.0f;
    ataque = 25;
    posicion.x = 200.0f; // Empieza a la izquierda
    posicion.y = 300.0f;
}

void ChefMichelin::dibuja() {
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, 0);
    glColor3f(0.0f, 0.8f, 0.2f); // Verde oscuro

    glBegin(GL_POLYGON);
    glVertex2f(-radio, -radio);
    glVertex2f(radio, -radio);
    glVertex2f(radio, radio);
    glVertex2f(-radio, radio);
    glEnd();
    glPopMatrix();
}

// --- REY DE LA HAMBURGUESA ---
ReyHamburguesa::ReyHamburguesa() {
    vida = 150; // Vida de líder
    radio = 15.0f;
    ataque = 25; 
    posicion.x = 600.0f; // Empieza a la derecha
    posicion.y = 300.0f;

    tiempoRecarga = 0.0f; // Inicia con el arma cargada
}

void ReyHamburguesa::dibuja() {
    glPushMatrix();
    glTranslatef(posicion.x, posicion.y, 0);
    glColor3f(0.8f, 0.0f, 0.0f); // Rojo

    glBegin(GL_POLYGON);
    glVertex2f(-radio, -radio);
    glVertex2f(radio, -radio);
    glVertex2f(radio, radio);
    glVertex2f(-radio, radio);
    glEnd();
    glPopMatrix();
}

bool ReyHamburguesa::piensaYMueve(float targetY) {
    // 1. Calculamos el error de posición (la diferencia de altura)
    float errorY = targetY - posicion.y;
    float umbral = 10.0f; // Nuestra banda muerta de tolerancia

    // 2. Lógica del Actuador
    if (errorY > umbral) {
        velocidad.y = 150.0f; // El objetivo está arriba, subimos
    }
    else if (errorY < -umbral) {
        velocidad.y = -150.0f; // El objetivo está abajo, bajamos
    }
    else {
        velocidad.y = 0.0f; // Estan alineados  
        return true;        // Enviamos la orden de FUEGO
    }

    return false; // Si nos estamos moviendo, no disparamos
}