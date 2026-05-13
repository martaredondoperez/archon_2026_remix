#include "RodajaLimon.h"

RodajaLimon::RodajaLimon(int f, int c) : Voladora(SALUDABLE, f, c) {
    nombre = "Rodaja de Limon";
    sprite = new ETSIDI::SpriteSequence("imagenes/VOLADORA_Rodaja_Limon.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}