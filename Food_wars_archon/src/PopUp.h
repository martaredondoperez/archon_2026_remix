#pragma once
#include <vector>
#include <string>
#include "Boton.h" 

class PopUp {
private:
    float x, y, ancho, alto;
    float r, g, b; // Faltaban estas variables
    bool visible;
    std::string titulo;
    std::vector<std::string> lineas;
    Boton *botonCerrar;

public:
    // Corregido: std::string (con espacio) y nombres de parámetros consistentes
    PopUp(std::string _titulo, float _x, float _y, float _ancho, float _alto);
    ~PopUp();
    void setVisible(bool v) { visible = v; }
    bool esVisible() { return visible; }

    void anadirLinea(const std::string& texto);
    void setColor(float _r, float _g, float _b) { r = _r; g = _g; b = _b; }
    void dibuja();
    bool gestionarClick(float mouseX, float mouseY);
};