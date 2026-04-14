#pragma once
#ifndef DEFINICIONES_H
#define DEFINICIONES_H

// Estados posibles del juego
enum Estado {
    INICIO,
    MENU_PRINCIPAL,
    SELECCION_BANDO,
    TABLERO,
    ARENA,
    PAUSA,
    GAMEOVER
};
// Bandos
enum Bando { HEALTHY, JUNK, NINGUNO };
//Modos de juego
enum Modo { JvJ, JvM };
#endif