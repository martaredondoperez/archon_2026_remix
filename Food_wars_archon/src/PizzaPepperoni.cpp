#include "PizzaPepperoni.h"

PizzaPepperoni::PizzaPepperoni(int f, int c) : Voladora(BASURA, f, c) {
    nombre = "Pizza Pepperoni";
    sprite = new ETSIDI::SpriteSequence("imagenes/VOLADORA_Pizza_Pepperoni.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}