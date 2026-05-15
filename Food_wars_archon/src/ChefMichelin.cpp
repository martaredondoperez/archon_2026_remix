#include "ChefMichelin.h"

ChefMichelin::ChefMichelin(int f, int c) : Lider(SALUDABLE, f, c) {
    nombre = "Chef Michelin";
    sprite = new ETSIDI::SpriteSequence("imagenes/LIDER_Chef_Michelin.png", 4, 1, 150);
    if (sprite != nullptr) {
        sprite->setSize(45, 45);
        sprite->setCenter(22.5, 22.5);
    }
}