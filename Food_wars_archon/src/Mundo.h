#ifndef MUNDO_H
#define MUNDO_H

#include "Definiciones.h"
#include "freeglut.h"
#include "Interfaz.h"
#include "tablero.h"
#include "Arena.h"

struct EntradaRanking {
    char nombre[50];
    int turnos;
    char bando[20]; // "Sana" o "Basura"
};

class Mundo {
public:
    // Constructor: Inicializa el estado
    Mundo() {
        estadoActual = MENU_PRINCIPAL;
    }

    void inicializa();
    void dibuja();

    // Gestión de entradas
    void teclado(unsigned char tecla, int x, int y);
    void tecladoUp(unsigned char tecla, int x, int y);
    void teclasEspecialesUp(int tecla, int x, int y);

    void mouse(int button, int state, int x, int y);
    static void mousePasivo(int x, int y);
    static float mouseX, mouseY;
    int dificultadIA;
    void teclasEspeciales(int tecla, int x, int y);

    int ganadorJuego = 0; // Guardará el 1 o el 2
private:
    // ATRIBUTOS (Solo una vez cada uno)
    Estado estadoActual;
    Interfaz interfaz;
    EstadoInfo infoActual = NINGUNA;
    Tablero tablero;
    Arena arena;
    int numJugadores;       // Para guardar si eligieron 1 o 2
    int bandoSeleccionado;  // Para guardar si eligieron Healthy o Junk
    // Definimos unos números para los bandos (puedes usar un enum si prefieres)
    const int HEALTHY = 1;
    const int JUNK = 2;
    bool pausa;
    void registrarVictoria(int ganador, int turnosTotales);

};

#endif                                                                                         