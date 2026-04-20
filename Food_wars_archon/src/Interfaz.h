#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H
#include "ETSIDI.h"

class Interfaz {
    ETSIDI::Sprite fondo; // Declaramos el objeto fondo
    ETSIDI::Sprite logo;
    ETSIDI::Sprite fondoSeleccion;
public:
    Interfaz(); // Necesitamos el constructor para inicializar la imagen
    void dibujaMenuPrincipal();
    void dibujaSeleccionBando();
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaInstrucciones();
    void dibujaPausa();
    void dibujaFinal();
    void dibujaBoton(float x, float y, float ancho, float alto, const char* texto, bool esVerde=true);
    void dibujaTexto(const char* texto, float x, float y, float r, float g, float b);
    void dibujaPopUp(const char* titulo, const char* descripcion, bool esVerde);


};

#endif