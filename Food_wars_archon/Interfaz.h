#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H

class Interfaz {
public:
    void dibujaMenuPrincipal();
    void dibujaSeleccionBando();
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaPausa();
    void dibujaFinal();
    void dibujaBoton(float x, float y, float ancho, float alto, const char* texto);
};

#endif