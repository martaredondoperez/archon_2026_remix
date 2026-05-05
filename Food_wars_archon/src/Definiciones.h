#pragma once
#ifndef DEFINICIONES_H
#define DEFINICIONES_H

enum Bando { SALUDABLE, BASURA };
enum TipoFicha { LIDER, ESPECIAL, VOLADORA, PESADA, DISTANCIA }; 
enum Estado {
    INICIO,
    MENU_PRINCIPAL,
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
enum EstadoInfo { NINGUNA, INFO_HEALTHY, INFO_JUNK };
