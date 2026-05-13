#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H
#include <vector>
#include <string>
#include "ETSIDI.h"
#include <algorithm>
#include "BotonRectangular.h"
#include "BotonCircular.h"
#include "Boton.h"
#include "Definiciones.h"
#include <map>

struct InfoFicha {
    std::string nombre;
    int vidaActual;
    int vidaMax;
    bool activa = false; // Indica si hay una ficha bajo el ratón o no
    int ataque;
};
class Mundo;
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

    // Agrupamos los botones por "pantallas"
    std::vector<Boton*> botonesMenuPrincipal;
    std::vector<Boton*> botonesSeleccion;
    std::vector<Boton*> botonesDificultad;
    std::vector<Boton*> botonesInstrucciones;
    std::vector<Boton*> botonesConfig;
    std::vector<Boton*> botonesFinal;
    std::vector<Boton*> botonesPopUp;
    std::vector<Boton*> botonesNombre;
    std::vector<Boton*> botonesRanking;


public:
    Interfaz();
    ~Interfaz();
    void inicializa(Mundo* mundo);
    // Método para detectar qué botón se pulsó
    Boton* getBotonPulsado(const std::vector<Boton*>& lista, float mx, float my);
    std::vector<Boton*>& getBotonesPopUp() { return botonesPopUp; }
    //Puntero menu pricipal
    Boton* boton1jugador;
    Boton* boton2jugador;
    Boton* botoninstrucciones;
    Boton* botonsalir;
    // Punteros menu seleccion bandos
    Boton* btnHealthy;
    Boton* btnJunk;
    Boton* btnVolverSeleccion;
    Boton* btnInfoHealthy;
    Boton* btnInfoJunk;
    //punteros menu instrucciones
    Boton* btnVolverInstrucciones;
    //punteros menu configuracion
    BotonRectangular* btnMusica; // Lo declaramos como Rectangular para acceder a sus métodos de cambio de texto
    Boton* btnCerrarConfig;
    //punteros menu final
    Boton* btnRanking;
    Boton* btnReintentar;
    Boton* btnSalirFinal;
    //punteros popup
    BotonRectangular* btnCerrarPopUp; // El botón "X"
    //punteros menu pantalla nombre
    Boton* btnVolverNombre;
    //punteros menu ranking
    Boton* btnVolverRanking;
    // Funciones de Pantallas Completas
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaInstrucciones();
    void dibujaPausa();
    void dibujaFinal(int ganador);

    std::vector<Boton*>* getBotonesActivos(Estado estado) {
        switch (estado) {
        case MENU_PRINCIPAL:   return &botonesMenuPrincipal;
        case SELECCION_BANDO:  return &botonesSeleccion;
        case INSTRUCCIONES:    return &botonesInstrucciones;
        case PANTALLA_NOMBRE:  return &botonesNombre;
        //case FIN:    return &botonesConfig; // O el nombre que uses para pausa
        case RANKING:          return &botonesRanking;
        case GAMEOVER:    return &botonesFinal;
        default:               return &botonesMenuPrincipal; // Por defecto
        }
    }
    // Funciones de Elementos UI (Herramientas de dibujo)
    void dibujaTexto(const char* texto, float x, float y, float r, float g, float b);
    void dibujaPopUp(const char* titulo, const std::vector<std::string>& lineas, float r, float g, float b);
    void dibujaHUDJuego(InfoFicha info);
    void dibujaMenuConfig(bool musicaActiva);
    void mostrarInfoBando(int bando);
    void mostrarInfoTablero(int tipo);
    void dibujaPantallaNombre(int numJugador, std::string nombreActual);
    void dibujaMenuRanking(std::string nombreJugadorActual);
    Boton* detectarClick(const std::vector<Boton*>& lista, float mx, float my);
    void actualizaEstadoBotones(float mx, float my, Estado estadoActual);

};

#endif