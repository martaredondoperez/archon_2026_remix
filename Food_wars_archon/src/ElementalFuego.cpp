#include "ElementalFuego.h"

ElementalFuego::ElementalFuego(Bando b, int f, int c) : Elemental(b, f, c) {
    nombre = "Elemental de Fuego";
    sprite = new ETSIDI::SpriteSequence("imagenes/ELEMENTAL_Fuego.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}