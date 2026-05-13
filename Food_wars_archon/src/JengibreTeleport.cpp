#include "JengibreTeleport.h"

JengibreTeleport::JengibreTeleport(int f, int c) : Especial(SALUDABLE, f, c) {
    nombre = "Jengibre Teleport";
    sprite = new ETSIDI::SpriteSequence("imagenes/ESPECIAL_Jengibre_Teleport.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}