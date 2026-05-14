#pragma once
#include "freeglut.h"
#include <functional>
#include <string>

class Boton {
protected:
    float x, y;          // Posición
    float r, g, b;  
    bool mouseOver;
    std::function<void()> accion;
    std::string texto;
public:
    Boton(float _x, float _y, float _r, float _g, float _b, std::string _texto) : x(_x), y(_y), r(_r), g(_g), b(_b), texto(_texto) {}
    // Método para asignar la tarea
    void setAccion(std::function<void()> _accion) { accion = _accion; }

    // Método para ejecutar la tarea
    void ejecutar() { if (accion) accion(); }

    virtual void dibuja() = 0;
    virtual bool isMouseOver(float mx, float my) = 0;

    void gestionarEfectos(float& escala, float& brilloExtra) {
        if (mouseOver) {
            escala = 1.1f;      // 10% más grande
            brilloExtra = 0.2f; // Un poco más claro
        }
        else {
            escala = 1.0f;
            brilloExtra = 0.0f;
        }
    }
    std::string getTexto() { return texto; }

    void setMouseOver(bool over) { mouseOver = over; }
};