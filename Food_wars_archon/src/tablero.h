#pragma once
#include "freeglut.h" 
#include "Comida.h"
#include "ETSIDI.h"
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
    std::string nombreJugador1, nombreJugador2, bufferEscritura;
    int nombresRecogidos, maxNombresNecesarios;
    char nombreSana[50], nombreBasura[50];
    bool modoUnJugador;
    int bandoIA;

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
    Tablero();
    ~Tablero();

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
    void jugarTurnoIA();
    bool caminoLibre(int fOrg, int cOrg, int fDest, int cDest);

    // Variables de comunicación (se mantienen públicas para facilitar el acceso rápido del Mundo)
    bool combatePendiente = false;
    Comida* atacantePendiente = nullptr;
    Comida* defensorPendiente = nullptr;

    // GETTERS Y SETTERS PARA MUNDO 

    // Nombres (usamos referencias para que Mundo pueda modificarlos)
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