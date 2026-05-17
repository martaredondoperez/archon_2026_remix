#include "BotonCircular.h"
#include <cmath>
#include <algorithm> // Para usar std::min

BotonCircular::BotonCircular(float _cx, float _cy, float _rad, ETSIDI::Sprite* _sprite,
    float _r, float _g, float _b, std::string _t)
    : Boton(_cx, _cy, _r, _g, _b, _t), radio(_rad), sprite(_sprite) {
}

void BotonCircular::dibuja() {
    // 1. Salvaguardar todos los estados de OpenGL para que este botón no afecte a nada más
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    float s, brillo;
    gestionarEfectos(s, brillo); // ¡Ahora el círculo también brilla y crece!

    glPushMatrix();
    glTranslatef(x, y, 0);

    // Aplicamos la escala al radio
    float radioEfectivo = radio * s;

    // Desactivamos texturas para pintar el fondo de color plano
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Usar los componentes de color del objeto y sumar el brillo (limitando a un máximo de 1.0f)
    glColor3f(std::min(this->r + brillo, 1.0f),
        std::min(this->g + brillo, 1.0f),
        std::min(this->b + brillo, 1.0f));

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(radioEfectivo * cos(theta), radioEfectivo * sin(theta));
    }
    glEnd();

    if (sprite) {
        // CORRECCIÓN CRUCIAL: Volver a activar las texturas para el Sprite de ETSIDI
        glEnable(GL_TEXTURE_2D);

        // Forzamos color blanco para que la textura no se tiña del color del círculo
        glColor3f(1.0f, 1.0f, 1.0f);

        // El icono también crece con el botón
        sprite->setPos(-radioEfectivo * 0.7f, -radioEfectivo * 0.7f);
        sprite->setSize(radioEfectivo * 1.4f, radioEfectivo * 1.4f);
        sprite->draw();
    }

    glPopMatrix();

    // Restaurar los estados originales de OpenGL
    glPopAttrib();
}

bool BotonCircular::isMouseOver(float mx, float my) {
    float distancia = std::sqrt(std::pow(mx - x, 2) + std::pow(my - y, 2));
    return (distancia <= radio);
}