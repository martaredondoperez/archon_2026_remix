#ifndef MUNDO_H
#define MUNDO_H

#include "Definiciones.h"
#include "freeglut.h"
#include "Interfaz.h"

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
    void gestionaRaton(int boton, int estado, int x, int y);

private:
    // ATRIBUTOS (Solo una vez cada uno)
    Estado estadoActual;
    Interfaz interfaz;
    EstadoInfo infoActual = NINGUNA;

};

#endif                                                                                         