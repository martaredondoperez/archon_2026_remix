#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H
#include <vector>
#include <string>
#include "ETSIDI.h"
#include <algorithm>

class Interfaz {
private:
    // Elementos gráficos
    ETSIDI::Sprite fondo;
    ETSIDI::Sprite logo;
    ETSIDI::Sprite fondoSeleccion;
    ETSIDI::Sprite iconoPausa;
    ETSIDI::Sprite iconoAjustes;
    ETSIDI::Sprite iconoInfo;
    ETSIDI::Sprite iconoVolver;
    ETSIDI::Sprite fondo_menu_dificultad;
public:
    Interfaz();

    // Funciones de Pantallas Completas
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaInstrucciones();
    void dibujaPausa();
    void dibujaFinal(int ganador);
    void dibujaMenuDificultad();

    // Funciones de Elementos UI (Herramientas de dibujo)
    void dibujaBoton(float x, float y, float ancho, float alto, const char* texto, float r1, float g1, float b1, float r2, float g2, float b2);    
    void dibujaTexto(const char* texto, float x, float y, float r, float g, float b);
    void dibujaPopUp(const char* titulo, const std::vector<std::string>& lineas, float r, float g, float b);
    void dibujaBotonCircular(float cx, float cy, float radio, ETSIDI::Sprite& imagen, float r, float g, float b);
    bool botonPulsado(float mouseX, float mouseY, float btnX, float btnY, float btnAncho, float btnAlto);
    void dibujaHUDJuego();
    void dibujaMenuConfig(bool musicaActiva);
    bool botonCircularPulsado(float clickX, float clickY, float cx, float cy, float radio);
    void mostrarInfoBando(int bando);
    void mostrarInfoTablero(int tipo);
    void dibujaPantallaNombre(int numJugador, std::string nombreActual);
    void dibujaMenuRanking(std::string nombreJugadorActual);

};

#endif