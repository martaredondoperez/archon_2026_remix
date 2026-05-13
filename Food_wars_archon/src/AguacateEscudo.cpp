#include "AguacateEscudo.h"

AguacateEscudo::AguacateEscudo(int f, int c) : pesada(SALUDABLE, f, c) {
    nombre = "Aguacate Escudo";
    sprite = new ETSIDI::SpriteSequence("imagenes/PESADA_Aguacate_Escudo.png", 4, 1, 150);
    if (sprite) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}