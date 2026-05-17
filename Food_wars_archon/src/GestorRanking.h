#pragma once
#ifndef GESTOR_RANKING_H
#define GESTOR_RANKING_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

class EntradaRanking {
private:
    std::string nombre;
    int turnos;
    std::string bando;

public:
    // Constructor por defecto
    EntradaRanking() : nombre(""), turnos(0), bando("") {}

    // Constructor con parámetros
    EntradaRanking(const std::string& _nombre, int _turnos, const std::string& _bando)
        : nombre(_nombre), turnos(_turnos), bando(_bando) {}

    // Getters
    const std::string& getNombre() const { return nombre; }
    int getTurnos() const { return turnos; }
    const std::string& getBando() const { return bando; }

    // Setters
    void setNombre(const std::string& _nombre) { nombre = _nombre; }
    void setTurnos(int _turnos) { turnos = _turnos; }
    void setBando(const std::string& _bando) { bando = _bando; }
};

class GestorRanking {
private:
    std::string rutaArchivo;
    std::vector<EntradaRanking> ranking;

    // Métodos privados
    bool archivoExiste() const;
    void cargarDelArchivo();
    void guardarEnArchivo();

public:
    // Constructor
    GestorRanking(const std::string& ruta = "ranking.dat");

    // Destructor
    ~GestorRanking() = default;

    // Métodos públicos
    void agregarEntrada(const std::string& nombre, int turnos, const std::string& bando);
    std::vector<EntradaRanking> obtenerRanking() const;
    std::vector<EntradaRanking> obtenerTop5() const;
    int obtenerPuestoJugador(const std::string& nombre) const;
    void limpiarRanking();
    bool guardar();
    bool cargar();
    int getTamanio() const { return (int)ranking.size(); }
    int obtenerPuestoExactoPartida(const std::string& nombre, int turnos, const std::string& bando) const;
    int obtenerTotalJugadores() const;

};

#endif // GESTOR_RANKING_H
