#ifndef MUNDO_H
#define MUNDO_H

#include "Definiciones.h"
#include "freeglut.h"
#include "Interfaz.h"
#include "tablero.h"
#include "Arena.h"
#include "GestorPantalla.h"
#include "GestorRanking.h"

class Mundo {
public:
    friend class Interfaz;
    // Constructor: Inicializa el estado
    Mundo() {
        estadoActual = MENU_PRINCIPAL;
    }

    void inicializa();
    void dibuja();
    void actualizaFisicas();
    // Gestión de entradas
    void teclado(unsigned char tecla, int x, int y);
    void tecladoUp(unsigned char tecla, int x, int y);
    void teclasEspecialesUp(int tecla, int x, int y);

    void mouse(int button, int state, int x, int y);
    void mousePasivo(int x, int y);
    static float mouseX, mouseY;
    int dificultadIA;
    void teclasEspeciales(int tecla, int x, int y);
    int ganadorJuego = 0; // Guardará el 1 o el 2
    void setEstado(Estado nuevoEstado) { estadoActual = nuevoEstado; }
    void setInfoActual(EstadoInfo nuevaInfo) { infoActual = nuevaInfo; }

    void setNumJugadores(int n) { numJugadores = n; }

    // Método para seleccionar bando y pasar a la siguiente pantalla
    void seleccionarBando(int bando) {
        //bandoSeleccionado = bando;
        estadoActual = TABLERO; // O el siguiente estado según tu PDF
        tablero.inicializa();
    }
    int getEstadoActual() { return estadoActual; }
    EstadoInfo getInfoActual() { return infoActual; }

private:
    // ATRIBUTOS (Solo una vez cada uno)
    Estado estadoActual;
    Interfaz interfaz;
    EstadoInfo infoActual;
    Tablero tablero;
    Arena arena;
    GestorPantalla gestorPantalla;
    GestorRanking gestorRanking;
    int numJugadores;       // Para guardar si eligieron 1 o 2
    int bandoSeleccionado;  // Para guardar si eligieron Healthy o Junk
    // Definimos unos números para los bandos (puedes usar un enum si prefieres)
    const int HEALTHY = 1;
    const int JUNK = 2;
    bool pausa;
    void registrarVictoria(int ganador, int turnosTotales);

};

#endif                                                                                         