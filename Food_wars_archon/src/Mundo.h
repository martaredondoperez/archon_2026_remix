#ifndef MUNDO_H
#define MUNDO_H

#include "Definiciones.h"
#include "freeglut.h"
#include "Interfaz.h"
#include "tablero.h"
#include "Luchador.h"
#include <vector>
#include "Proyectil.h"

class Mundo {
public:
    // Constructor: Inicializa el estado
    Mundo() {
        estadoActual = MENU_PRINCIPAL;
    }

    void inicializa();
    void dibuja();
    void update();

    // Gestión de entradas
    void teclado(unsigned char tecla, int x, int y);
    void teclaUp(unsigned char tecla, int x, int y); // SI SUELTA PARA DE MOVERSE
    void gestionaRaton(int boton, int estado, int x, int y);

private:
    // ATRIBUTOS (Solo una vez cada uno)
    Estado estadoActual;
    Interfaz interfaz;
    EstadoInfo infoActual = NINGUNA;
    Tablero tablero;
    Luchador jugador1;

    std::vector<Proyectil> disparos;
};

#endif                                                                                         