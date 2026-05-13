#include "ChorroVitaminas.h"

// Fíjate que aquí debe poner ChorroVitaminas, no RodajaLimon
ChorroVitaminas::ChorroVitaminas(int f, int c) : Distancia(SALUDABLE, f, c) {
    nombre = "Chorro de Vitaminas";
    sprite = new ETSIDI::SpriteSequence("imagenes/DISTANCIA_Chorro_Vitaminas.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}