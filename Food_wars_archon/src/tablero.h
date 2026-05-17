#pragma once
#include "freeglut.h" 
#include "Comida.h"
#include "ETSIDI.h"
#include "Interfaz.h"
#include <vector>
#include <string>

class Tablero {
private:
    // --- CONSTANTES ---
    static constexpr int DIMENSION = 9; // Evitamos el "9" mágico por todo el código
    static constexpr float ladoCasilla = 50.0f;

    // --- MATRIZ Y ESTADO ---
    Comida* casillas[DIMENSION][DIMENSION];
    Bando turnoActual;
    int turnosTotales;

    // --- GESTIÓN DE RATÓN ---
    bool haySeleccion;
    int filaSel, colSel;

    // --- RECURSOS VISUALES ---
    ETSIDI::Sprite fondo_tablero;

    // --- MAGIA Y ESTADOS ---
    bool menuMagiaActivo;
    int hechizoSeleccionado;
    bool esperandoObjetivo;
    bool hechizosSanaUsados[7];
    bool hechizosBasuraUsados[7];

    // --- PIEZAS ELIMINADAS (MORGUE) ---
    std::vector<Comida*> bajasSaludables;
    std::vector<Comida*> bajasBasura;

    // --- VARIABLES AUXILIARES PARA HECHIZOS ---
    Comida* muertoSeleccionado = nullptr;
    Comida* primeraPiezaIntercambio = nullptr;
    Comida* piezaParaTeletransporte = nullptr;
    int pasoTeletransporte = 0;

    // --- CONFIGURACIÓN DE PARTIDA ---
    int dificultadIA;
    int numJugadores;

public:
    Tablero();
    ~Tablero();

    InfoFicha obtenerInfoRaton(float mouseX, float mouseY);

    // Métodos principales
    void inicializa();
    void dibuja(bool pausaActiva);
    void gestionRaton(int boton, int x, int y, bool pausaActiva);
    void gestionTeclado(unsigned char tecla, int x, int y);
    void gestionTeclasEspeciales(int tecla, int x, int y);

    // Getters y Setters (Encapsulación)
    int getTurnoActual() const { return (int)turnoActual; }
    int getTurnos() const { return turnosTotales; }
    bool hayCombatePendiente() const { return combatePendiente; }

    void setTurnoInicial(Bando b) { turnoActual = b; }
    void setDificultad(int d) { dificultadIA = d; }
    void setNumJugadores(int n) { numJugadores = n; }
    void setModoUnJugador(bool m) { modoUnJugador = m; }
    void setBandoIA(int b) { bandoIA = b; }
    bool esModoUnJugador() const { return modoUnJugador; }
    int getBandoIA() const { return bandoIA; }

    // Lógica de juego
    bool esPuntoDePoder(int f, int c) const;
    bool esCasillaOscilante(int f, int c) const;
    int comprobarVictoria();
    void resolverCombate(int ganador);

    //  VARIABLES DE LA IA 
    bool modoUnJugador = true; //treu de momento para pruebas
    int bandoIA = 2; // Asumimos que el 1 es SALUDABLE y el 2 es BASURA. La IA será la BASURA.
    int bandoJugador1 = 1; // El bando que eligió el jugador 1 (1=SALUDABLE, 2=BASURA)

    // El "cerebro" que programaremos en el siguiente paso
    void jugarTurnoIA();
    bool caminoLibre(int fOrg, int cOrg, int fDest, int cDest);
    int getTurnosTotales() const { return turnosTotales; }

    // Variables de comunicación (se mantienen públicas para facilitar el acceso rápido del Mundo)
    bool combatePendiente = false;
    Comida* atacantePendiente = nullptr;
    Comida* defensorPendiente = nullptr;

    // GETTERS Y SETTERS PARA MUNDO 

    // Nombres (usamos referencias para que Mundo pueda modificarlos)
    std::string nombreJugador1 = "";
    std::string nombreJugador2 = "";
    std::string bufferEscritura = ""; // Lo que se escribe en tiempo real
    int nombresRecogidos = 0;         // Contador (0, 1 o 2)
    int maxNombresNecesarios = 2;     // Por defecto 2, si es contra IA será 1
    char nombreSana[50];
    char nombreBasura[50];

    std::string& getBuffer() { return bufferEscritura; }
    std::string getNombreJ1() const { return nombreJugador1; }
    std::string getNombreJ2() const { return nombreJugador2; }
    void setNombreJ1(std::string n) { nombreJugador1 = n; }
    void setNombreJ2(std::string n) { nombreJugador2 = n; }

    // Contadores
    int getNombresRecogidos() const { return nombresRecogidos; }
    void incrementarNombres() { nombresRecogidos++; }
    int getMaxNombres() const { return maxNombresNecesarios; }
    void setMaxNombres(int max) { maxNombresNecesarios = max; }
    void resetNombresRecogidos() { nombresRecogidos = 0; }


    // Acceso a los char arrays (para funciones de C antiguo)
    char* getNombreSana() { return nombreSana; }
    char* getNombreBasura() { return nombreBasura; }


    std::string getBuffer() const { return bufferEscritura; }
    void limpiarBuffer() { bufferEscritura = ""; }
    void borrarUltimaLetraBuffer() {
        if (!bufferEscritura.empty()) bufferEscritura.pop_back();
    }
    void anadirLetraBuffer(char c) { bufferEscritura += c; }
};