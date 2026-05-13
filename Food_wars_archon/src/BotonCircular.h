#pragma once
#include "Boton.h"
#include "ETSIDI.h"

class BotonCircular : public Boton {
private:
    float radio;
    ETSIDI::Sprite* sprite; // Puntero al sprite que ya tienes cargado
public:
    BotonCircular(float _cx, float _cy, float _rad, ETSIDI::Sprite* _sprite,
        float _r, float _g, float _b);

    void dibuja() override;
    bool isMouseOver(float mx, float my) override;
};