#include "DispatoKetchup.h"

DisparoKetchup::DisparoKetchup(int f, int c) : Distancia(BASURA, f, c) {
    nombre = "Disparo de Ketchup";
    sprite = new ETSIDI::SpriteSequence("imagenes/DISTANCIA_Disparo_Ketchup.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}