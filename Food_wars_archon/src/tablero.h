#pragma once
#include "freeglut.h" 
#include "Comida.h"
#include "ETSIDI.h"
#include <vector>
#include <string>
#include "Interfaz.h"

class Tablero {

    Comida* casillas[9][9];
   
    float ladoCasilla=50.0f;
    //memoria raton
    bool haySeleccion; // ficha agarrada?
    int filaSel;       // Fila de la ficha 
    int colSel;        // Columna de la ficha 
    Bando turnoActual;
    int turnosTotales;
    int dificultadIA; // Guardará el nivel elegido
    int numJugadores; // Para saber si activar la IA o no
    ETSIDI::Sprite fondo_tablero;

    // MENU MAGIA
    bool menuMagiaActivo;       
    int hechizoSeleccionado;    
    bool esperandoObjetivo=false;

    // Arrays para recordar qué hechizos se han usado ya (true = gastado)
    bool hechizosSanaUsados[7];
    bool hechizosBasuraUsados[7];

    //LISTA PIEZAS ELIMINADAS
    std::vector<Comida*> bajasSaludables;
    std::vector<Comida*> bajasBasura;
    // Para el hechizo de revivir
    Comida* muertoSeleccionado = NULL; 
    //LISTA HECHIZO INTERCAMBIADOR
    Comida* primeraPiezaIntercambio = NULL;
    //para el hechizo de teletrasnporte
    Comida* piezaParaTeletransporte = NULL; // Para guardar la pieza elegida
    int pasoTeletransporte = 0;           // 0: elegir pieza, 1: elegir destino
public:
    Tablero();          // Constructor
    ~Tablero(); //Destructor
    void dibuja(bool pausaActiva);      // Función que llamará Mundo.cpp
    void inicializa();  // Para resetear el tablero al empezar
    void gestionRaton(int boton, int x, int y, bool pausaActiva); //proceso de raton
    void setTurnoInicial(Bando bandoElegido);
    void setDificultad(int d) { dificultadIA = d; }
    void setNumJugadores(int n) { numJugadores = n; }

    bool esPuntoDePoder(int fila, int columna);
    bool esCasillaOscilante(int fila, int columna);
    int comprobarVictoria();
    void gestionTeclado(unsigned char tecla, int x, int y);
    void gestionTeclasEspeciales(int tecla, int x, int y);

    int getTurnoActual() const { return turnoActual; }

    bool combatePendiente = false;
    Comida* atacantePendiente = NULL;
    Comida* defensorPendiente = NULL;

    // Función para cuando la arena termine
    void resolverCombate(int ganador);

    std::string nombreJugador1 = "";
    std::string nombreJugador2 = "";
    std::string bufferEscritura = ""; // Lo que se escribe en tiempo real
    int nombresRecogidos = 0;         // Contador (0, 1 o 2)
    int maxNombresNecesarios = 2;     // Por defecto 2, si es contra IA será 1
    char nombreSana[50];
    char nombreBasura[50];

    int getTurnos() { return turnosTotales; }

    InfoFicha obtenerInfoRaton(float mouseX, float mouseY);
    //  VARIABLES DE LA IA 
    bool modoUnJugador = true; //treu de momento para pruebas
    int bandoIA = 2; // Asumimos que el 1 es SALUDABLE y el 2 es BASURA. La IA será la BASURA.
    int bandoJugador1 = 1; // El bando que eligió el jugador 1 (1=SALUDABLE, 2=BASURA)

    // El "cerebro" que programaremos en el siguiente paso
    void jugarTurnoIA();
    bool caminoLibre(int fOrg, int cOrg, int fDest, int cDest);
    int getTurnosTotales() const { return turnosTotales; }

};