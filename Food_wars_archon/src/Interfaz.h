#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "ETSIDI.h"

class Interfaz {
private:
    // Elementos gráficos
    ETSIDI::Sprite fondo;
    ETSIDI::Sprite logo;
    ETSIDI::Sprite fondoSeleccion;


public:
    Interfaz();

    // Funciones de Pantallas Completas
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaInstrucciones();
    void dibujaPausa();
    void dibujaFinal();

    // Funciones de Elementos UI (Herramientas de dibujo)
    void dibujaBoton(float x, float y, float ancho, float alto, const char* texto, bool esVerde = true);
    void dibujaTexto(const char* texto, float x, float y, float r, float g, float b);
    void dibujaPopUp(const char* titulo, const char* descripcion, bool esVerde);
    void dibujaBotonInfo(float x_boton, float y_boton, float ancho_boton, float alto_boton, bool esVerde);
    bool botonPulsado(float mouseX, float mouseY, float btnX, float btnY, float btnAncho, float btnAlto);

};

#endif