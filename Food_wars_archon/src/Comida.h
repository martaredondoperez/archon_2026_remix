#pragma once
#include "Definiciones.h"
#include "ETSIDI.h"
#include <string>
// CLASE PADRE (Abstracta)
class Comida {
public: // Lo dejamos público para no romper vuestro Tablero.cpp
    int fila, columna;
    Bando bando;
    TipoFicha tipo;
    int rangoMovimiento;
    bool encarcelada = false;
    int vidaMax;
    int vidaActual;
    int ataque;
    bool volador = false;
    bool estaEncarcelada = false;
    bool seleccionadaParaHechizo = false;
    std::string nombre; 
    ETSIDI::SpriteSequence* sprite = nullptr;

    float velocidadMov;
    float rangoAtaque;
    float cadencia;

    // Constructor Padre (solo inicializa lo básico)
    Comida(Bando b, TipoFicha t, int f, int c) : bando(b), tipo(t), fila(f), columna(c) {}

    // Destructor virtual 
    virtual ~Comida() { delete sprite; }

    // ¡LA MAGIA DEL POLIMORFISMO!
    // Al poner "= 0", obligamos a las clases hijas a escribir este código
    virtual bool intentarMover(int nuevaFila, int nuevaColumna) = 0;

    // El resto de funciones comunes se quedan aquí tal cual las teníais:
    virtual void recibirDano(int cantidad);
    virtual void dibuja(float xMin, float yMin, float lado);
    virtual void actualiza(float t);
    Bando getBando() { return bando; }
};
