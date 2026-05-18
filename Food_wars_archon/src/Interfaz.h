#pragma once
#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <vector>
#include <string>
#include <map>
#include "ETSIDI.h"
#include "Boton.h"
#include "BotonRectangular.h"
#include "BotonCircular.h"
#include "Definiciones.h"
#include "PopUp.h"
#include "GestorRanking.h"

struct InfoFicha {
    std::string nombre;
    int vidaActual;
    int vidaMax;
    bool activa = false; 
    int ataque;
};

class Mundo;

class Interfaz {
private:
    // Elementos gráficos - SOLO PUNTEROS PARA INICIALIZACIÓN DIFERIDA
    ETSIDI::Sprite fondo;
    ETSIDI::Sprite logo;
    ETSIDI::Sprite fondoSeleccion;
    ETSIDI::Sprite iconoPausa;
    ETSIDI::Sprite iconoAjustes;
    ETSIDI::Sprite iconoInfo;
    ETSIDI::Sprite iconoVolver;
    ETSIDI::Sprite iconoInfoJunk;

    std::map<Estado, std::vector<Boton*>> mapaBotones;

    // Método interno de limpieza
    void limpiarBotones();

    Mundo* mundo; // Referencia al mundo para las acciones

public:
    PopUp* popUpActivo; // Público para que Mundo pueda acceder a él
    PopUp* popUpAjustes; // Público para gestionar clics en los botones
    PopUp* popUpInfoHealthy;
    PopUp* popUpInfoJunk;
    PopUp* popUpGuia;
    PopUp* popUpRanking;
    Interfaz();
    ~Interfaz();

    void inicializa(Mundo* m);

    // --- GESTIÓN POLIMÓRFICA ---
    void dibujaBotones(Estado estadoActual, EstadoInfo infoActual);
    void gestionarClick(float mx, float my, Estado estadoActual);
    void actualizaEstadoBotones(float mx, float my, Estado estadoActual);
    void actualizarMouse(float mx, float my);

    // --- DIBUJO DE PANTALLAS ---
    void dibujaMenu();
    void dibujaSeleccion();
    void dibujaInstrucciones();
    void dibujaPausa();
    void dibujaFinal(int ganador);
    
    // --- HERRAMIENTAS DE UI ---
    void dibujaTexto(const std::string& texto, float x, float y, float r, float g, float b);
    void dibujaPopUp(const char* titulo, const std::vector<std::string>& lineas, float r, float g, float b);
    void dibujaHUDJuego(InfoFicha info);
    void dibujaMenuConfig(bool musicaActiva);
    void mostrarInfoBando(int bando);
    void mostrarInfoTablero(int tipo);
    void mostrarRanking();
    void dibujaPantallaNombre(int numJugador, std::string nombreActual);

    // Helper methods implemented in Interfaz.cpp
    void dibujaBotonCircular(float cx, float cy, float radio, ETSIDI::Sprite& imagen, float r, float g, float b);
    bool botonPulsado(float mouseX, float mouseY, float btnX, float btnY, float btnAncho, float btnAlto);
    bool botonCircularPulsado(float clickX, float clickY, float cx, float cy, float radio);

    // En Interfaz.h (Sección pública)
    void dibujaMenuRanking(const GestorRanking* gestor, const std::string& nombreJugadorActual, int turnosActuales, const std::string& bandoActual);
};

#endif