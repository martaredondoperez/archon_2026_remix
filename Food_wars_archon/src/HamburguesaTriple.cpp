#include "HamburguesaTriple.h"

HamburguesaTriple::HamburguesaTriple(int f, int c) : pesada(BASURA, f, c) {
    nombre = "Hamburguesa Triple";
    sprite = new ETSIDI::SpriteSequence("imagenes/PESADA_Hamburguesa_Triple.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}