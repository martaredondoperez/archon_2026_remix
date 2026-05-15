#include "ReyHamburguesa.h"

ReyHamburguesa::ReyHamburguesa(int f, int c) : Lider(BASURA, f, c) {
    nombre = "Rey Hamburguesa";
    sprite = new ETSIDI::SpriteSequence("imagenes/LIDER_Rey_Hamburguesa.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}