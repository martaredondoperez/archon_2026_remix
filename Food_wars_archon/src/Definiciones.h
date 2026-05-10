#pragma once
#ifndef DEFINICIONES_H
#define DEFINICIONES_H

enum Bando { SALUDABLE=1, BASURA=2 };
enum TipoFicha { LIDER, ESPECIAL, VOLADORA, PESADA, DISTANCIA, ELEMENTAL
};
enum Estado {
    INICIO,
    MENU_PRINCIPAL,
    MENU_DIFICULTAD,
    SELECCION_BANDO,
    INSTRUCCIONES,
    TABLERO,
    ARENA,
    PAUSA,
    GAMEOVER
};

//Modos de juego
enum Modo { JvJ, JvM };
#endif
//Informacion de cada bando
enum EstadoInfo { NINGUNA, INFO_HEALTHY, INFO_JUNK, INFO_GENERAL, INFO_AJUSTES};
