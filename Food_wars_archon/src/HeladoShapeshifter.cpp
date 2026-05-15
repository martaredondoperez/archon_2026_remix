#include "HeladoShapeshifter.h"

HeladoShapeshifter::HeladoShapeshifter(int f, int c) : Especial(BASURA, f, c) {
    nombre = "Helado Shapeshifter";
    sprite = new ETSIDI::SpriteSequence("imagenes/ESPECIAL_Helado_Shapeshifter.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}