#include "BotonCircular.h"
#include <cmath>

BotonCircular::BotonCircular(float _cx, float _cy, float _rad, ETSIDI::Sprite* _sprite,
    float _r, float _g, float _b)
    : Boton(_cx, _cy, _r, _g, _b), radio(_rad), sprite(_sprite) {
}

void BotonCircular::dibuja() {
    float s, b;
    gestionarEfectos(s, b); // ¡Ahora el círculo también brilla y crece!

    glPushMatrix();
    glTranslatef(x, y, 0);

    // Aplicamos la escala al radio
    float radioEfectivo = radio * s;

    glColor3f(r + b, g + b, b + b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(radioEfectivo * cos(theta), radioEfectivo * sin(theta));
    }
    glEnd();

    if (sprite) {
        // El icono también crece con el botón
        sprite->setPos(-radioEfectivo * 0.7f, -radioEfectivo * 0.7f);
        sprite->setSize(radioEfectivo * 1.4f, radioEfectivo * 1.4f);
        sprite->draw();
    }
    glPopMatrix();
}

bool BotonCircular::isMouseOver(float mx, float my) {
    float distancia = std::sqrt(std::pow(mx - x, 2) + std::pow(my - y, 2));
    return (distancia <= radio);
}