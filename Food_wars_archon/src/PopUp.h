#pragma once
#include <vector>
#include <string>
#include <functional>
#include "Boton.h" 

class PopUp {
private:
    float x, y, ancho, alto;
    float r, g, b;
    bool visible;
    std::string titulo;
    std::vector<std::string> lineas;
    Boton* botonCerrar; // Mantenemos el puntero a la clase base (Polimorfismo)
    std::vector<Boton*> botonesInteractivos; // Botones adicionales dentro del popup

public:
    PopUp(std::string _titulo, float _x, float _y, float _ancho, float _alto);
    ~PopUp();
    void setVisible(bool v) { visible = v; }
    bool esVisible() { return visible; }

    void anadirLinea(const std::string& texto);
    void setColor(float _r, float _g, float _b) { r = _r; g = _g; b = _b; }
    void actualizarMouse(float mx, float my);
    void dibuja();
    bool gestionarClick(float mouseX, float mouseY);

    // Nuevos métodos para botones interactivos
    void anadirBoton(Boton* boton);
    void limpiarBotones();
};