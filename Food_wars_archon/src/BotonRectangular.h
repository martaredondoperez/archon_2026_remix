#pragma once
#include "Boton.h"
#include <string>

class BotonRectangular : public Boton {
private:
    float ancho, alto;
    float r2, g2, b2; // Segundo color para el degradado
public:
    bool mouseOver = false;
    BotonRectangular(float _x, float _y, float _w, float _h, std::string _t,
        float _r1, float _g1, float _b1,
        float _r2, float _g2, float _b2);

    void dibuja() override;
    bool isMouseOver(float mx, float my) override;
    void setTexto(std::string t) { texto = t; }
    void setColores(float _r1, float _g1, float _b1, float _r2, float _g2, float _b2) {
        r = _r1; g = _g1; b = _b1; // r, g, b vienen de la madre
        r2 = _r2; g2 = _g2; b2 = _b2; // r2, g2, b2 son del hijo rectangular
    }

};