#pragma once
#include "Luchador.h"

// --- Lider Bando Saludabel ---
class ChefMichelin : public Luchador {
public:
    ChefMichelin();
    void dibuja() override;
};

// --- Lider Bando Fast Food ---
class ReyHamburguesa : public Luchador {
public:
    float tiempoRecarga; // Cronómetro para no disparar infinitamente

    ReyHamburguesa();
    void dibuja() override;

    // La IA que devuelve 'true' cuando decide disparar
    bool piensaYMueve(float targetY);
};